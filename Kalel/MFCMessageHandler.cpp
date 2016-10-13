#include "stdafx.h"
#include "MFCMessageHandler.h"

// Message definitions
#include "DefinePostMessages.h"
#include "Resources/StringTable.h"


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

bool MFCMessageHandler::GotMachineSettings(const MachineSettings &pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	MachineSettings * newData = new MachineSettings(pParam);

	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_GOT_MACHINE_SETTINGS, NULL, (LPARAM)newData) == 0) 
	{
		delete newData;
		return false;
	}
	return true;
}

bool MFCMessageHandler::OnSync()
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_SYNCED, NULL, NULL) == 0) {
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeData(std::deque<ExperimentData*> * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, WM_EXCHANGEDATA, NULL, (LPARAM)pParam) == 0 ) 
	{
		delete pParam;
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
	if(::PostMessage(windowHandle, WM_DISPLAYMESSAGE, NULL, (LPARAM)message) == 0) {
		delete message;
		return false;
	}

	return true;
}

bool MFCMessageHandler::DisplayMessage(int pParam, std::wstring m)
{
	// Create a new pointer 
	CString * message = new CString;
	message->Format(pParam, m);
	
	// Other thread is now responsible for deleting this object
	if(::PostMessage(windowHandle, WM_DISPLAYMESSAGE, NULL, (LPARAM)message) == 0) {
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
		if(::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
			delete message;
			return false;
		}
	}
	else
	{
		if(::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
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
		if(::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOXCONF, (WPARAM)type, (LPARAM)message) == 0) {
			delete message;
			return false;
		}
	}
	else
	{
		if(::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
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
	GraphReset();

	return true;
}

bool MFCMessageHandler::ExperimentEnd()
{
	::PostMessage(windowHandle, WM_THREADFINISHEDREG, NULL, NULL);
	DisplayMessage(MESSAGE_FILLLINE);
	GraphReset();

	return true;
}

bool MFCMessageHandler::ThreadShutdown()
{
	::PostMessage(windowHandle, WM_THREADSHUTDOWN, NULL, NULL);
	DisplayMessage(MESSAGE_THREAD_SHUTTINGDOWN);
	GraphReset();

	return true;
}


bool MFCMessageHandler::GraphReset()
{
	::PostMessage(windowHandle, WM_GRAPHRESET, NULL, NULL);

	return true;
}