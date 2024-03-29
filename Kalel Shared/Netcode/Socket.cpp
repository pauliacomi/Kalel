#include "../Forcelib.h"
#include "Socket.h"

#include "Netcode Resources.h"

#include <exception>
#include <sstream>

#define NO_OF_CONN		5      /* Passed to listen() */

int Socket::nofSockets = 0;

void Socket::Start() 
{
	if (!nofSockets) {

#ifdef _WIN32

		/* Initialise Winsock */
		const int iReqWinsockVer = 2;					// Minimum winsock version asked
		const int iReqWinsockRev = 2;					// Minimum winsock revison asked
		const WORD wVersion = MAKEWORD(iReqWinsockVer, iReqWinsockRev);
		WSADATA wsaData;

		if (WSAStartup(wVersion, &wsaData) != 0) {
			throw std::runtime_error(ERR_WSASTARTUP);
		}
		if (LOBYTE(wsaData.wVersion) < iReqWinsockVer) {
			throw std::runtime_error(ERR_VERSION);
		}

#endif
	}
	++nofSockets;
}

Socket::Socket()
	: sock{ INVALID_SOCKET }
{
	Start();
}

Socket::Socket(SOCKET sk)
	: sock{ sk }
{
	Start();
}

Socket::~Socket()
{
	CloseGracefully();

	if (master)
	{
		delete master;
		delete readfds;
	}

#ifdef _WIN32
	--nofSockets;
	if (!nofSockets)
		WSACleanup();
#endif
}

SOCKET Socket::GetSocket()
{
	return sock;
}


void Socket::Listen(PCSTR port)
{
	// Create the address info struct
	struct addrinfo hints;									// The requested address
	struct addrinfo *result;								// Pointer to the result address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function


															// Resolve the local address and port to be used by the server
	if (getaddrinfo(NULL, port, &hints, &result) != 0) {
		throw std::runtime_error(ERR_GETADDRINFO);
	}

	// Loop through all the results and bind to the first we can
	struct addrinfo * loopAddr = result;
	for (loopAddr; loopAddr != NULL; loopAddr = loopAddr->ai_next)
	{
		// Create the socket
		sock = socket(loopAddr->ai_family, loopAddr->ai_socktype, loopAddr->ai_protocol);
		if (sock == INVALID_SOCKET) {
			continue;
		}

		// Enable the socket to reuse the address, lose "Address already in use" error message
		int reuseaddr = 1; /* True */
		if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, (const char *)&reuseaddr, sizeof(int)) == SOCKET_ERROR) {
			continue;
		}

		// Bind to the computer IP address - setup the TCP listening socket
		if (bind(sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		// Set socket option to timeout
		int timeout = 5000;																// 5 s
		setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));

		break;	// if we got here we are connected
	}

	if (loopAddr == NULL) {
		freeaddrinfo(result);
		throw std::runtime_error(ERR_CREATEBIND);
	}

	// clears no longer needed address info
	freeaddrinfo(result);

	// Listen to newly created socket
	if (listen(sock, NO_OF_CONN) == SOCKET_ERROR) {
		throw std::runtime_error(ERR_LISTENING);
	}


}

void Socket::Accept_PrimeSelect()
{
	if (!master)
	{
		master = new fd_set;
		readfds = new fd_set;
		FD_ZERO(&*master);				// Zero master out
		FD_ZERO(&*readfds);				// Zero read out
		FD_SET(sock, &*master);			// Add only main socket
	}
}


std::string Socket::Accept(SOCKET &clientSocket, timeval tv)
{
	*readfds = *master;
	SOCKET selectResult = select(sock + 1, &*readfds, NULL, NULL, &tv);

	if ((selectResult < 0) && (errno != EINTR)) {			// Check to see if select returned correctly
		throw std::runtime_error(ERR_ACCEPT);
	}

	if (FD_ISSET(sock, &*readfds))							// Check if the socket is in the set
	{
		// handle new connections

		struct sockaddr_storage theirAddr;				// The incoming address
		socklen_t size = sizeof theirAddr;				// Size of address struct
		memset(&theirAddr, 0, size);					// Fill addrinfo with zeroes

		clientSocket = accept(sock, (struct sockaddr*)&theirAddr, &size);

		if (clientSocket ==  INVALID_SOCKET) {	// Check for valid socket
			throw std::runtime_error(ERR_ACCEPT);
		}
		else {											// If valid, return the new socket
			return GetIP(theirAddr);
		}
	}
	
	return std::string();
}

