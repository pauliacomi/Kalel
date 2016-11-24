#include "../Forcelib.h"
#include "Client.h"

#include "http_helpers.h"
#include "stdHelpers.h"
#include "URLHelper.h"
#include "Netcode Resources.h"

// Logging functionality
#include "../log.h"	
#define FILE_LOGGING	"client.log"		// Comment this line to disable file logging
#define LOG_LEVEL		logDEBUG4			// Change the level of logging here

#include <sstream>


Client::Client()
{

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
	processThread = std::thread(&Client::Process, this, ip, port, req, resp);
	processThread.detach();
}


unsigned Client::Process(std::string ip, std::string port, std::function<void(http_request*)> request_func_, std::function<void(http_response*)> response_func_){
	
	STREAM_LOG(logDEBUG2) << "Enter thread ";
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG2) << "Enter thread";
#endif // FILE_LOGGING


	//*************************************************************************************************************************
	//						CONNECT TO SERVER
	//*************************************************************************************************************************
	// create socket and connect

	Socket l_sock;

	try{
		l_sock.Connect(ip.c_str(), port.c_str());
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
		FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
		return 1;
	}

	try {
		l_sock.SetNagle(false);		// Disable Nagle's algorithm, should lead to improved latency
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
		FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
		return 1;
	}

	//*************************************************************************************************************************
	//						OUTGOING REQUEST
	//*************************************************************************************************************************

	http_request request;
	request_func_(&request);


	std::string reqUrl;
	URLHelper::BuildReq(reqUrl, request.path_, request.params_);

	request.content_length_ = StringFrom(request.entity_.size());

	std::string requestString;

	try	{
		requestString += l_sock.SendLine(request.method_ + " " + reqUrl + " " + "HTTP/1.1");
		requestString += l_sock.SendLine(http::header::accept + request.accept_);
		if (!request.entity_.empty()) {
			requestString += l_sock.SendLine(http::header::content_length + request.content_length_);
			requestString += l_sock.SendLine(http::header::content_type + request.content_type_);
		}
		requestString += l_sock.SendLine("");
		if (!request.entity_.empty()) {
			requestString += l_sock.Send(request.entity_);
		}
	}
	catch (const std::exception& e)
	{
		STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
		FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
		return 1;
	}

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
		STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
		FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
		return 1;
	}

	if (responseString.empty()) {
		return 1;
	}

	http_response response;

	size_t posSpace = responseString.find_first_of(" ");

	response.http_version_	= responseString.substr(0, posSpace);
	response.status_		= ParseStatusCode(responseString.substr(posSpace + 1, 3));

	if (response.status_.empty()){
		STREAM_LOG(logERROR) << ERR_HTTP_CODE;
#ifdef FILE_LOGGING
		FILE_LOG(logERROR) << ERR_HTTP_CODE;
#endif // FILE_LOGGING
		return 1;
	}

	std::string line;
	bool messageToReceive = true;		// expecting a message

	while (1) {
		try	
		{
			line = l_sock.ReceiveLine();	// no message body is to be received, regular line-based receive 
		}
		catch (const std::exception& e)
		{
			STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
			FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
		}

		if (line.empty())
			break;

		responseString += line;

		unsigned int pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) {
			if (messageToReceive)		// a message body is specified using "Content-Length" header, will receive the required number of bytes
			{						
				u_long bytes = To<u_long>(response.content_length_);
				try
				{
					line = l_sock.ReceiveBytes(bytes);
				}
				catch (const std::exception& e)
				{
					STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
					FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
				}

				if (line.empty()) {
					response.status_ = http::responses::bad_request;
				}
				else {
					response.answer_ = line;
				}
				responseString += line;
			}
			break;
		}

		line = line.substr(0, pos_cr_lf);

		if (line.substr(0, http::header::server.size()) == http::header::server) {
			response.server_ = line.substr(http::header::server.size());
		}
		else if (line.substr(0, http::header::date.size()) == http::header::date) {
			response.date_ = line.substr(http::header::date.size());
		}
		else if (line.substr(0, http::header::connection.size()) == http::header::connection) {
			response.connection_ = line.substr(http::header::connection.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			response.content_length_ = line.substr(http::header::content_length.size());
			if (response.content_length_ == "0")
			{
				messageToReceive = false;
			}
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			response.content_type_ = line.substr(http::header::content_type.size());
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