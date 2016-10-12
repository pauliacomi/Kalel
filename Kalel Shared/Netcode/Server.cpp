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

void Server::SetLogs(std::vector<std::string> & vct)
{
	StreamLog::ReportingLevel() = LOG_LEVEL;
	Output2vector::Stream() = &vct;

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


	//
	//	Get the request
	//

	std::string request;
	std::string line;

	try	{
		line = sock->ReceiveLine();
	}
	catch (const std::exception& e)	{
		STREAM_LOG(logERROR) << e.what();
	}

	request += line;

	if (line.empty()) {								// Check for valid request
		return 10;
	}

	http_request req;

	if (line.find(http::method::get) == 0) {		// Find the html method the client sent
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

	std::string path;								// Process the path and parameters of the URL
	std::map<std::string, std::string> params;

	size_t posStartPath = line.find_first_of(" ") + 1;
	URLHelper::SplitGetReq(line.substr(posStartPath), path, params);

	req.path_ = path;
	req.params_ = params;

	bool messageToReceive = false;					// not expecting a message body
	bool messageReceived = false;					// not ready to receive the message body

	while (1) {
		try
		{			
			if (!messageReceived) {					// not attempting to receive message body, regular line-based receive 
				line = sock->ReceiveLine();
			}
			else {									// a message body is specified using "Content-Length" header, will receive the required number of bytes
				std::stringstream buffer(req.content_length_);
				u_long bytes;
				buffer >> bytes;

				line = sock->ReceiveBytes(bytes);

				req.entity_ = line;					// save the message body
				request += line;					// log the message body
				break;								// leave the loop
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
			if (messageToReceive)					// if we are expecting a message body, the first empty line will signal that it is ready to be sent
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

	STREAM_LOG(logDEBUG) << sock->GetSocket() << LOG_REQUEST << request;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << sock->GetSocket() << LOG_REQUEST << request;
#endif // FILE_LOGGING

	//
	//	Construct the response
	//

	http_response resp;
	proc_func_(&req, &resp);

	// Fill remaining headers
	resp.server_			= "Kalel Server";
	resp.date_				= GMTtime(RFC_1123);
	resp.connection_		= "close";
	resp.content_length_	= StringFrom(resp.answer_.size());


	//
	//	Send the response
	//

	std::string response;

	try
	{
		response += sock->Send("HTTP/1.1 ");

		if (messageToReceive && messageReceived && req.entity_.empty()) {
			resp.status_ = http::responses::bad_request;
			response += sock->SendLine(resp.status_);
		}
		else
		{
			if (!req.auth_realm_.empty()) {
				response += sock->SendLine(http::responses::unauthorised);
				response += sock->Send(http::header::www_authenticate + "Basic Realm=\"");
				response += sock->Send(req.auth_realm_);
				response += sock->SendLine("\"");
			}
			else {

				response += sock->SendLine(resp.status_);
			}

			response += sock->SendLine(http::header::date + resp.date_);
			response += sock->SendLine(http::header::server + resp.server_);
			response += sock->SendLine(http::header::connection + resp.connection_);
			response += sock->SendLine(http::header::content_type + resp.content_type_);
			response += sock->SendLine(http::header::content_length + resp.content_length_);
			response += sock->SendLine("");
			response += sock->Send(resp.answer_);
		}
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
	}
	
	STREAM_LOG(logDEBUG) << sock->GetSocket() << LOG_RESPONSE << response;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << sock->GetSocket() << LOG_RESPONSE << response;
#endif // FILE_LOGGING

	//
	// Close connection socket
	//

	STREAM_LOG(logDEBUG3) << "Exit thread " << sock->GetSocket();
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG3) << "Exit thread " << sock->GetSocket();
#endif // FILE_LOGGING

	return 0;
}