unsigned Socket::Connect(PCSTR ip, PCSTR port)
{
	// Create the address info struct
	struct addrinfo hints;									// The requested address
	struct addrinfo *result;								// Pointer to the result address

	memset(&hints, 0, sizeof hints);						// Fill addrinfo with zeroes
	hints.ai_family = AF_UNSPEC;							// IPv4/IPv6 family
	hints.ai_socktype = SOCK_STREAM;						// Stream socket
	hints.ai_protocol = IPPROTO_TCP;						// Using TCP protocol
	hints.ai_flags = AI_PASSIVE;							// Caller intends to use the returned socket address structure in a call to the bind function


	// Resolve the local address and port to be used by the server
	if (getaddrinfo(ip, port, &hints, &result) != 0) {
		throw std::runtime_error(ERR_GETADDRINFO);
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
		if (connect(sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	// clears no longer needed address info
	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		throw std::runtime_error(ERR_CONNECT);
	}

	return 0;
}


std::string Socket::Send(const std::string& sendbuf)
{
	int bytesSent;											// Error result
	int total{ 0 };											// how many bytes we've sent
	int length{ (int)sendbuf.length() };					// how many bytes we have left to send

															// Send an initial buffer
	while (total < (int)sendbuf.length())
	{
		bytesSent = send(sock, sendbuf.c_str() + total, length, 0);
		if (bytesSent == SOCKET_ERROR) {
			throw std::runtime_error(ERR_RECEIVE + std::to_string(WSAGetLastError()));
		}
		total += bytesSent;
		length -= bytesSent;
		if (total < (int)sendbuf.length())
		{
			int a = 0;
			a++;
		}
	}

	return sendbuf;
}

std::string Socket::SendLine(const std::string& sendbuf)
{
	std::string local = sendbuf;
	local += "\x0D\x0A";
	return Send(local);
}

std::string Socket::Receive()
{
	std::stringstream ret;
	const unsigned int bufsize = 1024;
	char buf[bufsize];
	int received;

	while(true){
		u_long arg = 0;

		if (ioctlsocket(sock, FIONREAD, &arg) != 0)
			break;
		if (arg == 0)
			break;
		if (arg > bufsize)
			arg = bufsize;

		received = recv(sock, buf, arg, 0);

		switch (received) {
		case 0: // not connected anymore;
			return "";
		case INVALID_SOCKET:
			if (errno == EAGAIN) {
				return ret.str();
			}
			else {
				throw std::runtime_error(ERR_RECEIVE + std::to_string(WSAGetLastError()));
			}
			break;
		default:
			ret.write(buf, received);
			break;
		}
	}

	return ret.str();
}

std::string Socket::ReceiveLine()
{
	std::stringstream ret;
	int received;

	while (true) {
		char r;
		
		received = recv(sock, &r, 1, 0);

		switch (received) {
		case 0: // not connected anymore;
			return "";
		case SOCKET_ERROR:
			if (errno == EAGAIN) {
				return ret.str();
			}
			else {
				throw std::runtime_error(ERR_RECEIVE + std::to_string(WSAGetLastError()));
			}
			break;
		default:
			ret << r;
			if (r == '\n')
				return ret.str();
			break;
		}

	}
}

std::string Socket::ReceiveBytes(u_long bytes)
{
	std::stringstream ret;
	const unsigned int bufsize = 1024;
	char buf[bufsize];
	int received;
	size_t total = 0;

	bool overflow = false;

	while (true) {

		if (total >= bytes) {
			if (total == bytes){
				break;
			}
			overflow = true;
			break;
		}

		received = recv(sock, buf, sizeof(buf), 0);

		switch (received) {
		case 0:					// not connected anymore;
			break;
		case INVALID_SOCKET:	// error
			if (errno == EAGAIN) {
				return ret.str();
			}
			else {
				throw std::runtime_error(ERR_RECEIVE + std::to_string(WSAGetLastError()));
			}
			break;
		default:
			total += received;
			ret.write(buf, received);
			break;
		}
	}

	if (total < bytes || overflow)
	{
		ret.clear();
	}
	return ret.str();
}

void Socket::SetLinger(bool lingerOn)
{
	struct linger lingerOption;

	if (lingerOn)
		lingerOption.l_onoff = 1;
	else 
		lingerOption.l_onoff = 0;

	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&lingerOption, sizeof(struct linger)) == -1){
		throw std::runtime_error(ERR_LINGER);
	}
}

void Socket::SetNagle(bool nagleOn)
{
	int nagleOption;

	if (nagleOn)
		nagleOption = 0;
	else
		nagleOption = 1;

	if (setsockopt(sock, IPPROTO_TCP, TCP_NODELAY, (char *)&nagleOption, sizeof(int)) == -1) {
		throw std::runtime_error(ERR_NAGLE + std::to_string(WSAGetLastError()));
	}
}


void Socket::Close()
{
	if (sock != INVALID_SOCKET)
	{
		int status = 0;

#ifdef _WIN32
		status = closesocket(sock);
#else
		status = close(sock);
#endif
		if (status == SOCKET_ERROR)
		{
			throw std::runtime_error(ERR_CLOSESOCKET);
		}

		sock = INVALID_SOCKET;
	}
}

void Socket::CloseGracefully()
{
	if (sock != INVALID_SOCKET) {
		//shutdown(sock, SD_SEND);
		Close();
	}
}


std::string Socket::GetIP(const sockaddr_storage &address)
{
	std::string humanIP;
	char name[256] = { 0 };
	int addrSize;

#ifdef _WIN32

	DWORD size;
	switch (address.ss_family)
	{
	case AF_INET:
		size = INET_ADDRSTRLEN;
		addrSize = sizeof(struct sockaddr_in);
		break;
	case AF_INET6:
		size = INET6_ADDRSTRLEN;
		addrSize = sizeof(struct sockaddr_in6);
		break;
	default:
		return NULL;
		break;
	}

	int rtval = WSAAddressToStringA((LPSOCKADDR)&address, addrSize, NULL, (LPSTR)name, &size);

	if (rtval)
	{
		humanIP = ERR_IP_CONVERT + std::to_string(WSAGetLastError());
	}
	else
	{
		humanIP = name;
	}

#else

	switch (address.ss_family)
	{
	case AF_INET:
		addrSize = sizeof(struct sockaddr_storage);
		inet_ntop(AF_INET, &(((struct sockaddr_in *)sa)->sin_addr), name, INET_ADDRSTRLEN);
		break;
	case AF_INET6:
		addrSize = sizeof(struct sockaddr_storage);
		inet_ntop(AF_INET6, &(((struct sockaddr_in6 *)sa)->sin6_addr), name, INET6_ADDRSTRLEN);
		break;
	default:
		return NULL;
		break;
	}

	humanIP = name;

#endif

	return humanIP;
}