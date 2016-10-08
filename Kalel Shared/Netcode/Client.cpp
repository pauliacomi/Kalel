#include "../Forcelib.h"
#include "Client.h"

#include "URLHelper.h"
#include "Netcode Resources.h"
#include "../log.h"

#include <sstream>
#include <bitset>         // std::bitset

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
	processThread = std::thread(&Client::Process, this, ip, port, req, resp);
	processThread.detach();
}


unsigned Client::Process(std::string ip, std::string port, std::function<void(http_request*)> request_func_, std::function<void(http_response*)> response_func_){
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

	std::stringstream str_str;
	str_str << req.entity_.size();

	std::string reqUrl;
	URLHelper::BuildReq(reqUrl, req.path_, req.params_);

	req.content_length_ = str_str.str();

	std::string request;

	try	{
		request += Send(l_sock, req.method_ + " ");
		request += Send(l_sock, reqUrl + " ");
		request += SendLine(l_sock, "HTTP/1.1");
		request += SendLine(l_sock, http::header::accept + req.accept_);
		if (!req.entity_.empty()) {
			request += SendLine(l_sock, http::header::content_length + req.content_length_);
			request += SendLine(l_sock, http::header::content_type + req.content_type_);
		}
		request += SendLine(l_sock, "");
		if (!req.entity_.empty()) {
			request += Send(l_sock, req.entity_);
		}
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
		try {
			Close(l_sock);
		}
		catch (const std::exception& e) {
			STREAM_LOG(logERROR) << e.what();
		}
		return 1;
	}
	
	STREAM_LOG(logDEBUG) << LOG_REQUEST << request;

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
	else if (line.find(http::responses::ok) != std::string::npos) {
		resp.status_ = http::responses::ok;
	}
	else if (line.find(http::responses::unauthorised) != std::string::npos) {
		resp.status_ = http::responses::unauthorised;
	}

	bool messageToReceive = true;		// expecting a message
	bool messageReceived = false;		// but not ready to receive it, first the header

	while (1) {
		try	{
			if (!messageReceived) {			// no message body is to be received, regular line-based receive 
				line = ReceiveLine(l_sock);
			}
			else {							// a message body is specified using "Content-Length" header, will receive the required number of bytes
				std::stringstream buffer(resp.content_length_);
				u_long bytes;
				buffer >> bytes;

				line = ReceiveBytes(l_sock, bytes);

				if (line.empty()) {
					resp.answer_ = http::responses::bad_request;
				}
				else {
					resp.answer_ = line;
					response += line;
				}
				break;
			}
		}
		catch (const std::exception& e)
		{
			STREAM_LOG(logERROR) << e.what();
		}

		if (line.empty())
			break;

		response += line;

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) {
			if (messageToReceive)
			{
				messageToReceive = false;
				messageReceived = true;
				continue;
			}
			break;
		}

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, http::header::server.size()) == http::header::server) {
			resp.server_ = line.substr(http::header::server.size());
		}
		else if (line.substr(0, http::header::date.size()) == http::header::date) {
			resp.date_ = line.substr(http::header::date.size());
		}
		else if (line.substr(0, http::header::connection.size()) == http::header::connection) {
			resp.connection_ = line.substr(http::header::connection.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			resp.content_length_ = line.substr(http::header::content_length.size());
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			resp.content_type_ = line.substr(http::header::content_type.size());
		}
	}

	STREAM_LOG(logDEBUG) << LOG_RESPONSE << response;

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