#pragma once

#ifdef _WIN32
/* See http://stackoverflow.com/questions/12765743/getaddrinfo-on-win32 */
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0501  /* Windows XP. */
#endif
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#else
/* Assume that any non-Windows platform uses POSIX-style sockets instead. */
#define	INVALID_SOCKET -1	/* To make sure the POSIX-int handle can be compared to the WIN-uint handle  */
typedef	SOCKET int;			/* To make sure the POSIX-int handle can be compared to the WIN-uint handle  */
#endif

class Socket
{
public:
	Socket();
	~Socket();

	void Open(PCSTR ip, PCSTR port);
	void Close();

	SOCKET Get();

private:
	SOCKET sock = INVALID_SOCKET;
};

