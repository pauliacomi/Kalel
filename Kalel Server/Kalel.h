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

	void GetLogs(std::vector<std::string> &logs);

protected:
	void ServerProcessing(http_request * req, http_response * resp);
	ThreadManager threadManager;
	Server server;
	Handles handles;
	UnicodeConv unicodeConverter;

	std::vector<std::string> serverLogs;
	std::shared_ptr<MachineSettings> machineSettings;

	std::vector<std::shared_ptr<std::string>> logCollection;		// All non-error logs are stored here
	std::vector<std::shared_ptr<std::string>> errorCollection;		// All error logs are stored here
	std::vector<std::shared_ptr<ExperimentData>> dataCollection;	// Data for an experiment is stored here
};