#include "http_server.h"

#include "http_helpers.h"
#include "URLHelper.h"
#include "base64.h"
#include "Netcode Resources.h"

#include "../stringHelpers.h"
#include "../timeHelpers.h"
#include "../log.h"	



HTTPServer::HTTPServer(PCSTR port)
	: accepting{ false }
{
	// Listen on the socket
	listeningSocket.Listen(port);

	LOG(logDEBUG3) << LOG_LISTENING << listeningSocket.GetSocket();
	FILE_LOG(logDEBUG3) << LOG_LISTENING << listeningSocket.GetSocket();
}


HTTPServer::~HTTPServer()
{
	// Terminate the accepting thread 
	accepting = false;
	if (acceptThread.joinable())
	{
		acceptThread.join();
	}
}


void HTTPServer::AddMethod(std::function<void(http_request*, http_response*)> r, std::string url)
{
	funcMap.emplace(std::make_pair(url, r));
}



void HTTPServer::Start()
{
	if (accepting == false)
	{
		accepting = true;
		acceptThread = std::thread(&HTTPServer::AcceptLoop, this);
	}
}



unsigned HTTPServer::AcceptLoop()
{
	LOG(logDEBUG3) << LOG_ACCEPT_ENTER;
	FILE_LOG(logDEBUG3) << LOG_ACCEPT_ENTER;

	listeningSocket.Accept_PrimeSelect();
	
	struct timeval tv;								// Time to run loop
	tv.tv_sec = 1;									// 1 seconds
	tv.tv_usec = 0;									// 0 microseconds

	SOCKET s;
	std::string theirIP;													// The ip in string form
	
	while (accepting) {

		try
		{
			theirIP = listeningSocket.Accept(s, tv);
			if (theirIP.empty()) {
				auto it = threadPool.begin();											// If we time out we can also do thread cleanup here
				while (it != threadPool.end()) {
					if (it->joinable()) {
						it->join();
						it = threadPool.erase(it);
						if (it == threadPool.end()) break;
						else ++it;;
					}
				}
				continue;
			}
		}
		catch (const std::exception& e)
		{
			LOG(logDEBUG1) << e.what();
			FILE_LOG(logDEBUG1) << e.what();
			return 1;
		}

		LOG(logDEBUG3) << LOG_ACCEPTED_SOCK << theirIP;
		FILE_LOG(logDEBUG3) << LOG_ACCEPTED_SOCK << theirIP;

		std::unique_ptr<Socket> acceptedSocket = std::make_unique<Socket>(s);				// Create a client socket pointer from the accepted SOCKET
		acceptedSocket->SetNagle(false);													// Disable Nagle's algorithm, should lead to improved latency
		threadPool.emplace_back(&HTTPServer::Process, this, std::move(acceptedSocket));		// Start the request processing thread
	}

	LOG(logDEBUG3) << LOG_ACCEPT_LEAVE;
	FILE_LOG(logDEBUG3) << LOG_ACCEPT_LEAVE;

	return 0;
}


