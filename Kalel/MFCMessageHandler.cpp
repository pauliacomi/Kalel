#include "stdafx.h"
#include "MFCMessageHandler.h"

// Message definitions
#include "DefinePostMessages.h"
#include "../Kalel Shared/Resources/StringTable.h"


MFCMessageHandler::MFCMessageHandler()
{
}


MFCMessageHandler::~MFCMessageHandler()
{
}

bool MFCMessageHandler::setHandle(HWND h)
{
	ASSERT(h != NULL);
	windowHandle = h;
	return false;
}


// Functions for sending messages below

bool MFCMessageHandler::ConnectionComplete()
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_SIGNAL_SERVER_CONNECTED, NULL, NULL) == 0) {
		return false;
	}
	return true;
}


bool MFCMessageHandler::OnSetMachineSettings()
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_SET_MACHINESETTINGS, NULL, NULL) == 0) {
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeMachineSettings(const MachineSettings &pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	MachineSettings * newData = new MachineSettings(pParam);

	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGE_MACHINESETTINGS, NULL, (LPARAM)newData) == 0)
	{
		delete newData;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeExperimentSettings(const ExperimentSettings &pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	ExperimentSettings * newData = new ExperimentSettings(pParam);

	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGE_EXPERIMENTSETTINGS, NULL, (LPARAM)newData) == 0)
	{
		delete newData;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeData(std::deque<ExperimentData*> * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGEDATA, NULL, (LPARAM)pParam) == 0 ) 
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeLogs(std::map<std::wstring, std::wstring>* pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGELOGS, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeControlState(const ControlInstrumentState &pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	ControlInstrumentState * newData = new ControlInstrumentState(pParam);

	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGESTATE, NULL, (LPARAM)newData) == 0)
	{
		delete newData;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeControlStateSpecific(const ControlInstrumentStateData &pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	ControlInstrumentStateData * newData = new ControlInstrumentStateData(pParam);

	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGESTATESPECIFIC, NULL, (LPARAM)newData) == 0)
	{
		delete newData;
		return false;
	}
	return true;
}

bool MFCMessageHandler::DisplayMessage(int pParam, int pInt1, int pInt2, double pDouble)
{
	// Create a new pointer 
	CString * message = new CString;
	if (pDouble != default_val)
	{
		message->Format(pParam, pDouble);
	}
	else
	{
		if (pInt1 != default_val)
		{
			if (pInt1 != default_val)
			{
				message->Format(pParam, pInt1, pInt2);
			}
			else
			{
				message->Format(pParam, pInt1);
			}
		}
		else
		{
			message->Format(pParam);
		}
	}

	// Other thread is now responsible for deleting this object
	if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGE, NULL, (LPARAM)message) == 0) {
		delete message;
		return false;
	}

	return true;
}

bool MFCMessageHandler::DisplayMessage(int pParam, std::wstring m)
{
	// Create a new pointer 
	CString * message = new CString;
	message->Format(pParam, m.c_str());
	
	// Other thread is now responsible for deleting this object
	if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGE, NULL, (LPARAM)message) == 0) {
		delete message;
		return false;
	}

	return true;
}

bool MFCMessageHandler::DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, double pDouble1, double pDouble2)
{
	// Create a new pointer 
	UINT * type = new UINT(nType);
	CString * message = new CString;

	// Format the string. Yes I know it's not the best solution.
	if (pDouble1 != default_val)
	{
		if (pDouble2 != default_val){
			message->Format(pParam, pDouble1);
		}
		else {
			message->Format(pParam, pDouble1, pDouble2);
		}
	}
	else
	{
		message->Format(pParam);
	}

	// Check if the message box is supposed to alert the user or ask for input
	// Other thread is now responsible for deleting this object
	if (blocksProgram)
	{
		if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
			delete message;
			return false;
		}
	}
	else
	{
		if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
			delete message;
			return false;
		}
	}

	return true;
}

bool MFCMessageHandler::DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, std::wstring pString)
{
	// Create a new pointer 
	UINT * type = new UINT(nType);
	CString * message = new CString;
	message->Format(pParam, pString.c_str());

	// Check if the message box is supposed to alert the user or ask for input
	// Other thread is now responsible for deleting this object
	if (blocksProgram)
	{
		if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
			delete message;
			return false;
		}
	}
	else
	{
		if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
			delete message;
			return false;
		}
	}

	return true;
}


bool MFCMessageHandler::ExperimentStart()
{
	DisplayMessage(MESSAGE_FILLLINE);
	DisplayMessage(MESSAGE_EXPSTART);

	return true;
}

bool MFCMessageHandler::ExperimentEnd()
{
	::PostMessage(windowHandle, UWM_THREADFINISHEDREG, NULL, NULL);
	DisplayMessage(MESSAGE_FILLLINE);

	return true;
}

bool MFCMessageHandler::ThreadShutdown()
{
	::PostMessage(windowHandle, UWM_THREADSHUTDOWN, NULL, NULL);
	DisplayMessage(MESSAGE_THREAD_SHUTTINGDOWN);

	return true;
}
