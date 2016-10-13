#include "../Forcelib.h"
#include "Server.h"

#include "http_helpers.h"
#include "URLHelper.h"
#include "stdHelpers.h"
#include "base64.h"
#include "Netcode Resources.h"

// Logging functionality
#include "../log.h"	
#define FILE_LOGGING	"serverlog.txt"		// Comment this line to disable file logging
#define LOG_LEVEL		logDEBUG4			// Change the level of logging here

#include <sstream>

Server::Server(PCSTR port)
	: accepting{ false }
{
#ifdef FILE_LOGGING

	FILELog::ReportingLevel() = LOG_LEVEL;
	FILE * f;
	fopen_s(&f, FILE_LOGGING, "w");
	Output2FILE::Stream() = f;

#endif // FILE_LOGGING

	// Listen on the socket
	listeningSocket.Listen(port);

#ifdef FILE_LOGGING
	FILE_LOG(logINFO) << LOG_LISTENING << listeningSocket.GetSocket();
#endif // FILE_LOGGING
}


Server::~Server()
{
	// Terminate the accepting thread 
	accepting = false;
	if (acceptThread.joinable())
	{
		acceptThread.join();
	}
}

void Server::SetLogs(std::vector<std::string> & vct, std::mutex & mtx)
{
	StreamLog::ReportingLevel() = LOG_LEVEL;
	Output2vector::Stream() = &vct;
	Output2vector::Mutex() = &mtx;

	STREAM_LOG(logINFO) << LOG_LISTENING << listeningSocket.GetSocket();
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



unsigned Server::AcceptLoop()
{

#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG1) << "Entering accept loop";
#endif // FILE_LOGGING

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
				continue;
			}
		}
		catch (const std::exception& e)
		{
			STREAM_LOG(logERROR) << e.what();
			return 1;
		}

		STREAM_LOG(logINFO) << LOG_ACCEPTED_SOCK << theirIP;
#ifdef FILE_LOGGING
		FILE_LOG(logINFO) << LOG_ACCEPTED_SOCK << theirIP;
#endif // FILE_LOGGING

		std::unique_ptr<Socket> acceptedSocket = std::make_unique<Socket>(s);			// Create a client socket pointer from the accepted SOCKET
		std::thread(&Server::Process, this, std::move(acceptedSocket)).detach();		// Start the request processing thread
	}

#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG1) << "Leaving accept loop";
#endif // FILE_LOGGING

	return 0;
}


