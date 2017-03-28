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

	void Ping(http_request* req, http_response* resp);
	void Sync(http_request * req, http_response * resp);
	void MachineSettingsSync(http_request* req, http_response* resp);
	void ExperimentSettingsSync(http_request* req, http_response* resp);
	void InstrumentStateSync(http_request* req, http_response* resp);
	void DataSync(http_request* req, http_response* resp);
	void LogSync(http_request* req, http_response* resp);
	void RequestSync(http_request* req, http_response* resp);
	void AutomationControl(http_request* req, http_response* resp);
	void Debug(http_request* req, http_response* resp);
};
