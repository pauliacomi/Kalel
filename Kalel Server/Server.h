#pragma once

#include <thread>
#include <vector>

class Server
{
public:
	Server();
	~Server();

	// If no port specified, server defaults to http (80)
	void Listen(PCSTR port = "http");
	void Accept();
	void Process(SOCKET sock);
	void Send(SOCKET sock, static char * sendbuf);
	void Receive(SOCKET sock, char * receivebuf);
	void Close(SOCKET sock);

private:
	SOCKET serverSocket;								// Socket
	std::vector<SOCKET*> connectedSockets;				// The list of connected sockets
	struct addrinfo *result;							// Pointer to the result address
	bool accepting;										// Powers the main loop
	void AcceptLoop();									// Function started by thread
};