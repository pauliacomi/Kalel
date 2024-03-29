#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"
#include "http_define.h"
#include "../dispatchQueue.h"

#include <string>

class HTTPClient
{
public:
	HTTPClient();
	~HTTPClient();

	void SetCredentials(const std::string & username, const std::string & password);

	// If no port specified, client defaults to http (80)	
	void Request(std::function<void(http_request*)> req, std::function<void(http_response*)> resp, std::string ip, std::string port = "http");

protected:
	std::string username = "";
	std::string password = "";

	dispatch_queue disp_q{ 10 };

	unsigned Process(std::string ip, std::string port, std::function<void(http_request*)> request_func_, std::function<void(http_response*)> response_func_);
	inline unsigned ErrorCaught(std::string err_str, std::function<void(http_response*)> response_func_);

	unsigned SendRequest(Socket & sock, http_request & request);

	unsigned ReceiveResponse(Socket & sock, http_response & resp);
};