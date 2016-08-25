#pragma once
#include "Socket.h"

#include <vector>
#include <atomic>
#include <thread>

class Server : Socket
{
public:
	Server();
	~Server();

	// If no port specified, server defaults to http (80)
	void Listen(PCSTR port = "http");
	void Accept();

protected:
	struct addrinfo *result;							// Pointer to the result address
	std::vector<SOCKET*> connectedSockets;				// The list of connected sockets
	std::atomic_bool accepting;							// Powers the main loop
	void AcceptLoop();									// Function started by thread
	unsigned Process(SOCKET sock);
	std::thread acceptThread;
	std::exception_ptr teptr;
};