unsigned HTTPServer::Process(std::unique_ptr<Socket> sock)
{

	LOG(logDEBUG3) << LOG_PROCESS_ENTER << sock->GetSocket();
	FILE_LOG(logDEBUG3) << LOG_PROCESS_ENTER << sock->GetSocket();


	//*************************************************************************************************************************
	//						INCOMING REQUEST
	//*************************************************************************************************************************
	//	Get the request

	http_request request;									// Create request
	http_response response;									// Create response

	if (ReceiveRequest(*sock, request, response) != 0)		// We try to receive
	{														// But exit if an error
		LOG(logDEBUG1) << ERR_HTTP_RECEIVE;
		LOG(logDEBUG1) << LOG_PROCESS_EXIT << sock->GetSocket();
		FILE_LOG(logDEBUG1) << LOG_PROCESS_EXIT << sock->GetSocket();
		
		return 1;											
	}


	//	Process request authentication
	if (!request.authentication.empty())
	{
		if (request.authentication_method == "Basic")
		{
			std::string decodedAuth = base64_decode(request.authentication);		// Decode
			size_t pos_colon = decodedAuth.find(":");
			request.username = decodedAuth.substr(0, pos_colon);
			request.password = decodedAuth.substr(pos_colon + 1);

			if (request.username == "user" && request.password == "")				// TODO: need to set the values here
			{
				response.authentication_given = true;
			}
		}
		else response.error = true;
	}
	else response.error = true;

	//*************************************************************************************************************************
	//						OUTGOING RESPONSE
	//*************************************************************************************************************************

	// Run the corresponding function, but only if there is no error
	if (!response.error) {
		try {
			funcMap[request.path](&request, &response);
		}
		catch (const std::exception& e){

			response.status = http::responses::internal_err;

			LOG(logDEBUG1) << e.what();
			FILE_LOG(logDEBUG1) << e.what();
		}

		if (response.status.empty())								// If the response is not set, then there's an error in the function
		{
			response.status = http::responses::internal_err;
		}
	}

	// Fill remaining headers
	response.http_version		= "HTTP/1.1";
	response.server				= "Kalel Server";
	response.date				= timeh::GMTtime(RFC_1123);
	response.connection			= "close";
	response.www_authentication = "Kalel";
	response.content_length		= stringh::StringFrom<size_t>(response.body.size());

	// Send it
	SendResponse(*sock, response);

	//
	// Exit
	//

	// Make sure all data will get sent before socket is closed
	// sock->SetLinger(true);

	LOG(logDEBUG3) << LOG_PROCESS_EXIT << sock->GetSocket();
	FILE_LOG(logDEBUG3) << LOG_PROCESS_EXIT << sock->GetSocket();

	return 0;
}

