#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"

#include <string>
#include <thread>

class Client : Socket
{
public:
	Client();
	~Client();
	
	typedef   void(*request_func) (http_request*);
	typedef   void(*response_func) (http_response*);

	// If no port specified, client defaults to http (80)	
	void Request(request_func req, response_func resp, std::string ip, std::string port = "http");

protected:
	struct addrinfo *result;							// Pointer to the result address
	struct sockaddr *peer;								// Pointer to the server address

	unsigned Process(std::string ip, std::string port);

	std::thread processThread;

	static request_func request_func_;
	static response_func response_func_;
};

#endif