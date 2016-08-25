#include "Server.h"
#include "http_request.h"
#include "Netcode Resources.h"
#include "URLHelper.h"
#include "base64.h"

#include <thread>
#include <vector>
#include <sstream>


#define NO_OF_CONN		5      /* Passed to listen() */


Server::Server()
	: accepting{ false }
{
}


Server::~Server()
{
	accepting = false;

	for (size_t i = 0; i < connectedSockets.size(); i++)
	{
		Close(*connectedSockets[i]);
		delete connectedSockets[i];
	}
	connectedSockets.clear();
}


void Server::Listen(PCSTR port)
{
	sock = INVALID_SOCKET;

	// Create the address info struct
	struct addrinfo hints;									// The requested address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function

	
	// Resolve the local address and port to be used by the server
	struct addrinfo *result = NULL;							// Pointer to the result address
	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		stringex.set(ERR_GETADDRINFO);
		throw stringex;
	}

	// Loop through all the results and bind to the first we can
	struct addrinfo * i = result;
	for (i; i != NULL; i = i->ai_next)
	{
		// Create the socket
		sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (sock == INVALID_SOCKET) {
			continue;
		}

		// Enable the socket to reuse the address, lose "Address already in use" error message
		int reuseaddr = 1; /* True */
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(int)) == SOCKET_ERROR) {
			continue;
		}
		
		// Bind to the computer IP address - setup the TCP listening socket
		if (bind(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	if (i == NULL){
		stringex.set(ERR_CREATEBIND);
		throw stringex;
	}

	// clears no longer needed address info
	freeaddrinfo(result);
	result = NULL;

	// Listen to newly created socket
	if (listen(sock, NO_OF_CONN) == SOCKET_ERROR) {
		stringex.set(ERR_LISTENING);
		throw stringex;
	}
}


void Server::Accept()
{
	if (accepting == false)
	{
		accepting = true;
		std::thread(&Server::AcceptLoop, this).detach();
	}
}



void Server::AcceptLoop()
{
	while (accepting) {
		// Accept a client socket
		SOCKET clientSocket;
		struct sockaddr_storage theirAddr;					// The incoming address
		socklen_t size = sizeof theirAddr;					// Size of address struct
		memset(&theirAddr, 0, size);						// Fill addrinfo with zeroes

		clientSocket = accept(sock, (struct sockaddr*)&theirAddr, &size);
		if (clientSocket == INVALID_SOCKET) {
			stringex.set(ERR_ACCEPT);
			throw stringex;
		}
		else {
			//if (theirAddr.ss_family == AF_INET)
			//{
			//	 inet_ntoa(((struct sockaddr_in*)&theirAddr)->sin_addr), ntohs(((struct sockaddr_in*)&theirAddr)->sin_port);
			//}

			std::thread(&Server::Process, this, clientSocket).detach();
		}
	}
}



unsigned Server::Process(SOCKET sock)
{
	std::string line = ReceiveLine(sock);

	if (line.empty()) {
		Close(sock);
		return 1;
	}

	http_request req;
	
	//
	//	Get the request
	//

	if (line.find("GET") == 0) {
		req.method_ = "GET";
	}
	else if (line.find("POST") == 0) {
		req.method_ = "POST";
	}

	if (line.find("PUT") == 0) {
		req.method_ = "PUT";
	}
	else if (line.find("DELETE") == 0) {
		req.method_ = "DELETE";
	}

	std::string path;
	std::map<std::string, std::string> params;

	size_t posStartPath = line.find_first_not_of(" ", 3);

	URLHelper urlhelper;
	urlhelper.SplitGetReq(line.substr(posStartPath), path, params);

	req.status_ = "202 OK";
	req.path_ = path;
	req.params_ = params;

	static const std::string authorization   = "Authorization: Basic ";
	static const std::string accept          = "Accept: "             ;
	static const std::string accept_language = "Accept-Language: "    ;
	static const std::string accept_encoding = "Accept-Encoding: "    ;
	static const std::string user_agent      = "User-Agent: "         ;

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
		else if (line.substr(0, accept.size()) == accept) {
			req.accept_ = line.substr(accept.size());
		}
		else if (line.substr(0, accept_language.size()) == accept_language) {
			req.accept_language_ = line.substr(accept_language.size());
		}
		else if (line.substr(0, accept_encoding.size()) == accept_encoding) {
			req.accept_encoding_ = line.substr(accept_encoding.size());
		}
		else if (line.substr(0, user_agent.size()) == user_agent) {
			req.user_agent_ = line.substr(user_agent.size());
		}
	}

	//
	//	Construct the response
	//

	if (req.path_ == "/") {
		req.answer_ = "<html><head><title>";
		req.answer_ += "Top KEK";
		req.answer_ += "</title></head><body bgcolor='#4444ff'>";
		req.answer_ += "KEKKEK";
		req.answer_ += "</body></html>";
	}

	std::stringstream str_str;
	str_str << req.answer_.size();

	time_t ltime;
	time(&ltime);
	tm gmt;
	gmtime_s(&gmt, &ltime);

	static std::string const serverName = "Some Server";				//should fill

	char asctime_remove_nl[26];
	asctime_s(asctime_remove_nl, 26, &gmt);
	asctime_remove_nl[24] = 0;

	Send(sock, "HTTP/1.1 ");

	if (!req.auth_realm_.empty()) {
		SendLine(sock, "401 Unauthorized");
		Send(sock, "WWW-Authenticate: Basic Realm=\"");
		Send(sock, req.auth_realm_);
		SendLine(sock, "\"");
	}
	else {
		SendLine(sock, req.status_);
	}
	SendLine(sock, std::string("Date: ") + asctime_remove_nl + " GMT");
	SendLine(sock, std::string("Server: ") + serverName);
	SendLine(sock, "Connection: close");
	SendLine(sock, "Content-Type: text/html; charset=ISO-8859-1");
	SendLine(sock, "Content-Length: " + str_str.str());
	SendLine(sock, "");
	SendLine(sock, req.answer_);

	// Close connection socket
	Close(sock);
	return 0;
}