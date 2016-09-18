#ifndef SERVER_H
#define SERVER_H
#pragma once

#include "Socket.h"

#include <vector>
#include <atomic>
#include <thread>
#include "http_request.h"

class Server : Socket
{
public:
	Server();
	~Server();

	typedef   void(*request_func) (http_request*);

	// If no port specified, server defaults to http (80)
	void Listen(PCSTR port = "http");
	void Accept(request_func);

protected:
	struct addrinfo *result;							// Pointer to the result address
	std::vector<SOCKET*> connectedSockets;				// The list of connected sockets
	std::atomic_bool accepting;							// Powers the main loop
	void AcceptLoop();									// Function started by thread
	unsigned Process(SOCKET sock);
	std::thread acceptThread;
	std::exception_ptr teptr;

	static request_func request_func_;
};

#endif