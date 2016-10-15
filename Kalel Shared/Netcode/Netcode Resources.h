#ifndef NETCODERES_H
#define NETCODERES_H
#pragma once

#define ERR_WSASTARTUP			"WSAStartup failed"
#define ERR_VERSION				"Version not supported"
#define ERR_GETADDRINFO			"Getaddrinfo failed"
#define ERR_CREATEBIND			"Could not create or bind the socket: "
#define ERR_LISTENING			"Listening to the socket failed: "
#define ERR_SEND				"Send failed: "
#define ERR_RECEIVE				"Receive failed: "
#define ERR_CLOSESOCKET			"Close socket failed: "
#define ERR_LINGER				"Error when setting socket to linger: "
#define ERR_ACCEPT				"Accept failed"
#define ERR_CONNECT				"Unable to connect to server!"
#define ERR_IP_CONVERT			"Error translating IP to readable form: "

#define LOG_LISTENING			" listening on socket: "
#define LOG_ACCEPTED_SOCK		" accepted new socket: "
#define LOG_REQUEST				" request was: \n"
#define LOG_RESPONSE			" response sent: \n"
#endif