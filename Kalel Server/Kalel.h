#pragma once

#include "../Kalel Shared/Netcode/Server.h"
#include "Automation/ThreadManager.h"

#include "../Kalel Shared/Com Classes/ExperimentData.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../Kalel Shared/Com Classes/MachineSettings.h"

#include <vector>
#include <string>

class Kalel
{
public:
	Kalel();
	~Kalel();

protected:
	ThreadManager threadManager;

	Server server;

	
	std::vector<std::unique_ptr<std::string>> logCollection;		// All non-error logs are stored here
	std::vector<std::unique_ptr<std::string>> errorCollection;		// All error logs are stored here
	std::vector<std::unique_ptr<ExperimentData>> dataCollection;	// Data for an experiment is stored here
};

