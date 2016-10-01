#include "../Forcelib.h"
#include "Client.h"

#include "URLHelper.h"
#include "Netcode Resources.h"
#include "../log.h"

#include <sstream>

Client::Client()
	: peer{ nullptr }
	, result{ nullptr }
{
}


Client::~Client()
{
	// Free result structure in case we throw
	if (result != nullptr){
		freeaddrinfo(result);
	}
}


void Client::SetLogs(std::vector<std::string> & vct)
{
	StreamLog::ReportingLevel() = logDEBUG4;
	Output2vector::Stream() = &vct;
}



void Client::Request(std::function<void(http_request*)> req, std::function<void(http_response*)> resp, std::string ip, std::string port)
{
	request_func_ = req;
	response_func_ = resp;

	processThread = std::thread(&Client::Process, this, ip, port);
	processThread.detach();
}


unsigned Client::Process(std::string ip, std::string port){
	// Create the address info struct
	struct addrinfo hints;									// The requested address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function


	// Resolve the local address and port to be used by the server
	if (getaddrinfo(ip.c_str(), port.c_str(), &hints, &result) != 0) {
		STREAM_LOG(logERROR) << ERR_GETADDRINFO;
		return 1;
	}

	SOCKET l_sock = INVALID_SOCKET;

	// Loop through all the results and bind to the first we can
	for (struct addrinfo * loopAddr = result; loopAddr != NULL; loopAddr = loopAddr->ai_next)
	{
		// Create the socket
		l_sock = socket(loopAddr->ai_family, loopAddr->ai_socktype, loopAddr->ai_protocol);
		if (l_sock == INVALID_SOCKET) {
			continue;
		}

		// Connect to server.
		if (connect(l_sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	if (l_sock == INVALID_SOCKET) {
		STREAM_LOG(logERROR) << ERR_CONNECT;
		return 1;
	}

	//
	// send
	//

	http_request req;
	request_func_(&req);

	std::string reqUrl;
	URLHelper urlHelper;
	urlHelper.BuildReq(reqUrl, req.path_, req.params_);

	try	{
		Send(l_sock, req.method_ + " ");
		Send(l_sock, reqUrl + " ");
		SendLine(l_sock, "HTTP/1.1");
		SendLine(l_sock, req.header_accept + req.accept_);
		SendLine(l_sock, "");
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
	}
	

	//
	// receive
	//

	std::string response;
	std::string line;

	try {
		line = ReceiveLine(l_sock);
	}
	catch (const std::exception& e)	{
		STREAM_LOG(logERROR) << e.what();
	}

	response += line;

	if (line.empty()) {
		freeaddrinfo(result);
		result = nullptr;
		try {
			Close(l_sock);
		}
		catch (const std::exception& e) {
			STREAM_LOG(logERROR) << e.what();
		}
		return 1;
	}

	http_response resp;

	if (line.find(http::responses::not_found) != std::string::npos) {
		resp.status_ = http::responses::not_found;
	}
	else if (line.find(http::responses::success) != std::string::npos) {
		resp.status_ = http::responses::success;
	}
	else if (line.find(http::responses::ok) != std::string::npos) {
		resp.status_ = http::responses::ok;
	}
	else if (line.find(http::responses::unauthorised) != std::string::npos) {
		resp.status_ = http::responses::unauthorised;
	}

	while (1) {
		try	{
			line = ReceiveLine(l_sock);
		}
		catch (const std::exception& e)
		{
			STREAM_LOG(logERROR) << e.what();
		}

		if (line.empty())
			break;

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0)
			break;

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, resp.header_server.size()) == resp.header_server) {
			resp.server_ = line.substr(resp.header_server.size());
		}
	}

	/////

	response_func_(&resp);

	/////

	try {
		CloseGracefully(l_sock);
	}
	catch (const std::exception& e) {
		STREAM_LOG(logERROR) << e.what();
	}

	// clears no longer needed address info
	freeaddrinfo(result);
	result = nullptr;
	
	return 1;
}