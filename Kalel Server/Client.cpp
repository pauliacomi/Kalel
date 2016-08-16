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

#include "Client.h"
#include <string>


#define PORT		"32001" /* Port to connect to */


Client::Client()
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


Client::~Client()
{
#ifdef _WIN32
	WSACleanup();
#endif
}

int Client::Run()
{
	char *sendbuf = "this is a test";

	char recvbuf[512];
	int recvbuflen = 512;
	int iResult;

	SOCKET clientSocket = INVALID_SOCKET;

	// Create the address info struct
	struct addrinfo *result = NULL,
		//*ptr = NULL,
		hints;

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_INET;								// IPv4 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function

	// Resolve the local address and port to be used by the client
	if (getaddrinfo("127.0.0.1", PORT, &hints, &result) != 0) {
		std::string a("Getaddrinfo failed");
		throw a;
	}

	// Create the socket
	clientSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (clientSocket == INVALID_SOCKET) {
		std::string a("Socket failed");
		throw a;
	}

	// Connect to server.
	iResult = connect(clientSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		Close(clientSocket);
	}

	if (clientSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return 1;
	}

	// Send an initial buffer
	iResult = send(clientSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		Close(clientSocket);
		return 1;
	}

	printf("Bytes Sent: %ld\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(clientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		Close(clientSocket);
		return 1;
	}

	// Receive until the peer closes the connection
	do {

		iResult = recv(clientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
			printf("Bytes received: %d\n", iResult);
		else if (iResult == 0)
			printf("Connection closed\n");
		else
			printf("recv failed with error: %d\n", WSAGetLastError());

	} while (iResult > 0);

	// cleanup
	Close(clientSocket);

	return 0;
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
		std::string a("close socket failed");
		throw a;
	}
}