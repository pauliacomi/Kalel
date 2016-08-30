#include "Forcelib.h"
#include "Socket.h"

#include "Netcode Resources.h"

int Socket::nofSockets = 0;

void Socket::Start() {
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
	Close(sock);

#ifdef _WIN32
	--nofSockets;
	if (!nofSockets)
		WSACleanup();
#endif
}



void Socket::Send(SOCKET l_sock, const std::string& sendbuf)
{
	int bytesSent;											// Error result
	int total{ 0 };											// how many bytes we've sent
	int length{ (int)sendbuf.length() };					// how many bytes we have left to send

															// Send an initial buffer
	while (total < (int)sendbuf.length())
	{
		bytesSent = send(l_sock, sendbuf.c_str() + total, length, 0);
		if (bytesSent == SOCKET_ERROR) {
			stringex.set(ERR_SEND);
			throw stringex;
		}
		total += bytesSent;
		length -= bytesSent;
	}
}

void Socket::SendLine(SOCKET l_sock, const std::string& sendbuf)
{
	std::string local = sendbuf;
	local += '\n';
	Send(l_sock, local);
}

std::string Socket::Receive(SOCKET l_sock)
{
	std::string ret;
	char buf[1024];
	int received;

	while(true){
		u_long arg = 0;

		if (ioctlsocket(l_sock, FIONREAD, &arg) != 0)
			break;
		if (arg == 0)
			break;
		if (arg > 1024)
			arg = 1024;

		received = recv(l_sock, buf, arg, 0);

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

std::string Socket::ReceiveLine(SOCKET l_sock)
{
	std::string ret;
	int received;

	while (true) {
		char r;

		received = recv(l_sock, &r, 1, 0);

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

void Socket::Close(SOCKET l_sock)
{
	if (l_sock != INVALID_SOCKET) {
		
		int status = closesocket(l_sock);
		l_sock = INVALID_SOCKET;

		if (status == SOCKET_ERROR)
		{
			stringex.set(ERR_CLOSESOCKET);
			throw stringex;
		}

	}
}

void Socket::CloseGracefully(SOCKET l_sock)
{
	if (l_sock != INVALID_SOCKET) {
		int status = 0;

#ifdef _WIN32
		status = shutdown(l_sock, SD_SEND);
		if (status == 0) {
			status = closesocket(l_sock);
			l_sock = INVALID_SOCKET;
		}
#else
		status = shutdown(l_sock, SHUT_RDWR);
		if (status == 0) {
			status = close(l_sock);
			l_sock = INVALID_SOCKET;
		}
#endif

		if (status == SOCKET_ERROR)
		{
			stringex.set(ERR_CLOSESOCKET);
			throw stringex;
		}

	}
}