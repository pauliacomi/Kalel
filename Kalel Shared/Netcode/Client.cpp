#include "../Forcelib.h"
#include "Client.h"

#include "URLHelper.h"
#include "Netcode Resources.h"

#include <sstream>

Client::request_func Client::request_func_ = 0;

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


void Client::Connect(std::string ip, std::string port)
{
	ip_ = ip;
	port_ = port;

	requestThread = std::thread(&Client::Start, this);
}


void Client::Start(){
	// Create the address info struct
	struct addrinfo hints;									// The requested address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function


	// Resolve the local address and port to be used by the server
	if (getaddrinfo(ip_.c_str(), port_.c_str(), &hints, &result) != 0) {
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

	std::stringstream str_str;
	str_str << req.answer_.size();

	Send(sock, req.method_);
	Send(sock, reqUrl);
	SendLine(sock, "HTTP/1.1");
	SendLine(sock, req.accept_);


	// receive

	std::string line = ReceiveLine(sock);

	while (1) {
		line = ReceiveLine(sock);

		if (line.empty())
			break;

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0)
			break;

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, authorization.size()) == authorization) {
			req.authentication_given_ = true;
			std::string encoded = line.substr(authorization.size());
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			req.username_ = decoded.substr(0, pos_colon);
			req.password_ = decoded.substr(pos_colon + 1);
		}
	}

	/////

	// clears no longer needed address info
	freeaddrinfo(result);
	result = nullptr;
}