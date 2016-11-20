#include "../Forcelib.h"
#include "Socket.h"

#include "Netcode Resources.h"

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
			stringex.set(ERR_WSASTARTUP);
			throw stringex;
		}
		if (LOBYTE(wsaData.wVersion) < iReqWinsockVer) {
			stringex.set(ERR_VERSION);
			throw stringex;
		}

#endif
	}
	++nofSockets;
}

Socket::Socket()
	: sock{ INVALID_SOCKET }
	, master{ nullptr }
	, readfds{ nullptr }
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
		stringex.set(ERR_GETADDRINFO);
		throw stringex;
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

		break;	// if we got here we are connected
	}

	if (loopAddr == NULL) {
		freeaddrinfo(result);
		stringex.set(ERR_CREATEBIND);
		throw stringex;
	}

	// clears no longer needed address info
	freeaddrinfo(result);

	// Listen to newly created socket
	if (listen(sock, NO_OF_CONN) == SOCKET_ERROR) {
		stringex.set(ERR_LISTENING);
		throw stringex;
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
	int selectResult = select(sock + 1, &*readfds, NULL, NULL, &tv);

	if ((selectResult < 0) && (errno != EINTR)) {			// Check to see if select returned correctly
		stringex.set(ERR_ACCEPT);
		throw stringex;
	}

	if (FD_ISSET(sock, &*readfds))							// Check if the socket is in the set
	{
		// handle new connections

		struct sockaddr_storage theirAddr;				// The incoming address
		socklen_t size = sizeof theirAddr;				// Size of address struct
		memset(&theirAddr, 0, size);					// Fill addrinfo with zeroes

		clientSocket = accept(sock, (struct sockaddr*)&theirAddr, &size);

		if (clientSocket ==  INVALID_SOCKET) {	// Check for valid socket
			stringex.set(ERR_ACCEPT);
			throw stringex;
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
		if (connect(sock, loopAddr->ai_addr, (int)loopAddr->ai_addrlen) == SOCKET_ERROR) {
			continue;
		}

		break;	// if we got here we are connected
	}

	// clears no longer needed address info
	freeaddrinfo(result);

	if (sock == INVALID_SOCKET) {
		stringex.set(ERR_CONNECT);
		throw stringex;
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
			std::string l = std::to_string(WSAGetLastError());
			stringex.set(ERR_SEND + l);
			throw stringex;
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
	std::string ret;
	char buf[1024];
	int received;

	while(true){
		u_long arg = 0;

		if (ioctlsocket(sock, FIONREAD, &arg) != 0)
			break;
		if (arg == 0)
			break;
		if (arg > 1024)
			arg = 1024;

		received = recv(sock, buf, arg, 0);

		switch (received) {
		case 0: // not connected anymore;
			return "";
		case INVALID_SOCKET:
			if (errno == EAGAIN) {
				return ret;
			}
			else {
				stringex.set(ERR_RECEIVE);
				throw stringex;
				//// not connected anymore
				//return "";
			}
		}

		std::string t;
		t.assign(buf, received);
		ret += t;
	}
	return ret;
}

std::string Socket::ReceiveLine()
{
	std::string ret;
	int received;

	while (true) {
		char r;

		received = recv(sock, &r, 1, 0);

		switch (received) {
		case 0: // not connected anymore;
			return "";
		case INVALID_SOCKET:
			if (errno == EAGAIN) {
				return ret;
			}
			else {
				stringex.set(ERR_RECEIVE);
				throw stringex;
				//// not connected anymore
				//return "";
			}
		}

		ret += r;
		if (r == '\n')
			return ret;
	}
}

std::string Socket::ReceiveBytes(u_long bytes)
{
	std::string ret;
	char buf[1024];
	int received;
	size_t total = 0;

	bool overflow = false;

	while (true) {
		u_long arg = 0;

		if (total > bytes) {
			overflow = true;
			break;
		}
		if (ioctlsocket(sock, FIONREAD, &arg) != 0) {
			break;
		}
		if (arg == 0) {
			if (total < bytes || overflow)
			{
				received = recv(sock, buf, arg, 0);
				break;
			}
			break;
		}
		if (arg > 1024) {
			arg = 1024;
		}

		received = recv(sock, buf, arg, 0);

		switch (received) {
		case 0: // not connected anymore;
			return "";
		case INVALID_SOCKET:
			if (errno == EAGAIN) {
				return ret;
			}
			else {
				stringex.set(ERR_RECEIVE);
				throw stringex;
			}
		}

		std::string t;
		t.assign(buf, received);
		ret += t;
		total += arg;
	}
	if (total < bytes || overflow)
	{
		ret.clear();
	}
	return ret;
}

void Socket::SetLinger(bool lingerOn)
{
	struct linger lingerOption;

	if (lingerOn)
		lingerOption.l_onoff = 1;
	else 
		lingerOption.l_onoff = 0;

	if (setsockopt(sock, SOL_SOCKET, SO_LINGER, (char *)&lingerOption, sizeof(struct linger)) == -1){
		stringex.set(ERR_LINGER);
		throw stringex;
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
		std::string err = ERR_NAGLE + std::to_string(WSAGetLastError());
		stringex.set(err);
		throw stringex;
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
			stringex.set(ERR_CLOSESOCKET);
			throw stringex;
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