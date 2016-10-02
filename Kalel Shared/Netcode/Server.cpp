#include "../Forcelib.h"
#include "Server.h"

#include "Netcode Resources.h"
#include "URLHelper.h"
#include "base64.h"

#include "../log.h"

#include <sstream>
#include <bitset>         // std::bitset

#define NO_OF_CONN		5      /* Passed to listen() */

Server::Server()
	: accepting{ false }
	, teptr{ nullptr }
	, result{ nullptr }
{
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


	// Close all sockets?
	/*for (size_t i = 0; i < socketCollection.size(); i++)
	{
		if (socketCollection[i] != INVALID_SOCKET)
		{
			CloseGracefully(socketCollection[i]);
		}
	}
	socketCollection.clear();*/
}

void Server::SetLogs(std::vector<std::string> & vct)
{
	StreamLog::ReportingLevel() = logDEBUG4;
	Output2vector::Stream() = &vct;
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

	STREAM_LOG(logINFO) << LOG_LISTENING;
}


void Server::Accept(std::function<void(http_request*, http_response*)> r)
{
	proc_func_ = r;

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
			catch (const std::exception& e)
			{
				teptr = std::current_exception();
				STREAM_LOG(logERROR) << e.what();
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
			//socketCollection.push_back(clientSocket);

			if (clientSocket == INVALID_SOCKET) {
				try
				{
					stringex.set(ERR_ACCEPT);
					throw stringex;
				}
				catch (const std::exception& e)
				{
					teptr = std::current_exception();
					STREAM_LOG(logERROR) << e.what();
				}
			}
			else {
				STREAM_LOG(logINFO) << LOG_ACCEPTED_SOCK << GetIP(theirAddr);
				std::thread newThread = std::thread(&Server::Process, this, clientSocket);
				newThread.detach();
			}
		}
	}
}


unsigned Server::Process(SOCKET l_sock)
{
	//
	//	Get the request
	//

	std::string request;
	std::string line;

	try	{
		line = ReceiveLine(l_sock);
	}
	catch (const std::exception& e)	{
		STREAM_LOG(logERROR) << e.what();
	}

	request += line;

	if (line.empty()) {
		try	{
			Close(l_sock);
		}
		catch (const std::exception& e)	{
			STREAM_LOG(logERROR) << e.what();
		}
		return 1;
	}

	http_request req;

	if (line.find(http::method::get) == 0) {
		req.method_ = http::method::get;
	}
	else if (line.find(http::method::post) == 0) {
		req.method_ = http::method::post;
	}
	else if (line.find(http::method::put) == 0) {
		req.method_ = http::method::put;
	}
	else if (line.find(http::method::del) == 0) {
		req.method_ = http::method::del;
	}

	std::string path;
	std::map<std::string, std::string> params;

	size_t posStartPath = line.find_first_of(" ") + 1;

	URLHelper urlhelper;
	urlhelper.SplitGetReq(line.substr(posStartPath), path, params);

	req.path_ = path;
	req.params_ = params;

	bool messageToReceive = false;			// not expecting a message
	bool messageReceived = false;			// not ready to receive it, first the header

	while (1) {
		try
		{			
			if (!messageReceived) {			// no message body is to be received, regular line-based receive 
				line = ReceiveLine(l_sock);
			}
			else {							// a message body is specified using "Content-Length" header, will receive the required number of bytes
				std::stringstream buffer(req.content_length_);
				u_long bytes;
				buffer >> bytes;

				line = ReceiveBytes(l_sock, bytes);

				req.entity_ = line;
				request += line;
				break;
			}
		}
		catch (const std::exception& e)
		{
			STREAM_LOG(logERROR) << e.what();
		}

		if (line.empty()) 
			break;
		
		request += line;
		
		// Check for empty line
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

		if (line.substr(0, http::header::authorization.size()) == http::header::authorization) {
			req.authentication_given_ = true;
			std::string encoded = line.substr(http::header::authorization.size());
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			req.username_ = decoded.substr(0, pos_colon);
			req.password_ = decoded.substr(pos_colon + 1);
		}
		else if (line.substr(0, http::header::accept.size()) == http::header::accept) {
			req.accept_ = line.substr(http::header::accept.size());
		}
		else if (line.substr(0, http::header::accept_language.size()) == http::header::accept_language) {
			req.accept_language_ = line.substr(http::header::accept_language.size());
		}
		else if (line.substr(0, http::header::accept_encoding.size()) == http::header::accept_encoding) {
			req.accept_encoding_ = line.substr(http::header::accept_encoding.size());
		}
		else if (line.substr(0, http::header::user_agent.size()) == http::header::user_agent) {
			req.user_agent_ = line.substr(http::header::user_agent.size());
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			req.content_type_ = line.substr(http::header::content_type.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			req.content_length_ = line.substr(http::header::content_length.size());
			messageToReceive = true;
		}
	}

	STREAM_LOG(logDEBUG) << LOG_REQUEST << request;


	//
	//	Construct the response
	//

	http_response resp;
	proc_func_(&req, &resp);

	

	std::stringstream str_str;
	str_str << resp.answer_.size();

	// Get time in GMT format
	time_t ltime;
	time(&ltime);
	tm gmt;
	gmtime_s(&gmt, &ltime);
	char asctime_remove_nl[26];
	asctime_s(asctime_remove_nl, 26, &gmt);
	asctime_remove_nl[24] = 0;

	resp.server_			= "Kalel Server";								//should fill
	resp.date_				= std::string(asctime_remove_nl) + " GMT";
	resp.connection_		= "close";
	resp.content_length_	= str_str.str();


	//
	//	Send the response
	//

	std::string response;

	try
	{
		response += Send(l_sock, "HTTP/1.1 ");

		if (messageToReceive && messageReceived && req.entity_.empty()) {
			resp.status_ = http::responses::bad_request;
			response += SendLine(l_sock, resp.status_);
		}
		else
		{
			if (!req.auth_realm_.empty()) {
				response += SendLine(l_sock, http::responses::unauthorised);
				response += Send(l_sock, http::header::www_authenticate + "Basic Realm=\"");
				response += Send(l_sock, req.auth_realm_);
				response += SendLine(l_sock, "\"");
			}
			else {

				response += SendLine(l_sock, resp.status_);
			}

			response += SendLine(l_sock, http::header::date + resp.date_);
			response += SendLine(l_sock, http::header::server + resp.server_);
			response += SendLine(l_sock, http::header::connection + resp.connection_);
			response += SendLine(l_sock, http::header::content_type + resp.content_type_);
			response += SendLine(l_sock, http::header::content_length + resp.content_length_);
			response += SendLine(l_sock, "");
			response += Send(l_sock, resp.answer_);
		}
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
	}
	
	STREAM_LOG(logDEBUG) << LOG_RESPONSE << response;
	


	//
	// Close connection socket
	//

	try	{
		CloseGracefully(l_sock);
	}
	catch (const std::exception& e)	{
		STREAM_LOG(logERROR) << e.what();
	}

	return 0;
}