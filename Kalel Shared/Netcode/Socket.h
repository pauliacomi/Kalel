#ifndef SOCKET_H
#define SOCKET_H
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
	void set(std::string l_err)
	{
		this->err = l_err;
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

	std::string Send(SOCKET sock, const std::string& sendbuf);
	std::string SendLine(SOCKET sock, const std::string& sendbuf);
	std::string Receive(SOCKET sock);
	std::string ReceiveLine(SOCKET sock);
	std::string ReceiveBytes(SOCKET l_sock, u_long bytes);
	void Close(SOCKET &sock);
	void CloseGracefully(SOCKET &sock);

	std::string GetIP(const sockaddr_storage &address);

protected:
	SOCKET sock;										// Socket
	stringexception stringex;

private:
	void Start();
	static int  nofSockets;
};

#endif
