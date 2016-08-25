#pragma once
#include "Socket.h"

class Client : Socket
{
public:
	Client();
	~Client();

	// If no port specified, client defaults to http (80)
	void Connect(PCSTR ip, PCSTR port = "http");

protected:
	struct addrinfo *result;							// Pointer to the result address
	struct sockaddr *peer;								// Pointer to the server address

};

