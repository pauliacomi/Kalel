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



void Socket::Send(SOCKET sock, const std::string& sendbuf)
{
	int bytesSent;											// Error result
	int total{ 0 };											// how many bytes we've sent
	int length{ (int)sendbuf.length() };					// how many bytes we have left to send

															// Send an initial buffer
	while (total < (int)sendbuf.length())
	{
		bytesSent = send(sock, sendbuf.c_str() + total, length, 0);
		if (bytesSent == SOCKET_ERROR) {
			stringex.set(ERR_SEND);
			throw stringex;
		}
		total += bytesSent;
		length -= bytesSent;
	}
}

void Socket::SendLine(SOCKET sock, const std::string& sendbuf)
{
	std::string local = sendbuf;
	local += '\n';
	Send(sock, local);
}

std::string Socket::Receive(SOCKET sock)
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

std::string Socket::ReceiveLine(SOCKET sock)
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

void Socket::Close(SOCKET sock)
{
	if (sock != INVALID_SOCKET) {
		
		int status = closesocket(sock);
		sock = INVALID_SOCKET;

		if (status == SOCKET_ERROR)
		{
			stringex.set(ERR_CLOSESOCKET);
			throw stringex;
		}

	}
}

void Socket::CloseGracefully(SOCKET sock)
{
	if (sock != INVALID_SOCKET) {
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
}