unsigned Server::Process(std::unique_ptr<Socket> sock)
{

	STREAM_LOG(logDEBUG2) << "Enter thread for socket" << sock->GetSocket();
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG2) << "Enter thread for socket" << sock->GetSocket();
#endif // FILE_LOGGING


	//*************************************************************************************************************************
	//						INCOMING REQUEST
	//*************************************************************************************************************************
	//	Get the request

	std::string requestString;

	try	{													// Get the first line
		requestString = sock->ReceiveLine();
	}
	catch (const std::exception& e)	{
		STREAM_LOG(logERROR) << e.what();
	}

	if (requestString.empty()) {							// Check for valid request
		return 1;
	}

	http_request request;									// Create request class


															// Find the html method the client sent
	size_t posSpace = requestString.find_first_of(" ");

	request.method_ = ParseMethod(requestString.substr(0, posSpace));
	if(request.method_.empty()){
		STREAM_LOG(logDEBUG2) << "Request method unknown: " << sock->GetSocket();
#ifdef FILE_LOGGING
		FILE_LOG(logDEBUG2) << "Request method unknown: " << sock->GetSocket();
#endif // FILE_LOGGING
		return 1;
	}

															// Process the path and parameters of the URL
	URLHelper::SplitGetReq(requestString.substr(posSpace + 1), request.path_, request.params_);



	std::string line;										// Start receiving the rest of the request
	bool messageToReceive = false;							// not expecting a message body

	while (1) {
		try
		{			
			line = sock->ReceiveLine();				// not attempting to receive message body, regular line-based receive 
		}
		catch (const std::exception& e)
		{
			STREAM_LOG(logERROR) << e.what();
		}

		if (line.empty()) 
			break;
		
		requestString += line;
		
		// Check for CRLF position
		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) {						// if the line is empty, it's either the end of the request or we are expecting a message
			if (messageToReceive)					// if we are expecting a message body, receive 
			{
				u_long bytes = To<u_long>(request.content_length_);

				try
				{
					line = sock->ReceiveBytes(bytes);		// get the required number of bytes
				}
				catch (const std::exception& e)
				{
					STREAM_LOG(logERROR) << e.what();
				}

				request.entity_ = line;					// save the message body
				requestString += line;					// log the message body
			}
			break;
		}

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, http::header::authorization.size()) == http::header::authorization) {
			request.authentication_given_ = true;
			std::string encoded = line.substr(http::header::authorization.size());
			std::string decoded = base64_decode(encoded);

			unsigned int pos_colon = decoded.find(":");

			request.username_ = decoded.substr(0, pos_colon);
			request.password_ = decoded.substr(pos_colon + 1);
		}
		else if (line.substr(0, http::header::accept.size()) == http::header::accept) {
			request.accept_ = line.substr(http::header::accept.size());
		}
		else if (line.substr(0, http::header::accept_language.size()) == http::header::accept_language) {
			request.accept_language_ = line.substr(http::header::accept_language.size());
		}
		else if (line.substr(0, http::header::accept_encoding.size()) == http::header::accept_encoding) {
			request.accept_encoding_ = line.substr(http::header::accept_encoding.size());
		}
		else if (line.substr(0, http::header::user_agent.size()) == http::header::user_agent) {
			request.user_agent_ = line.substr(http::header::user_agent.size());
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			request.content_type_ = line.substr(http::header::content_type.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			request.content_length_ = line.substr(http::header::content_length.size());
			messageToReceive = true;
		}
	}

	STREAM_LOG(logDEBUG) << sock->GetSocket() << LOG_REQUEST << requestString;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << sock->GetSocket() << LOG_REQUEST << requestString;
#endif // FILE_LOGGING



	//*************************************************************************************************************************
	//						OUTGOING RESPONSE
	//*************************************************************************************************************************
	// Construct it

	http_response response;
	proc_func_(&request, &response);

	// Fill remaining headers
	response.server_			= "Kalel Server";
	response.date_				= GMTtime(RFC_1123);
	response.connection_		= "close";
	response.content_length_	= StringFrom<size_t>(response.answer_.size());


	//
	//	Send the response
	//

	std::string responseString;

	try
	{
		responseString += sock->Send("HTTP/1.1 ");

		if (messageToReceive && request.entity_.empty()) {
			response.status_ = http::responses::bad_request;
			responseString += sock->SendLine(response.status_);
		}
		else
		{
			if (!request.auth_realm_.empty()) {
				responseString += sock->SendLine(http::responses::unauthorised);
				responseString += sock->Send(http::header::www_authenticate + "Basic Realm=\"");
				responseString += sock->Send(request.auth_realm_);
				responseString += sock->SendLine("\"");
			}
			else {

				responseString += sock->SendLine(response.status_);
			}

			responseString += sock->SendLine(http::header::date + response.date_);
			responseString += sock->SendLine(http::header::server + response.server_);
			responseString += sock->SendLine(http::header::connection + response.connection_);
			responseString += sock->SendLine(http::header::content_type + response.content_type_);
			responseString += sock->SendLine(http::header::content_length + response.content_length_);
			responseString += sock->SendLine("");
			responseString += sock->Send(response.answer_);
		}
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
	}
	
	STREAM_LOG(logDEBUG) << sock->GetSocket() << LOG_RESPONSE << responseString;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << sock->GetSocket() << LOG_RESPONSE << responseString;
#endif // FILE_LOGGING

	//
	// Exit
	//

	STREAM_LOG(logDEBUG3) << "Exit thread " << sock->GetSocket();
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG3) << "Exit thread " << sock->GetSocket();
#endif // FILE_LOGGING

	return 0;
}