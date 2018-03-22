#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"
#include "http_define.h"
#include "dispatch_queue.h"

#include <string>

class HTTPClient
{
public:
	HTTPClient();
	~HTTPClient();

	// If no port specified, client defaults to http (80)	
	void Request(std::function<void(http_request*)> req, std::function<void(http_response*)> resp, std::string ip, std::string port = "http");


protected:
	dispatch_queue disp_q{ 10 };

	unsigned Process(std::string ip, std::string port, std::function<void(http_request*)> request_func_, std::function<void(http_response*)> response_func_);
	inline unsigned ErrorCaught(std::string err_str, std::function<void(http_response*)> response_func_);

	unsigned ReceiveResponse(Socket & sock, http_response & resp);
};