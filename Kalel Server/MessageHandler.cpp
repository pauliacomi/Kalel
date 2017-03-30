#include "MessageHandler.h"

#include "../Kalel Shared/Resources/DefineText.h"
#include "../Kalel Shared/Com Classes/ExperimentData.h"
#include "../Kalel Shared/timeHelpers.h"
#include "../Kalel Shared/stringFormat.h"
#include "Automation/CommonPointers.h"

MessageHandler::MessageHandler(Storage &h)
	: storage{ h }
{
}


MessageHandler::~MessageHandler()
{
}


// Functions for sending messages below


bool MessageHandler::ExchangeData(const ExperimentData &pParam)
{
	// Create a new instance of the storage cla	ss and equate it to the local class
	std::shared_ptr<ExperimentData> newData = std::make_shared<ExperimentData>(pParam);

	// Add to map
	storage.pushData(timeh::NowTime(), newData);

	return true;
}


bool MessageHandler::DisplayMessage(std::string pParam, int pInt1, int pInt2, double pDouble)
{
	// Create a new pointer 
	std::string message;
	if (pDouble != default_val)
	{
		message = string_format(message, pParam, pDouble );
	}
	else
	{
		if (pInt1 != default_val)
		{
			if (pInt1 != default_val)
			{
				message = string_format(pParam, pInt1, pInt2);
			}
			else
			{
				message = string_format(pParam, pInt1);
			}
		}
		else
		{
			message = string_format(pParam);
		}
	}
	
	storage.pushInfoLogs(timeh::NowTime(), message);

	return true;
}

bool MessageHandler::DisplayMessage(std::string pParam, std::string m)
{
	std::string message;
	message = string_format(pParam, m);

	storage.pushInfoLogs(timeh::NowTime(), message);

	return true;
}

bool MessageHandler::DisplayMessageBox(std::string pParam, unsigned nType, bool blocksProgram, double pDouble1, double pDouble2)
{
	std::string message;

	// Format the string. Yes I know it's not the best solution.
	if (pDouble1 != default_val)
	{
		if (pDouble2 != default_val) {
			message = string_format(pParam, pDouble1);
		}
		else {
			message = string_format(pParam, pDouble1, pDouble2);
		}
	}
	else
	{
		message = string_format(pParam);
	}

	//storage.automationErrorLogs.insert(std::make_pair(timeh::NowTime(), message));

	return true;
}

bool MessageHandler::DisplayMessageBox(std::string pParam, unsigned nType, bool blocksProgram, std::string pString)
{
	std::string message;
	message = string_format(pParam, pString);

	//storage.automationErrorLogs.insert(std::make_pair(timeh::NowTime(), message));

	//// Check if the message box is supposed to alert the user or ask for input
	//// Other thread is now responsible for deleting this object
	//if (blocksProgram)
	//{
	//	if (::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
	//		delete message;
	//		return false;
	//	}
	//}
	//else
	//{
	//	if (::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
	//		delete message;
	//		return false;
	//	}
	//}

	return true;
}


bool MessageHandler::ExperimentStart()
{
	DisplayMessage(MESSAGE_FILLLINE);
	DisplayMessage(MESSAGE_EXPSTART);

	return true;
}

bool MessageHandler::ExperimentEnd()
{
	DisplayMessage(MESSAGE_FILLLINE);

	return true;
}

bool MessageHandler::ThreadShutdown()
{
	DisplayMessage(MESSAGE_THREAD_SHUTTINGDOWN);

	return true;
}
