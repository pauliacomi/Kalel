#include "../Forcelib.h"
#include "http_client.h"

#include "http_helpers.h"
#include "URLHelper.h"
#include "Netcode Resources.h"

// Logging functionality
#include "../stringHelpers.h"
#include "../log.h"	

#include <sstream>
#include <algorithm>


HTTPClient::HTTPClient()
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


HTTPClient::~HTTPClient()
{
}


void HTTPClient::SetLogs(std::vector<std::string> & vct)
{
	StreamLog::ReportingLevel() = LOG_LEVEL;
	Output2vector::Stream() = &vct;
}


void HTTPClient::Request(std::function<void(http_request*)> func_req, std::function<void(http_response*)> func_resp, std::string ip, std::string port)
{
	std::lock_guard<std::mutex> lock(threadMutex);
	threadPool.emplace_back(&HTTPClient::Process, this, ip, port, func_req, func_resp);
}

void HTTPClient::removeThread(std::thread::id id)
{
	std::lock_guard<std::mutex> lock(threadMutex);
	auto iter = std::find_if(threadPool.begin(), threadPool.end(), [=](std::thread &t) { return (t.get_id() == id); });
	if (iter != threadPool.end())
	{
		iter->detach();
		threadPool.erase(iter);
	}
}

unsigned HTTPClient::Process(std::string ip, std::string port, std::function<void(http_request*)> func_req, std::function<void(http_response*)> func_resp){
	
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
		return ErrorCaught(e.what(), func_resp);
	}

	try {
		l_sock.SetNagle(false);						// Disable Nagle's algorithm, should lead to improved latency
	}
	catch (const std::exception& e)
	{
		return ErrorCaught(e.what(), func_resp);
	}

	//*************************************************************************************************************************
	//						OUTGOING REQUEST
	//*************************************************************************************************************************

	http_request request;														// Create request
	func_req(&request);															// Populate request from function

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
		return ErrorCaught(e.what(), func_resp);
	}

	// Log request string
	STREAM_LOG(logDEBUG) << l_sock.GetSocket() << LOG_REQUEST << requestString;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << l_sock.GetSocket() << LOG_REQUEST << requestString;
#endif // FILE_LOGGING


	//*************************************************************************************************************************
	//						INCOMING RESPONSE
	//*************************************************************************************************************************

	http_response response;

	if (ReceiveResponse(l_sock, response) != 0)
	{
		return ErrorCaught(response.error_str, func_resp);
	}
	else
	{
		func_resp(&response);
	}

	//
	// Exit
	//

	STREAM_LOG(logDEBUG2) << "Exit thread";
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG2) << "Exit thread";
#endif // FILE_LOGGING

	std::thread(&HTTPClient::removeThread, this, std::this_thread::get_id()).detach();
	return 0;
}


inline unsigned HTTPClient::ErrorCaught(std::string err_str, std::function<void(http_response*)> func_resp)
{
	STREAM_LOG(logERROR) << err_str;
#ifdef FILE_LOGGING
	FILE_LOG(logERROR) << err_str;
#endif // FILE_LOGGING

	http_response response;
	response.disconnected = true;
	response.error = true;
	response.error_str = err_str;
	func_resp(&response);

	std::thread(&HTTPClient::removeThread, this, std::this_thread::get_id()).detach();

	return 1;
}


unsigned HTTPClient::ReceiveResponse(Socket & sock, http_response & resp)
{

	std::string responseString;

	try {
		responseString = sock.ReceiveLine();
	}
	catch (const std::exception& e) {
		resp.error_str = e.what();
		return 1;
	}

	// Check if initial line is empty
	if (responseString.empty()) {
		return 1;
	}


	// Parse the first line of the resp
	size_t posSpace = responseString.find_first_of(" ");

	resp.http_version = responseString.substr(0, posSpace);
	resp.status = ParseStatusCode(responseString.substr(posSpace + 1, 3));

	if (resp.status.empty()) {
		resp.error_str = ERR_HTTP_CODE;
		return 1;
	}

	//*************** Now process headers

	std::string line;										// Start receiving the rest of the request
	bool messageToReceive = false;							// not expecting a message

	while (true) {
		try
		{
			line = sock.ReceiveLine();	// no message body is to be received, regular line-based receive 
		}
		catch (const std::exception& e)
		{
			resp.error_str = e.what();
			return 1;
		}

		if (line.empty()) {									// if line is empty, there's an error
			return 1;
		}

		responseString += line;

		// Check for CRLF position
		size_t pos_cr_lf = line.find_first_of("\x0a\x0d");
		if (pos_cr_lf == 0) {								// If we find an empty line, it's the end of the headers
			break;
		}

		line = line.substr(0, pos_cr_lf);					// Otherwise we remove the CLRF and see what kind of header it is

		if (line.substr(0, http::header::server.size()) == http::header::server) {
			resp.server = line.substr(http::header::server.size());
		}
		else if (line.substr(0, http::header::date.size()) == http::header::date) {
			resp.date = line.substr(http::header::date.size());
		}
		else if (line.substr(0, http::header::connection.size()) == http::header::connection) {
			resp.connection = line.substr(http::header::connection.size());
		}
		else if (line.substr(0, http::header::content_type.size()) == http::header::content_type) {
			resp.content_type = line.substr(http::header::content_type.size());
		}
		else if (line.substr(0, http::header::content_length.size()) == http::header::content_length) {
			resp.content_length = line.substr(http::header::content_length.size());
			if (resp.content_length != "0") { messageToReceive = true; }
		}
	}

	//*************** Receive response body (if applicable)

	if (messageToReceive)												// If we are expecting a message body then 
	{
		u_long bytes = stringh::To<u_long>(resp.content_length);		// We will receive the amount of bytes specified by the header

		try
		{
			line = sock.ReceiveBytes(bytes);
		}
		catch (const std::exception& e)
		{
			resp.error_str = e.what();
			return 1;
		}

		if (line.empty()) {
			resp.status = http::responses::bad_request;
		}
		else {
			resp.body = line;
		}
		responseString += line;
	}

	//*************** Response is now received

	STREAM_LOG(logDEBUG) << sock.GetSocket() << LOG_RESPONSE << responseString;
#ifdef FILE_LOGGING
	FILE_LOG(logDEBUG) << sock.GetSocket() << LOG_RESPONSE << responseString;
#endif // FILE_LOGGING

	return 0;
}
