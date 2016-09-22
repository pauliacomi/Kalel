#ifndef CLIENT_H
#define CLIENT_H
#pragma once

#include "Socket.h"
#include "http_request.h"

#include <string>
#include <thread>

class Client : Socket
{
public:
	Client();
	~Client();
	
	typedef   void(*request_func) (http_request*);

	// If no port specified, client defaults to http (80)
	void Connect(request_func r, std::string ip, std::string port = "http");


protected:
	struct addrinfo *result;							// Pointer to the result address
	struct sockaddr *peer;								// Pointer to the server address

	void Start();
	std::string ip_;
	std::string port_;

	std::thread requestThread;

	static request_func request_func_;
};

#endif