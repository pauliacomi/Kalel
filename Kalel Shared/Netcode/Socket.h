#pragma once

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif

//#define _WINSOCK_DEPRECATED_NO_WARNINGS
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
#include <atomic>

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

	SOCKET GetSocket();

	void Listen(PCSTR port);
	void Accept_PrimeSelect();
	std::string Socket::Accept(SOCKET &clientSocket, timeval tv);
	unsigned Connect(PCSTR ip, PCSTR port);

	std::string Send(const std::string& sendbuf);
	std::string SendLine(const std::string& sendbuf);
	std::string Receive();
	std::string ReceiveLine();
	std::string ReceiveBytes(u_long bytes);

	// Set socket to linger after closing, SHOULD NOT BE NORMALLY USED
	// Will throw on error
	void SetLinger(bool lingerOn);

	// Disable or enable Nagle's algorithm.
	// Normally enabled
	// Will throw on error
	void SetNagle(bool nagleOn);

	// Close a socket
	// Will throw on error
	void Close();

	// Close a socket
	// First send shutdown signal
	// Will throw on error
	void CloseGracefully();

	std::string GetIP(const sockaddr_storage &address);

protected:
	SOCKET sock;						// Socket
	stringexception stringex;			// Custom exception

	// For select functionality
	fd_set * master;					// master file descriptor list
	fd_set * readfds;					// Set of listening sockets

private:
	void Start();
	static int  nofSockets;
};