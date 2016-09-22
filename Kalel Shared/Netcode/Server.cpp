#include "../Forcelib.h"
#include "Server.h"

#include "http_request.h"
#include "Netcode Resources.h"
#include "URLHelper.h"
#include "base64.h"

#include "../log.h"

#include <sstream>

#define NO_OF_CONN		5      /* Passed to listen() */

Server::request_func Server::request_func_ = 0;

Server::Server()
	: accepting{ false }
	, teptr{ nullptr }
	, result{ nullptr }
{
	StreamLog::ReportingLevel() = logDEBUG4;
	Output2stream::Stream() = &log;
}


Server::~Server()
{
	// Terminate the accepting thread 
	accepting = false;
	if (acceptThread.joinable())
	{
		acceptThread.join();
	}

	// Free result structure in case we throw
	if (result != nullptr) {
		freeaddrinfo(result);
	}

	// Remove all connected sockets
	for (size_t i = 0; i < connectedSockets.size(); i++)
	{
		Close(*connectedSockets[i]);
		delete connectedSockets[i];
	}
	connectedSockets.clear();
}


void Server::Listen(PCSTR port)
{
	// Create the address info struct
	struct addrinfo hints;									// The requested address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function

	
	// Resolve the local address and port to be used by the server
	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		stringex.set(ERR_GETADDRINFO);
		throw stringex;
	}

	// Loop through all the results and bind to the first we can
	struct addrinfo * loopAddr = result;
	for (loopAddr; loopAddr != NULL; loopAddr = loopAddr->ai_next)
	{
		// Create the socket
		sock = socket(loopAddr->ai_family, loopAddr->ai_socktype, loopAddr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			continue;
		}

		// Enable the socket to reuse the address, lose "Address already in use" error message
		int reuseaddr = 1; /* True */
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(int)) == SOCKET_ERROR) {
			continue;
		}
		
		// Bind to the computer IP address - setup the TCP listening socket
		if (bind(sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	if (loopAddr == NULL){
		stringex.set(ERR_CREATEBIND);
		throw stringex;
	}

	// clears no longer needed address info
	freeaddrinfo(result);
	result = nullptr;

	// Listen to newly created socket
	if (listen(sock, NO_OF_CONN) == SOCKET_ERROR) {
		stringex.set(ERR_LISTENING);
		throw stringex;
	}

	STREAM_LOG(logINFO) << "Listening to socket";

}


void Server::Accept(request_func r)
{
	request_func_ = r;

	if (accepting == false)
	{
		accepting = true;
		acceptThread = std::thread(&Server::AcceptLoop, this);
	}
}



void Server::AcceptLoop()
{
	struct timeval tv;				// Time to run loop
	tv.tv_sec = 1;					// 1 seconds
	tv.tv_usec = 0;					// 0 microseconds

	fd_set master;					// master file descriptor list
	fd_set readfds;					// Set of listening sockets
	FD_ZERO(&master);				// Zero them out
	FD_ZERO(&readfds);				// Zero them out
	FD_SET(sock, &master);			// Add only main socket

	while (accepting) {

		readfds = master;
		int selectResult = select(sock + 1, &readfds, NULL, NULL, &tv);

		if ((selectResult < 0) && (errno != EINTR))	{
			try
			{
				stringex.set(ERR_ACCEPT);
				throw stringex;
			}
			catch (const std::exception&)
			{
				teptr = std::current_exception();
			}
		}


		if (FD_ISSET(sock, &readfds))
		{
			// handle new connections

			// Create a client socket
			SOCKET clientSocket;
			struct sockaddr_storage theirAddr;					// The incoming address
			socklen_t size = sizeof theirAddr;					// Size of address struct
			memset(&theirAddr, 0, size);						// Fill addrinfo with zeroes

			clientSocket = accept(sock, (struct sockaddr*)&theirAddr, &size);

			if (clientSocket == INVALID_SOCKET) {
				try
				{
					stringex.set(ERR_ACCEPT);
					throw stringex;
				}
				catch (const std::exception&)
				{
					teptr = std::current_exception();
				}
			}
			else {
				//if (theirAddr.ss_family == AF_INET)
				//{
					 //inet_ntoa(((struct sockaddr_in*)&theirAddr)->sin_addr), ntohs(((struct sockaddr_in*)&theirAddr)->sin_port);
				//}

				STREAM_LOG(logINFO) << "Accepted new socket";

				std::thread(&Server::Process, this, clientSocket).detach();
			}

		}
	}
}




unsigned Server::Process(SOCKET l_sock)
{
	std::string request;
	std::string line = ReceiveLine(l_sock);
	request += line;

	if (line.empty()) {
		Close(l_sock);
		return 1;
	}

	
	//
	//	Get the request
	//
	
	http_request req;

	if (line.find("GET") == 0) {
		req.method_ = "GET";
	}
	else if (line.find("POST") == 0) {
		req.method_ = "POST";
	}
	else if (line.find("PUT") == 0) {
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


	while (1) {
		line = ReceiveLine(l_sock);
		request += line;

		if (line.empty()) 
			break;
		
		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) 
			break;

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, req.authorization.size()) == req.authorization) {
			req.authentication_given_ = true;
			std::string encoded = line.substr(req.authorization.size());
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			req.username_ = decoded.substr(0, pos_colon);
			req.password_ = decoded.substr(pos_colon + 1);
		}
		else if (line.substr(0, req.accept.size()) == req.accept) {
			req.accept_ = line.substr(req.accept.size());
		}
		else if (line.substr(0, req.accept_language.size()) == req.accept_language) {
			req.accept_language_ = line.substr(req.accept_language.size());
		}
		else if (line.substr(0, req.accept_encoding.size()) == req.accept_encoding) {
			req.accept_encoding_ = line.substr(req.accept_encoding.size());
		}
		else if (line.substr(0, req.user_agent.size()) == req.user_agent) {
			req.user_agent_ = line.substr(req.user_agent.size());
		}
	}

	//
	//	Construct the response
	//

	request_func_(&req);

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

	std::string response;

	Send(l_sock, "HTTP/1.1 ");
	response += "HTTP/1.1 ";

	if (!req.auth_realm_.empty()) {
		SendLine(l_sock, "401 Unauthorized");
		Send(l_sock, "WWW-Authenticate: Basic Realm=\"");
		Send(l_sock, req.auth_realm_);
		SendLine(l_sock, "\"");
	}
	else {
		SendLine(l_sock, req.status_);
		response += req.status_ + "\n";
	}
	SendLine(l_sock, std::string("Date: ") + asctime_remove_nl + " GMT");
	SendLine(l_sock, std::string("Server: ") + serverName);
	SendLine(l_sock, "Connection: close");
	SendLine(l_sock, "Content-Type: text/html; charset=ISO-8859-1");
	SendLine(l_sock, "Content-Length: " + str_str.str());
	SendLine(l_sock, "");
	SendLine(l_sock, req.answer_);

	response += std::string("Date: ") + asctime_remove_nl + " GMT" + "\n";
	response += std::string("Server: ") + serverName + "\n";
	response += "Connection: close";
	response += "\n";
	response += "Content-Type: text/html; charset=ISO-8859-1";
	response += "\n";
	response += "Content-Length: " + str_str.str() + "\n";
	response += "\n";
	response += req.answer_ + "\n";
		    

	STREAM_LOG(logINFO) << "Request was: " << request;
	STREAM_LOG(logINFO) << "Response sent: " << response;
	
	// Close connection socket
	CloseGracefully(l_sock);
	return 0;
}