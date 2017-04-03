#ifndef SERVER_H
#define SERVER_H
#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"
#include "http_define.h"

#include <atomic>
#include <thread>
#include <functional>
#include <unordered_map>

class Server
{
public:
	// If no port specified, server defaults to http (80)
	Server(PCSTR port = "http");
	~Server();

	void AddMethod(std::function<void(http_request*, http_response*)> r, std::string url);
	void Start();

protected:
	Socket listeningSocket;
	
	std::atomic_bool accepting;							// Powers the main loop
	unsigned AcceptLoop();								// Function started as a thread to listen to incoming connections
	unsigned Process(std::unique_ptr<Socket> sock);		// Function started when a socket connects
	std::thread acceptThread;

	std::unordered_map<std::string, std::function<void(http_request*, http_response*)>> funcMap;
};

#endif