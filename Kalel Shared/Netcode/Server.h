#ifndef SERVER_H
#define SERVER_H
#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"
#include "http_define.h"

#include <vector>
#include <atomic>
#include <thread>
#include <functional>

class Server : Socket
{
public:
	Server();
	~Server();

	void SetLogs(std::vector<std::string>& vct);

	// If no port specified, server defaults to http (80)
	void Listen(PCSTR port = "http");
	void Accept(std::function<void(http_request*, http_response*)> r);

protected:
	struct addrinfo *result;							// Pointer to the result address
	std::vector<SOCKET> socketCollection;				// The list of socket threads
	std::atomic_bool accepting;							// Powers the main loop
	void AcceptLoop();									// Function started as a thread to listen to incoming connections
	unsigned Process(SOCKET sock);						// Function started when a socket connects
	std::thread acceptThread;
	std::exception_ptr teptr;

	std::function<void(http_request*, http_response*)> proc_func_;
};

#endif