#include "../Forcelib.h"
#include "Client.h"

#include "URLHelper.h"
#include "Netcode Resources.h"

#include <sstream>

Client::request_func Client::request_func_ = 0;
Client::response_func Client::response_func_ = 0;

Client::Client()
	: peer{ nullptr }
	, result{ nullptr }
{
}


Client::~Client()
{
	// Client sockets close gracefully
	CloseGracefully(sock);

	// Free result structure in case we throw
	if (result != nullptr){
		freeaddrinfo(result);
	}
}


void Client::Request(request_func req, response_func resp, std::string ip, std::string port = "http")
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
		stringex.set(ERR_GETADDRINFO);
		throw stringex;
	}

	// Loop through all the results and bind to the first we can
	for (struct addrinfo * loopAddr = result; loopAddr != NULL; loopAddr = loopAddr->ai_next)
	{
		// Create the socket
		sock = socket(loopAddr->ai_family, loopAddr->ai_socktype, loopAddr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			continue;
		}

		// Connect to server.
		if (connect(sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	if (sock == INVALID_SOCKET) {
		stringex.set(ERR_CONNECT);
		throw stringex;
	}

	/*iResult = getpeername(clientSocket, peer, sizeof(struct sockaddr));
	if (iResult == SOCKET_ERROR) {
	std::string a("Cannot get identity!");
	throw a;
	}*/
	/////

	// send

	http_request req;
	request_func_(&req);

	std::string reqUrl;
	URLHelper urlHelper;
	urlHelper.BuildReq(reqUrl, req.path_, req.params_);

	Send(sock, req.method_ + " ");
	Send(sock, reqUrl + " ");
	SendLine(sock, "HTTP/1.1");
	SendLine(sock, req.header_accept + req.accept_);
	SendLine(sock, "");


	// receive

	std::string response;
	std::string line = ReceiveLine(sock);
	response += line;

	if (line.empty()) {
		freeaddrinfo(result);
		result = nullptr;
		Close(sock);
		return 1;
	}

	http_response resp;

	if (line.find("404 Not Found") == 0) {
		resp.status_ = "404 Not Found";
	}
	else if (line.find("200 Success") == 0) {
		resp.status_ = "200 Success";
	}
	else if (line.find("202 OK") == 0) {
		resp.status_ = "202 OK";
	}
	else if (line.find("401 Unauthorised") == 0) {
		resp.status_ = "401 Unauthorised";
	}

	while (1) {
		line = ReceiveLine(sock);

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

	response_func_(&resp);

	/////

	// clears no longer needed address info
	freeaddrinfo(result);
	result = nullptr;
}