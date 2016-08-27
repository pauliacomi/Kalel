#pragma once

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
};


class Socket
{
public:
	Socket();
	Socket(SOCKET sk);
	~Socket();

	void Send(SOCKET sock, const std::string& sendbuf);
	void SendLine(SOCKET sock, const std::string& sendbuf);
	std::string Socket::Receive(SOCKET sock);
	std::string Socket::ReceiveLine(SOCKET sock);
	void Close(SOCKET sock);

	void CloseGracefully(SOCKET sock);

protected:
	SOCKET sock;										// Socket
	stringexception stringex;

private:
	void Start();
	static int  nofSockets;
};


