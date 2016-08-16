#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif

#include <winsock2.h>
#include <Ws2tcpip.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#endif


#include "Socket.h"
#include <string>

Socket::Socket()
	:sock{INVALID_SOCKET}
{
}


Socket::~Socket()
{
}


void Socket::Open(PCSTR ip, PCSTR port)
{
	// Create the address info struct
	struct addrinfo *result = NULL,
					*ptr = NULL,
					hints;

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_INET;								// IPv4 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function

															// Resolve the local address and port to be used by the server
	if (getaddrinfo(ip, port, &hints, &result) != 0) {
		std::string a("Getaddrinfo failed");
		throw a;
	}

	// Create the socket
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (sock == INVALID_SOCKET) {
		std::string a("Socket failed");
		throw a;
	}

	// Enable the socket to reuse the address
	int reuseaddr = 1; /* True */
	if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(int)) == SOCKET_ERROR) {
		std::string a("setsockopt failed");
		throw a;
	}

	// Bind to the address - setup the TCP listening socket
	if (bind(sock, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		freeaddrinfo(result);
		std::string a("bind failed");
		throw a;
	}

	// clears no longer needed address info
	freeaddrinfo(result);
}

void Socket::Close()
{
	int status = 0;

#ifdef _WIN32
	status = shutdown(sock, SD_BOTH);
	if (status == 0) { 
		status = closesocket(sock); 
		sock = INVALID_SOCKET;
	}
#else
	status = shutdown(sock, SHUT_RDWR);
	if (status == 0) { 
		status = close(sock);
		sock = INVALID_SOCKET;
	}
#endif

	if (status == SOCKET_ERROR)
	{
		std::string a("close socket failed");
		throw a;
	}
}

SOCKET Socket::Get()
{
	return sock;
}