#pragma once
#include "Socket.h"

#include <vector>

class Server : Socket
{
public:
	Server();
	~Server();

	// If no port specified, server defaults to http (80)
	void Listen(PCSTR port = "http");
	void Accept();

private:
	std::vector<SOCKET*> connectedSockets;				// The list of connected sockets
	bool accepting;										// Powers the main loop
	void AcceptLoop();									// Function started by thread
	unsigned Process(SOCKET sock);
};