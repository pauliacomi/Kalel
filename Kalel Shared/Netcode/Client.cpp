#include "../Forcelib.h"
#include "Client.h"

#include "http_helpers.h"
#include "stdHelpers.h"
#include "URLHelper.h"
#include "Netcode Resources.h"

// Logging functionality
#include "../log.h"	
#define FILE_LOGGING	"clientlog.txt"		// Comment this line to disable file logging
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
	StreamLog::ReportingLevel() = logDEBUG4;
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

	//
	// send
	//

	http_request req;
	request_func_(&req);


	std::string reqUrl;
	URLHelper::BuildReq(reqUrl, req.path_, req.params_);

	req.content_length_ = StringFrom(req.entity_.size());

	std::string request;

	try	{
		request += l_sock.Send(req.method_ + " ");
		request += l_sock.Send(reqUrl + " ");
		request += l_sock.SendLine("HTTP/1.1");
		request += l_sock.SendLine(http::header::accept + req.accept_);
		if (!req.entity_.empty()) {
			request += l_sock.SendLine(http::header::content_length + req.content_length_);
			request += l_sock.SendLine(http::header::content_type + req.content_type_);
		}
		request += l_sock.SendLine("");
		if (!req.entity_.empty()) {
			request += l_sock.Send(req.entity_);
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

	STREAM_LOG(logDEBUG) << l_sock.GetSocket() << LOG_REQUEST << request;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << l_sock.GetSocket() << LOG_REQUEST << request;
#endif // FILE_LOGGING

	//
	// receive
	//

	std::string response;
	std::string line;

	try {
		line = l_sock.ReceiveLine();
	}
	catch (const std::exception& e)	{
		STREAM_LOG(logERROR) << e.what();
#ifdef FILE_LOGGING
		FILE_LOG(logERROR) << e.what();
#endif // FILE_LOGGING
		return 1;
	}

	response += line;

	if (line.empty()) {
		return 1;
	}

	http_response resp;

	if (line.find(http::responses::not_found) != std::string::npos) {
		resp.status_ = http::responses::not_found;
	}
	else if (line.find(http::responses::ok) != std::string::npos) {
		resp.status_ = http::responses::ok;
	}
	else if (line.find(http::responses::unauthorised) != std::string::npos) {
		resp.status_ = http::responses::unauthorised;
	}

	bool messageToReceive = true;		// expecting a message
	bool messageReceived = false;		// but not ready to receive it, first the header

	while (1) {
		try	{
			if (!messageReceived) {			// no message body is to be received, regular line-based receive 
				line = l_sock.ReceiveLine();
			}
			else {							// a message body is specified using "Content-Length" header, will receive the required number of bytes
				std::stringstream buffer(resp.content_length_);
				u_long bytes;
				buffer >> bytes;

				line = l_sock.ReceiveBytes(bytes);

				if (line.empty()) {
					resp.status_ = http::responses::bad_request;
				}
				else {
					resp.answer_ = line;
				}
				response += line;
				break;
			}
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

		response += line;

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

		if (line.substr(0, http::header::server.size()) == http::header::server) {
			resp.server_ = line.substr(http::header::server.size());
		}
		else if (line.substr(0, http::header::date.size()) == http::header::date) {
			resp.date_ = line.substr(http::header::date.size());
		}
		else if (line.substr(0, http::header::connection.size()) == http::header::connection) {
			resp.connection_ = line.substr(http::header::connection.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			resp.content_length_ = line.substr(http::header::content_length.size());
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			resp.content_type_ = line.substr(http::header::content_type.size());
		}
	}

	STREAM_LOG(logDEBUG) << l_sock.GetSocket() << LOG_RESPONSE << response;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << l_sock.GetSocket() << LOG_RESPONSE << response;
#endif // FILE_LOGGING

	/////

	response_func_(&resp);

	/////

	STREAM_LOG(logDEBUG2) << "Exit thread";
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG2) << "Exit thread";
#endif // FILE_LOGGING

	return 0;
}