//***********************************
//
//	Function receives the request
//		If exits with 0 - the request was recieved (correctly or not) and a response will be sent
//		If exits with 1 - there was an error in the receiving process -> no responses to send back
//
//***********************************
unsigned HTTPServer::ReceiveRequest(Socket & sock, http_request & req, http_response & resp)
{
	std::string requestString;										// This is where we will store the entire request


	//*************** Start by processing the initial request line

	try {													
		requestString = sock.ReceiveLine();							// Get the first line
	}
	catch (const std::exception& e) {
		LOG(logDEBUG1) << e.what();
		FILE_LOG(logDEBUG1) << e.what();
		return 1;
	}

	if (requestString.empty()) { return 1; }						// Check for valid request

	size_t posSpace = requestString.find_first_of(" ");		

	req.method = ParseMethod(requestString.substr(0, posSpace));	// Get and store the method
	if (req.method.empty()) {										// Method might not be implemented

		LOG(logDEBUG1) << LOG_METHOD_UNKNOWN << sock.GetSocket();
		FILE_LOG(logDEBUG1) << LOG_METHOD_UNKNOWN << sock.GetSocket();
		
		resp.error = true;
		resp.status = http::responses::not_impl;
		
		return 0;													// We do not exit as an error, but we exit anyway
	}

	// Process the path and parameters of the URL
	URLHelper::SplitGetReq(requestString.substr(posSpace + 1), req.path, req.params, req.http_version);


	//*************** Now process headers

	std::string line;										// Start receiving the rest of the request
	bool messageToReceive = false;							// not expecting a message body

	while (true) {
		try
		{
			line = sock.ReceiveLine();						// not attempting to receive message body, regular line-based receive 
		}
		catch (const std::exception& e)
		{
			LOG(logDEBUG1) << e.what();
			FILE_LOG(logDEBUG1) << e.what();
		}

		if (line.empty()) {									// if line is empty, there's an error
			return 1;
		}

		requestString += line;

		// Check for CRLF position
		size_t pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) {								// If we find an empty line, it's the end of the headers
			break;
		}

		line = line.substr(0, pos_cr_lf);					// Otherwise we remove the CLRF and see what kind of header it is

		if (line.substr(0, http::header::authorization.size()) == http::header::authorization) {
			std::string authline = line.substr(http::header::authorization.size());
			size_t posSpace = authline.find_first_of(" ");
			req.authentication_method = authline.substr(0, posSpace);
			req.authentication = authline.substr(posSpace + 1);
		}
		else if (line.substr(0, http::header::accept.size()) == http::header::accept) {
			req.accept = line.substr(http::header::accept.size());
		}
		else if (line.substr(0, http::header::accept_language.size()) == http::header::accept_language) {
			req.accept_language = line.substr(http::header::accept_language.size());
		}
		else if (line.substr(0, http::header::accept_encoding.size()) == http::header::accept_encoding) {
			req.accept_encoding = line.substr(http::header::accept_encoding.size());
		}
		else if (line.substr(0, http::header::user_agent.size()) == http::header::user_agent) {
			req.user_agent = line.substr(http::header::user_agent.size());
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			req.content_type = line.substr(http::header::content_type.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			req.content_length = line.substr(http::header::content_length.size());
			if (req.content_length != "0") { messageToReceive = true; }
		}
	}

	//*************** Receive request body (if applicable)

	if (messageToReceive)											// If we are expecting a message body then 
	{
		u_long bytes = stringh::To<u_long>(req.content_length);		// We will receive the amount of bytes specified by the header

		try
		{
			line = sock.ReceiveBytes(bytes);						// So receive the required number of bytes
		}
		catch (const std::exception& e)								// If there's an error we exit
		{
			LOG(logDEBUG1) << e.what();
			FILE_LOG(logDEBUG1) << e.what();
			return 1;
		}

		req.body = line;											// save the message body
		requestString += line;										// 
	}

	//*************** Request is now received

	LOG(logDEBUG4) << sock.GetSocket() << LOG_REQUEST << requestString;
	FILE_LOG(logDEBUG4) << sock.GetSocket() << LOG_REQUEST << requestString;

	return 0;
}


//***********************************
//
//	Function sends the response
//		If exits with 0 - the request was recieved (correctly or not) and a response will be sent
//		If exits with 1 - there was an error in the receiving process -> no responses to send back
//
//***********************************
unsigned HTTPServer::SendResponse(Socket & sock, const http_response & resp)
{

	std::string responseString;

	try
	{
		responseString += sock.Send(resp.http_version + " ");							// Http version
																						// Then response
		if (!resp.authentication_given) {
			responseString += sock.SendLine(http::responses::unauthorised);
		}
		else {
			responseString += sock.SendLine(resp.status);
		}

		// Common response headers
		responseString += sock.SendLine(http::header::date + resp.date);
		responseString += sock.SendLine(http::header::server + resp.server);
		responseString += sock.SendLine(http::header::connection + resp.connection);

		// Unauthenticated response
		if (!resp.authentication_given) {
			responseString += sock.Send(http::header::www_authenticate);
			responseString += sock.SendLine("Basic Realm=\"" + resp.www_authentication + "\"");
			responseString += sock.SendLine("");
			return 0;
		}

		// Authenticated response
		responseString += sock.SendLine(http::header::content_type + resp.content_type);
		responseString += sock.SendLine(http::header::content_length + resp.content_length);
		responseString += sock.SendLine("");
		responseString += sock.Send(resp.body);

	}
	catch (const std::exception& e)
	{
		LOG(logDEBUG) << e.what();
		FILE_LOG(logERROR) << e.what();

		return 1;
	}

	LOG(logDEBUG4) << sock.GetSocket() << LOG_RESPONSE << responseString;
	FILE_LOG(logDEBUG4) << sock.GetSocket() << LOG_RESPONSE << responseString;


	return 0;
}
