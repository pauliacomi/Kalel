#include "Client.h"
#include "Netcode Resources.h"


Client::Client()
	: peer{NULL}
{
}


Client::~Client()
{
}

void Client::Connect(PCSTR ip, PCSTR port)
{
	int iResult;											// Error result

	// Create the address info struct
	struct addrinfo hints;									// The requested address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function


	// Resolve the local address and port to be used by the server
	struct addrinfo *result = NULL;							// Pointer to the result address
	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		stringex.set(ERR_GETADDRINFO);
		throw stringex;
	}
	// Loop through all the results and bind to the first we can
	for (struct addrinfo * i = result; i != NULL; i = i->ai_next)
	{
		// Create the socket
		sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (sock == INVALID_SOCKET) {
			continue;
		}

		// Connect to server.
		iResult = connect(sock, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
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
	result = NULL;
}