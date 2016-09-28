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
#include <memory>

struct Handles {
	std::shared_ptr<ExperimentSettings> experimentSettings;
	std::shared_ptr<MachineSettings> machineSettings;

	std::vector<std::shared_ptr<std::string>> * logCollection;		// All non-error logs are stored here
	std::vector<std::shared_ptr<std::string>> * errorCollection;		// All error logs are stored here
	std::vector<std::shared_ptr<ExperimentData>> * dataCollection;	// Data for an experiment is stored here
};

class MessageHandler
{
	// Constructor and destructor
public:
	MessageHandler(Handles h);
	virtual ~MessageHandler();

	// Message postback
	Handles handle;

public:

	// PostMessage functions
	bool ExperimentStart();
	bool ExperimentEnd();
	bool ThreadShutdown();
	bool GraphReset();
	bool ExchangeData(ExperimentData pParam);
	bool DisplayMessage(int pParam, int pInt1 = default_val, int pInt2 = default_val, double pDouble = default_val);								// Simple display message which takes an int

	bool DisplayMessage(int pParam, std::string m);

	// Messageboxes
	bool DisplayMessageBox(int message, UINT nType, bool blocksProgram, double pDouble1 = default_val, double pDouble2 = default_val);
	bool DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, std::string pString);
};


#endif // !MESSAGEHANDLER_H