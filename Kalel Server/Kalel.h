#pragma once

#include "../Kalel Shared/Netcode/Server.h"
#include "Automation/ThreadManager.h"
#include "Automation/CommonControls.h"
#include "Automation/CommonPointers.h"

class Kalel
{
public:
	Kalel();
	~Kalel();

	Server server;								// Http server for REST
	Storage storageVectors;						// Stores all settings, experiment data, logs etc
	Controls controlMechanisms;					// Controls all machines
	ThreadManager threadManager;	

protected:

	void Ping(http_request* req, http_response* resp);
	void Sync(http_request * req, http_response * resp);
	void MachineSettingsSync(http_request* req, http_response* resp);
	void ExperimentSettingsSync(http_request* req, http_response* resp);
	void ExperimentStatusSync(http_request* req, http_response* resp);
	void InstrumentStateSync(http_request* req, http_response* resp);
	void DataSync(http_request* req, http_response* resp);
	void LogSync(http_request* req, http_response* resp);
	void RequestSync(http_request* req, http_response* resp);
	void AutomationControl(http_request* req, http_response* resp);
	void ReloadParameters(http_request* req, http_response* resp);
};
