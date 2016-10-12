#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"
#include "http_define.h"

#include <string>
#include <thread>
#include <vector>
#include <functional>

class Client
{
public:
	Client();
	~Client();

	void SetLogs(std::vector<std::string>& vct);

	// If no port specified, client defaults to http (80)	
	void Request(std::function<void(http_request*)> req, std::function<void(http_response*)> resp, std::string ip, std::string port = "http");

protected:
	unsigned Process(std::string ip, std::string port, std::function<void(http_request*)> request_func_, std::function<void(http_response*)> response_func_);
	std::thread processThread;
};

#endif