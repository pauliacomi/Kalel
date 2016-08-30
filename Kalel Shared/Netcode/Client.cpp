#include "Forcelib.h"
#include "Client.h"

#include "Netcode Resources.h"


Client::Client()
	: peer{ nullptr }
	, result{ nullptr }
{
}


Client::~Client()
{
	// Client sockets close gracefully
	CloseGracefully(sock);

	// Free result structure in case we throw
	if (result != nullptr){
		freeaddrinfo(result);
	}
}

void Client::Connect(PCSTR ip, PCSTR port)
{
	// Create the address info struct
	struct addrinfo hints;									// The requested address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function


	// Resolve the local address and port to be used by the server
	if (getaddrinfo(ip, port, &hints, &result) != 0) {
		stringex.set(ERR_GETADDRINFO);
		throw stringex;
	}

	// Loop through all the results and bind to the first we can
	for (struct addrinfo * loopAddr = result; loopAddr != NULL; loopAddr = loopAddr->ai_next)
	{
		// Create the socket
		sock = socket(loopAddr->ai_family, loopAddr->ai_socktype, loopAddr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			continue;
		}

		// Connect to server.
		if(connect(sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR){
			continue;
		}

		break;	// if we got here we are connected
	}

	if (sock == INVALID_SOCKET) {
		stringex.set(ERR_CONNECT);
		throw stringex;
	}

	/*iResult = getpeername(clientSocket, peer, sizeof(struct sockaddr));
	if (iResult == SOCKET_ERROR) {
		std::string a("Cannot get identity!");
		throw a;
	}*/

	// clears no longer needed address info
	freeaddrinfo(result);
	result = nullptr;
}