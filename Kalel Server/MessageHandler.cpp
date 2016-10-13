#include "stdafx.h"
#include "MessageHandler.h"


MessageHandler::MessageHandler(Storage &h)
	: handles(&h)
{
}


MessageHandler::~MessageHandler()
{
}


// Functions for sending messages below


bool MessageHandler::ExchangeData(const ExperimentData &pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	std::shared_ptr<ExperimentData> newData = std::make_shared<ExperimentData>(pParam);

	// Lock to prevent any synchronisation errors
	handles->sharedMutex.lock();
	handles->dataCollection.push_back(newData);
	handles->sharedMutex.unlock();

	return true;
}


bool MessageHandler::DisplayMessage(int pParam, int pInt1, int pInt2, double pDouble)
{
	// Create a new pointer 
	CString message;
	if (pDouble != default_val)
	{
		message.Format(pParam, pDouble);
	}
	else
	{
		if (pInt1 != default_val)
		{
			if (pInt1 != default_val)
			{
				message.Format(pParam, pInt1, pInt2);
			}
			else
			{
				message.Format(pParam, pInt1);
			}
		}
		else
		{
			message.Format(pParam);
		}
	}
	
	//std::shared_ptr<std::string> newData = std::make_shared<std::string>();
	//handle.logCollection->push_back(newData);

	return true;
}

bool MessageHandler::DisplayMessage(int pParam, std::string m)
{
	
	// Create a new pointer 
	//CString message;
	//message.Format(pParam, m);

	//std::shared_ptr<std::string> newData = std::make_shared<std::string>(message);
	//handle.logCollection->push_back(newData);

	return true;
}

bool MessageHandler::DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, double pDouble1, double pDouble2)
{

	//// Create a new pointer 
	//UINT * type = new UINT(nType);
	//CString * message = new CString;

	//// Format the string. Yes I know it's not the best solution.
	//if (pDouble1 != default_val)
	//{
	//	if (pDouble2 != default_val) {
	//		message->Format(pParam, pDouble1);
	//	}
	//	else {
	//		message->Format(pParam, pDouble1, pDouble2);
	//	}
	//}
	//else
	//{
	//	message->Format(pParam);
	//}

	//// Check if the message box is supposed to alert the user or ask for input
	//// Other thread is now responsible for deleting this object
	//if (blocksProgram)
	//{
	//	if (::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
	//		delete message;
	//		return false;
	//	}
	//}
	//else
	//{
	//	if (::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
	//		delete message;
	//		return false;
	//	}
	//}

	return true;
}

bool MessageHandler::DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, std::string pString)
{
	//// Create a new pointer 
	//UINT * type = new UINT(nType);
	//CString * message = new CString;
	//message->Format(pParam, pString);

	//// Check if the message box is supposed to alert the user or ask for input
	//// Other thread is now responsible for deleting this object
	//if (blocksProgram)
	//{
	//	if (::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
	//		delete message;
	//		return false;
	//	}
	//}
	//else
	//{
	//	if (::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
	//		delete message;
	//		return false;
	//	}
	//}

	return true;
}


bool MessageHandler::ExperimentStart()
{
	//DisplayMessage(MESSAGE_FILLLINE);
	//DisplayMessage(MESSAGE_EXPSTART);
	GraphReset();

	return true;
}

bool MessageHandler::ExperimentEnd()
{
	//DisplayMessage(MESSAGE_FILLLINE);
	GraphReset();

	return true;
}

bool MessageHandler::ThreadShutdown()
{
	//DisplayMessage(MESSAGE_THREAD_SHUTTINGDOWN);
	GraphReset();

	return true;
}


bool MessageHandler::GraphReset()
{
	return true;
}