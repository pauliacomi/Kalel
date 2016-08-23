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

#include "Client.h"
#include "Netcode Resources.h"



Client::Client()
	: clientSocket{ INVALID_SOCKET }
	, result{NULL}
	, peer{NULL}
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


Client::~Client()
{
	if (clientSocket != INVALID_SOCKET) {
		Close(clientSocket);
	}
	if (result != NULL) {
		freeaddrinfo(result);
		result = NULL;
	}
#ifdef _WIN32
	WSACleanup();
#endif
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
		clientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (clientSocket == INVALID_SOCKET) {
			continue;
		}

		// Connect to server.
		iResult = connect(clientSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	if (clientSocket == INVALID_SOCKET) {
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

void Client::Send(static char * sendbuf)
{
	int bytesSent;											// Error result
	int total{ 0 };											// how many bytes we've sent
	int length{ (int)strlen(sendbuf) };						// how many bytes we have left to send

															// Send an initial buffer
	while (total < (int)strlen(sendbuf))
	{
		bytesSent = send(clientSocket, sendbuf + total, length, 0);
		if (bytesSent == SOCKET_ERROR) {
			stringex.set(ERR_SEND);
			throw stringex;
		}
		total += bytesSent;
		length -= bytesSent;
	}
}

void Client::Receive()
{
	int iResult;											// Error result
	char recvbuf[512];
	int recvbuflen = 512;
	// Receive until the peer closes the connection
	do {
		iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			// complete
		}
		else if (iResult == 0) {
			//printf("Connection closed\n");
		}
		else {
			stringex.set(ERR_RECEIVE);
			throw stringex;
		}

	} while (iResult > 0);
}


void Client::Close(SOCKET sock)
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