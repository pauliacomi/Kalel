#include "stdafx.h"
#include "MFCMessageHandler.h"

// Message definitions
#include "DefinePostMessages.h"


MFCMessageHandler::MFCMessageHandler()
	:windowHandle(NULL)
{
}


MFCMessageHandler::~MFCMessageHandler()
{
}

BOOL MFCMessageHandler::SetHandle(HWND h)
{
	// Get view handler and check for validity
	windowHandle = reinterpret_cast<HWND>(h);
	ASSERT(windowHandle != NULL);

	return 0;
}


// Functions for sending messages below


BOOL MFCMessageHandler::ExchangeData(ExperimentData pParam)
{
	// Create a new instance of the storage class and equate it to the local class
	ExperimentData * newData = new ExperimentData();
	*newData = pParam;

	// Post the required message, now the main thread is responsible for deleting the new class
	::PostMessage(windowHandle, WM_EXCHANGEDATA, NULL, (LPARAM)newData);

	return 0;
}


BOOL MFCMessageHandler::DisplayMessage(int pParam, int pInt1, int pInt2, double pDouble)
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
	::PostMessage(windowHandle, WM_DISPLAYMESSAGE, NULL, (LPARAM)message);

	return 0;
}

BOOL MFCMessageHandler::DisplayMessage(int pParam, CString m)
{
	// Create a new pointer 
	CString * message = new CString;
	message->Format(pParam, m);
	
	// Other thread is now responsible for deleting this object
	::PostMessage(windowHandle, WM_DISPLAYMESSAGE, NULL, (LPARAM)message);

	return 0;
}

BOOL MFCMessageHandler::DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, double pDouble1, double pDouble2)
{
	// Create a new pointer 
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
		::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOXCONF, nType, (LPARAM)message);
	}
	else
	{
		::PostMessage(windowHandle, WM_DISPLAYMESSAGEBOX, nType, (LPARAM)message);
	}

	return 0;
}

void MFCMessageHandler::ExperimentStart()
{
	GraphReset();
}

void MFCMessageHandler::ExperimentEnd()
{
	::PostMessage(windowHandle, WM_THREADFINISHEDREG, NULL, NULL);
}

void MFCMessageHandler::GraphReset() 
{
	::PostMessage(windowHandle, WM_GRAPHRESET, NULL, NULL);
}