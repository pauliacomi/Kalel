#pragma once

class Client
{
public:
	Client();
	~Client();

	// If no port specified, client defaults to http (80)
	void Connect(PCSTR ip, PCSTR port = "http");
	void Send(static char * sendbuf);
	void Receive();
	void Close(SOCKET sock);

protected:
	SOCKET clientSocket;								// Socket
	struct addrinfo *result;							// Pointer to the result address
	struct sockaddr *peer;								// Pointer to the server address
};

