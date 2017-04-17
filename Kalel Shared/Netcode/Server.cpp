#include "Server.h"

#include "http_helpers.h"
#include "URLHelper.h"
#include "stdHelpers.h"
#include "base64.h"
#include "Netcode Resources.h"

#include "../timeHelpers.h"
#include "../log.h"	



Server::Server(PCSTR port)
	: accepting{ false }
{
	// Listen on the socket
	listeningSocket.Listen(port);

	MEM_LOG(logINFO) << LOG_LISTENING << listeningSocket.GetSocket();
	FILE_LOG(logINFO) << LOG_LISTENING << listeningSocket.GetSocket();
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


void Server::AddMethod(std::function<void(http_request*, http_response*)> r, std::string url)
{
	funcMap.emplace(std::make_pair(url, r));
}



void Server::Start()
{
	if (accepting == false)
	{
		accepting = true;
		acceptThread = std::thread(&Server::AcceptLoop, this);
	}
}



unsigned Server::AcceptLoop()
{
	MEM_LOG(logDEBUG1) << LOG_ACCEPT_ENTER;
	FILE_LOG(logDEBUG1) << LOG_ACCEPT_ENTER;

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
			MEM_LOG(logERROR) << e.what();
			FILE_LOG(logERROR) << e.what();
			return 1;
		}

		MEM_LOG(logDEBUG1) << LOG_ACCEPTED_SOCK << theirIP;
		FILE_LOG(logDEBUG1) << LOG_ACCEPTED_SOCK << theirIP;

		std::unique_ptr<Socket> acceptedSocket = std::make_unique<Socket>(s);			// Create a client socket pointer from the accepted SOCKET
		acceptedSocket->SetNagle(false);												// Disable Nagle's algorithm, should lead to improved latency
		std::thread(&Server::Process, this, std::move(acceptedSocket)).detach();		// Start the request processing thread
	}

	MEM_LOG(logDEBUG1) << LOG_ACCEPT_LEAVE;
	FILE_LOG(logDEBUG1) << LOG_ACCEPT_LEAVE;

	return 0;
}


unsigned Server::Process(std::unique_ptr<Socket> sock)
{

	MEM_LOG(logDEBUG2) << LOG_PROCESS_ENTER << sock->GetSocket();
	FILE_LOG(logDEBUG2) << LOG_PROCESS_ENTER << sock->GetSocket();


	//*************************************************************************************************************************
	//						INCOMING REQUEST
	//*************************************************************************************************************************
	//	Get the request

	std::string requestString;

	try	{													// Get the first line
		requestString = sock->ReceiveLine();
	}
	catch (const std::exception& e)	{
		MEM_LOG(logERROR) << e.what();
		FILE_LOG(logERROR) << e.what();
	}

	if (requestString.empty()) {							// Check for valid request
		return 1;
	}

	http_request request;									// Create request class


															// Find the html method the client sent
	size_t posSpace = requestString.find_first_of(" ");

	request.method_ = ParseMethod(requestString.substr(0, posSpace));
	if(request.method_.empty()){
		MEM_LOG(logWARNING) << LOG_METHOD_UNKNOWN << sock->GetSocket();
		FILE_LOG(logWARNING) << LOG_METHOD_UNKNOWN << sock->GetSocket();
		return 1;
	}

															// Process the path and parameters of the URL
	URLHelper::SplitGetReq(requestString.substr(posSpace + 1), request.path_, request.params_);



	std::string line;										// Start receiving the rest of the request
	bool messageToReceive = false;							// not expecting a message body

	while (true) {
		try
		{			
			line = sock->ReceiveLine();						// not attempting to receive message body, regular line-based receive 
		}
		catch (const std::exception& e)
		{
			MEM_LOG(logERROR) << e.what();
			FILE_LOG(logERROR) << e.what();
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
					MEM_LOG(logERROR) << e.what();
					FILE_LOG(logERROR) << e.what();
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
			if (request.content_length_ == "0")
			{
				continue;
			}
			messageToReceive = true;
		}
	}

	MEM_LOG(logDEBUG3) << sock->GetSocket() << LOG_REQUEST << requestString;
	FILE_LOG(logDEBUG3) << sock->GetSocket() << LOG_REQUEST << requestString;



	//*************************************************************************************************************************
	//						OUTGOING RESPONSE
	//*************************************************************************************************************************
	// Construct it

	http_response response;
	try {
		funcMap[request.path_](&request, &response);
	}
	catch (const std::exception& e){

		response.status_ = http::responses::bad_request;

		MEM_LOG(logERROR) << e.what();
		FILE_LOG(logERROR) << e.what();
	}

	// Fill remaining headers
	response.server_			= "Kalel Server";
	response.date_				= timeh::GMTtime(RFC_1123);
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
		MEM_LOG(logERROR) << e.what();
		FILE_LOG(logERROR) << e.what();
	}
	
	MEM_LOG(logDEBUG3) << sock->GetSocket() << LOG_RESPONSE << responseString;
	FILE_LOG(logDEBUG3) << sock->GetSocket() << LOG_RESPONSE << responseString;

	//
	// Exit
	//

	// Make sure all data will get sent before socket is closed
	// sock->SetLinger(true);

	MEM_LOG(logDEBUG2) << LOG_PROCESS_EXIT << sock->GetSocket();
	FILE_LOG(logDEBUG2) << LOG_PROCESS_EXIT << sock->GetSocket();

	return 0;
}