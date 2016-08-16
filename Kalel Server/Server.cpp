#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <Ws2tcpip.h>
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>  /* Needed for getaddrinfo() and freeaddrinfo() */
#include <unistd.h> /* Needed for close() */
#define	INVALID_SOCKET -1	/* To make sure the POSIX-int handle can be compared to the WIN-uint handle  */
typedef	SOCKET int;			/* To make sure the POSIX-int handle can be compared to the WIN-uint handle  */
#endif

#include "Server.h"
#include <string>

#define PORT		"32001" /* Port to listen on */
#define BACKLOG		10      /* Passed to listen() */


Server::Server()
{
#ifdef _WIN32
	/* Initialise Winsock */
	const int iReqWinsockVer = 2;					// Minimum winsock version required
	const int iReqWinsockRev = 2;					// Minimum winsock revison required
	const WORD wVersion = MAKEWORD(iReqWinsockVer, iReqWinsockRev);
	WSADATA wsaData;

	if (WSAStartup(wVersion, &wsaData) != 0) {
		std::string a("WSAStartup failed");
		throw a;
	}
	if (LOBYTE(wsaData.wVersion) < iReqWinsockVer) {
		std::string a("Version not supported");
		throw a;
	}
	
#endif
}


Server::~Server()
{
#ifdef _WIN32
	WSACleanup();
#endif
}


void Server::Run()
{
	SOCKET serverSocket = INVALID_SOCKET;

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
	if (getaddrinfo(NULL, PORT, &hints, &result) != 0) {
		std::string a("Getaddrinfo failed");
		throw a;
	}

	// Create the socket
	serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (serverSocket == INVALID_SOCKET) {
		std::string a("Socket failed");
		throw a;
	}

	// Enable the socket to reuse the address
	int reuseaddr = 1; /* True */
	if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(int)) == SOCKET_ERROR) {
		std::string a("setsockopt failed");
		throw a;
	}

	// Bind to the address - setup the TCP listening socket
	if (bind(serverSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
		freeaddrinfo(result);
		std::string a("bind failed");
		throw a;
	}

	// clears no longer needed address info
	freeaddrinfo(result);

	// Listen to newly created socket
	if (listen(serverSocket, BACKLOG) == SOCKET_ERROR) {
		std::string a("listen failed");
		throw a;
	}

	// Accept a client socket
	SOCKET clientSocket;
	int size = sizeof(struct sockaddr);
	struct sockaddr_in theirAddr;
	memset(&theirAddr, 0, size);						// Fill addrinfo with zeroes

	clientSocket = accept(serverSocket, (struct sockaddr*)&theirAddr, &size);
	if (clientSocket == INVALID_SOCKET) {
		std::string a("accept failed");
		throw a;
	}
	else {
		printf("Got a connection from %s on port %d\n",
			inet_ntoa(theirAddr.sin_addr), ntohs(theirAddr.sin_port));
	}

	// No longer need server socket
	closesocket(serverSocket);

	// Receive until the peer shuts down the connection
	int iResult;
	int iSendResult;
	char recvbuf[512];
	int recvbuflen = 512;
	do {

		iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send(clientSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR) {
				closesocket(clientSocket);
				std::string a("send failed");
				throw a;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else {
			closesocket(clientSocket);
			std::string a("receive failed");
			throw a;
		}

	} while (iResult > 0);

	// Close connection socket
	sockClose(clientSocket);
}


void Server::sockClose(SOCKET sock)
{
	int status = 0;

#ifdef _WIN32
	status = shutdown(sock, SD_SEND);
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