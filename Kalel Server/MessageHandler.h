#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#pragma once

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
// Make sure that it is set to a value that the actual parameters can never take

#include "Automation/CommonPointers.h"

#include <string>
#include <memory>

class ExperimentData;

class MessageHandler
{
	// Constructor and destructor
public:
	MessageHandler(Storage &h);
	virtual ~MessageHandler();

	// Message postback
	Storage & storage;

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
	bool DisplayMessageBox(std::string message, unsigned nType, bool blocksProgram, double pDouble1 = default_val, double pDouble2 = default_val);
	bool DisplayMessageBox(std::string pParam, unsigned nType, bool blocksProgram, std::string pString);
};


#endif // !MESSAGEHANDLER_H