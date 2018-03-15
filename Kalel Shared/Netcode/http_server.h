#pragma once

#include "Socket.h"
#include "http_request.h"
#include "http_response.h"
#include "http_define.h"

#include <atomic>
#include <thread>
#include <functional>
#include <vector>
#include <unordered_map>

class HTTPServer
{
public:
	// If no port specified, server defaults to http (80)
	HTTPServer(PCSTR port = "http");
	~HTTPServer();

	void AddMethod(std::function<void(http_request*, http_response*)> r, std::string url);
	void Start();

protected:
	Socket listeningSocket;
	std::thread acceptThread;
	std::vector<std::thread> threadPool;
	std::unordered_map<std::string, std::function<void(http_request*, http_response*)>> funcMap;	// Map of different assigned functions for custom request processing
	
	std::atomic_bool accepting;							// Powers the main loop
	unsigned AcceptLoop();								// Function started as a thread to listen to incoming connections
	unsigned Process(std::unique_ptr<Socket> sock);		// Function started when a socket connects

	unsigned ReceiveRequest(Socket & sock, http_request & req, http_response & resp);
	unsigned SendResponse(Socket & sock, const http_response & resp);

};