#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#pragma once

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
// Make sure that it is set to a value that the actual parameters can never take

// Required to pass the experimental data to the main GUI
#include "../../Kalel/Kalel Shared/Com Classes/ExperimentData.h"
#include "../../Kalel/Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../../Kalel/Kalel Shared/Com Classes/MachineSettings.h"

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <mutex>

class Storage {
public:
	// Logs
	std::mutex serverLogsMtx;											// Mutex for the server logs					
	std::vector<std::string> serverLogs;								// Logs from the server are stored here
	
	std::mutex autoInfoLogsMutex;										// Synchronisation class, should be used whenever there are writes to the logs
	std::map<std::string, std::string> automationInfoLogs;				// All non-error logs are stored here
	std::map<std::string, std::string> automationErrorLogs;				// All error logs are stored here

	std::shared_ptr<MachineSettings> machineSettings;					// The machine settings are here
	std::shared_ptr<ExperimentSettings> experimentSettings;				// The experiment settings are here

	std::mutex sharedMutex;												// Synchronisation class, should be used whenever there are writes to the deque
	std::deque<std::shared_ptr<ExperimentData>> dataCollection;			// The collection of data from an experiment
};



class MessageHandler
{
	// Constructor and destructor
public:
	MessageHandler(Storage &h);
	virtual ~MessageHandler();

	// Message postback
	Storage * handles;

public:

	// PostMessage functions
	bool ExperimentStart();
	bool ExperimentEnd();
	bool ThreadShutdown();
	bool GraphReset();
	bool ExchangeData(const ExperimentData &pParam);

	bool DisplayMessage(std::string pParam, int pInt1 = default_val, int pInt2 = default_val, double pDouble = default_val);							// Simple display message which takes an int
	bool DisplayMessage(std::string pParam, std::string m);

	// Messageboxes
	bool DisplayMessageBox(std::string message, UINT nType, bool blocksProgram, double pDouble1 = default_val, double pDouble2 = default_val);
	bool DisplayMessageBox(std::string pParam, UINT nType, bool blocksProgram, std::string pString);
};


#endif // !MESSAGEHANDLER_H