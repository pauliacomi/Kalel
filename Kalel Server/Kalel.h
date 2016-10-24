#pragma once

#include "Automation/ThreadManager.h"

#include "../Kalel Shared/Netcode/Server.h"
#include "../Kalel Shared/Com Classes/ExperimentData.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../Kalel Shared/Com Classes/MachineSettings.h"
#include "../Kalel Shared/unicodeConv.h"

#include "Parameters/Parametres.h"
#include "MessageHandler.h"

#include <vector>
#include <string>

class Kalel
{
public:
	Kalel();
	~Kalel();

	void GetLogs(std::string &logs);

protected:
	Server server;
	void ServerProcessing(http_request * req, http_response * resp);
	
	ThreadManager threadManager;
	Storage storageVectors;
};