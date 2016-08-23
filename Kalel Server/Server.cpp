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

#include <string>
#include <exception>
#include <thread>
#include <vector>

#include "Server.h"
#include "Netcode Resources.h"


#define BACKLOG		10      /* Passed to listen() */


Server::Server()
	:serverSocket{ INVALID_SOCKET }
	, result{ NULL }
	, accepting{ false }
{
#ifdef _WIN32

	/* Initialise Winsock */
	const int iReqWinsockVer = 2;					// Minimum winsock version asked
	const int iReqWinsockRev = 2;					// Minimum winsock revison asked
	const WORD wVersion = MAKEWORD(iReqWinsockVer, iReqWinsockRev);
	WSADATA wsaData;

	if (WSAStartup(wVersion, &wsaData) != 0) {
		stringex.set(ERR_WSASTARTUP);
		throw stringex;
	}
	if (LOBYTE(wsaData.wVersion) < iReqWinsockVer) {
		stringex.set(ERR_VERSION);
		throw stringex;
	}
	
#endif
}


Server::~Server()
{
	accepting = false;

	if (serverSocket != INVALID_SOCKET) {
		Close(serverSocket);
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
	for (size_t i = 0; i < connectedSockets.size(); i++)
	{
		Close(*connectedSockets[i]);
		delete connectedSockets[i];
	}
	connectedSockets.clear();

#ifdef _WIN32
	WSACleanup();
#endif
}


void Server::Listen(PCSTR port)
{
	serverSocket = INVALID_SOCKET;

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
	struct addrinfo * i = result;
	for (i; i != NULL; i = i->ai_next)
	{
		// Create the socket
		serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (serverSocket == INVALID_SOCKET) {
			continue;
		}

		// Enable the socket to reuse the address, lose "Address already in use" error message
		int reuseaddr = 1; /* True */
		if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(int)) == SOCKET_ERROR) {
			continue;
		}
		
		// Bind to the computer IP address - setup the TCP listening socket
		if (bind(serverSocket, result->ai_addr, (int)result->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	if (i == NULL){
		stringex.set(ERR_CREATEBIND);
		throw stringex;
	}

	// clears no longer needed address info
	freeaddrinfo(result);
	result = NULL;

	// Listen to newly created socket
	if (listen(serverSocket, BACKLOG) == SOCKET_ERROR) {
		stringex.set(ERR_LISTENING);
		throw stringex;
	}
}


void Server::Accept()
{
	if (accepting == false)
	{
		accepting = true;
		std::thread(&Server::AcceptLoop, this).detach();
	}
}

void Server::AcceptLoop()
{
	while (accepting) {
		// Accept a client socket
		SOCKET clientSocket;
		struct sockaddr_storage theirAddr;					// The incoming address
		socklen_t size = sizeof theirAddr;					// Size of address struct
		memset(&theirAddr, 0, size);						// Fill addrinfo with zeroes

		clientSocket = accept(serverSocket, (struct sockaddr*)&theirAddr, &size);
		if (clientSocket == INVALID_SOCKET) {
			stringex.set(ERR_ACCEPT);
			throw stringex;
		}
		else {
			if (theirAddr.ss_family == AF_INET)
			{
				printf("Got a connection from %s on port %d\n",
					inet_ntoa(((struct sockaddr_in*)&theirAddr)->sin_addr), ntohs(((struct sockaddr_in*)&theirAddr)->sin_port));
			}
			std::thread(&Server::Process, this, serverSocket).detach();
		}
	}
}

void Server::Process(SOCKET sock)
{
	char * receivbuf[512];
	Receive(sock, *receivbuf);
	// Close connection socket
	Close(sock);
}

void Server::Send(SOCKET sock, static char * sendbuf)
{
	int bytesSent;											// Error result
	int total{ 0 };											// how many bytes we've sent
	int length{ (int)strlen(sendbuf) };							// how many bytes we have left to send

															// Send an initial buffer
	while (total < (int)strlen(sendbuf))
	{
		bytesSent = send(sock, sendbuf + total, length, 0);
		if (bytesSent == SOCKET_ERROR) {
			stringex.set(ERR_SEND);
			throw stringex;
		}
		total += bytesSent;
		length -= bytesSent;
	}
}

void Server::Receive(SOCKET sock, char * recvbuf)
{
	// Receive until the peer shuts down the connection
	int received;
	int sent;
	int length{ (int)strlen(recvbuf) };

	do {

		received = recv(sock, recvbuf, length, 0);
		if (received > 0) {

			// Echo the buffer back to the sender
			sent = send(sock, recvbuf, received, 0);
			if (sent == SOCKET_ERROR) {
				stringex.set(ERR_SEND);
				throw stringex;
			}

		}
		else if (received == 0)
			printf("Connection closing...\n");
		else {
			stringex.set(ERR_RECEIVE);
			throw stringex;
		}

	} while (received > 0);
}


void Server::Close(SOCKET sock)
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
		stringex.set(ERR_CLOSESOCKET);
		throw stringex;
	}
}


class stringexception : public std::exception
{
protected:
	std::string err;

public:
	void set(std::string err)
	{
		this->err = err;
	}

	virtual const char* what() const throw()
	{
		return err.c_str();
	}
} stringex;
