#include "../Forcelib.h"
#include "Client.h"

#include "http_helpers.h"
#include "URLHelper.h"
#include "Netcode Resources.h"

// Logging functionality
#include "../stringHelpers.h"
#include "../log.h"	

#include <sstream>


Client::Client()
{

#define FILE_LOGGING	"client.log"		// Comment this line to disable file logging
#define LOG_LEVEL		logDEBUG4			// Change the level of logging here

#ifdef FILE_LOGGING

	FILELog::ReportingLevel() = LOG_LEVEL;
	FILE * f;
	fopen_s(&f, FILE_LOGGING, "w");
	Output2FILE::Stream() = f;

#endif // FILE_LOGGING

}


Client::~Client()
{
}


void Client::SetLogs(std::vector<std::string> & vct)
{
	StreamLog::ReportingLevel() = LOG_LEVEL;
	Output2vector::Stream() = &vct;
}


void Client::Request(std::function<void(http_request*)> req, std::function<void(http_response*)> resp, std::string ip, std::string port)
{
	auto processThread = std::make_shared<std::thread>(std::thread(&Client::Process, this, ip, port, req, resp));
	processThread->detach();
}


unsigned Client::Process(std::string ip, std::string port, std::function<void(http_request*)> request_func_, std::function<void(http_response*)> response_func_){
	
	STREAM_LOG(logDEBUG2) << "Enter thread ";
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG2) << "Enter thread";
#endif // FILE_LOGGING


	//*************************************************************************************************************************
	//						CONNECT TO SERVER
	//*************************************************************************************************************************
	
	// Create socket
	Socket l_sock;

	try{
		l_sock.Connect(ip.c_str(), port.c_str());	// Try connecting
	}
	catch (const std::exception& e)
	{
		ErrorCaught(e.what(), response_func_);
		return 1;
	}

	try {
		l_sock.SetNagle(false);						// Disable Nagle's algorithm, should lead to improved latency
	}
	catch (const std::exception& e)
	{
		ErrorCaught(e.what(), response_func_);
		return 1;
	}

	//*************************************************************************************************************************
	//						OUTGOING REQUEST
	//*************************************************************************************************************************

	http_request request;														// Create request
	request_func_(&request);													// Populate request from function

	std::string reqUrl;															// Build URL
	URLHelper::BuildReq(reqUrl, request.path, request.params);

	request.content_length = stringh::StringFrom(request.body.size());

	// Build string request and send it
	std::string requestString;

	try	{
		requestString += l_sock.SendLine(request.method + " " + reqUrl + " " + "HTTP/1.1");
		requestString += l_sock.SendLine(http::header::accept + request.accept);
		if (!request.body.empty()) {
			requestString += l_sock.SendLine(http::header::content_length + request.content_length);
			requestString += l_sock.SendLine(http::header::content_type + request.content_type);
		}
		requestString += l_sock.SendLine("");
		if (!request.body.empty()) {
			requestString += l_sock.Send(request.body);
		}
	}
	catch (const std::exception& e)
	{
		ErrorCaught(e.what(), response_func_);
		return 1;
	}

	// Log request string
	STREAM_LOG(logDEBUG) << l_sock.GetSocket() << LOG_REQUEST << requestString;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << l_sock.GetSocket() << LOG_REQUEST << requestString;
#endif // FILE_LOGGING


	//*************************************************************************************************************************
	//						INCOMING RESPONSE
	//*************************************************************************************************************************

	std::string responseString;

	try {
		responseString = l_sock.ReceiveLine();
	}
	catch (const std::exception& e)	{
		ErrorCaught(e.what(), response_func_);
		return 1;
	}

	// Check if initial line is empty
	if (responseString.empty()) {
		return 1;
	}

	http_response response;

	// Parse the first line of the response
	size_t posSpace = responseString.find_first_of(" ");

	response.http_version	= responseString.substr(0, posSpace);
	response.status			= ParseStatusCode(responseString.substr(posSpace + 1, 3));

	if (response.status.empty()){
		ErrorCaught(ERR_HTTP_CODE, response_func_);
		return 1;
	}

	std::string line;
	bool messageToReceive = false;		// not expecting a message

	while (true) {
		try	
		{
			line = l_sock.ReceiveLine();	// no message body is to be received, regular line-based receive 
		}
		catch (const std::exception& e)
		{
			ErrorCaught(e.what(), response_func_);
		}

		if (line.empty())
			break;

		responseString += line;

		size_t pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) {
			if (messageToReceive)		// a message body is specified using "Content-Length" header, will receive the required number of bytes
			{						
				u_long bytes = stringh::To<u_long>(response.content_length);
				try
				{
					line = l_sock.ReceiveBytes(bytes);
				}
				catch (const std::exception& e)
				{
					ErrorCaught(e.what(), response_func_);
				}

				if (line.empty()) {
					response.status = http::responses::bad_request;
				}
				else {
					response.body = line;
				}
				responseString += line;
			}
			break;
		}

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, http::header::server.size()) == http::header::server) {
			response.server = line.substr(http::header::server.size());
		}
		else if (line.substr(0, http::header::date.size()) == http::header::date) {
			response.date = line.substr(http::header::date.size());
		}
		else if (line.substr(0, http::header::connection.size()) == http::header::connection) {
			response.connection = line.substr(http::header::connection.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			response.content_length = line.substr(http::header::content_length.size());
			if (response.content_length != "0")
			{
				messageToReceive = true;
			}
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			response.content_type = line.substr(http::header::content_type.size());
		}
	}

	STREAM_LOG(logDEBUG) << l_sock.GetSocket() << LOG_RESPONSE << responseString;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << l_sock.GetSocket() << LOG_RESPONSE << responseString;
#endif // FILE_LOGGING

	//*************************************************************************************************************************
	//						CALLBACK AND EXIT
	//*************************************************************************************************************************

	response_func_(&response);

	/////

	//
	// Exit
	//

	STREAM_LOG(logDEBUG2) << "Exit thread";
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG2) << "Exit thread";
#endif // FILE_LOGGING

	return 0;
}


inline void Client::ErrorCaught(std::string err_str, std::function<void(http_response*)> response_func_)
{
	STREAM_LOG(logERROR) << err_str;
#ifdef FILE_LOGGING
	FILE_LOG(logERROR) << err_str;
#endif // FILE_LOGGING

	http_response response;
	response.disconnected = true;
	response.error = true;
	response.error_str = err_str;
	response_func_(&response);
}
