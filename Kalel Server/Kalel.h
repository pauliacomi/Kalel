#pragma once

#include "Automation/ThreadManager.h"
#include "../Kalel Shared/Netcode/Server.h"

class Kalel
{
public:
	Kalel();
	~Kalel();

	void GetLogs(std::string &logs);

protected:
	Storage storageVectors;
	Server server;
	ThreadManager threadManager;
	
	void ServerProcessing(http_request * req, http_response * resp);
};