
#include "stdafx.h"

#include "MFCMessageHandler.h"

// Message definitions
#include "DefinePostMessages.h"
#include "StringTable.h"						// Error message strings


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

bool MFCMessageHandler::Disconnection()
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_SIGNAL_SERVER_DISCONNECTED, NULL, NULL) == 0) {
		return false;
	}
	return true;
}

bool MFCMessageHandler::SyncComplete()
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_SYNCED, NULL, NULL) == 0) {
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

bool MFCMessageHandler::ExchangeMachineSettings(MachineSettings * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGE_MACHINESETTINGS, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::OnSetExperimentSettings()
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_SET_EXPERIMENTSETTINGS, NULL, NULL) == 0) {
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeExperimentSettings(ExperimentSettings * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGE_EXPERIMENTSETTINGS, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeExperimentStatus(ExperimentStatus * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGE_EXPERIMENTSTATUS, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
		return false;
	}
	return true;
}


bool MFCMessageHandler::ExchangeData(StampedSafeStorage<ExperimentData>::Base * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGEDATA, NULL, (LPARAM)pParam) == 0 ) 
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeLogs(StampedSafeStorage<std::wstring>::Base * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGELOGS, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeRequests(StampedSafeStorage<std::wstring>::Base * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGEREQUESTS, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
		return false;
	}
	return true;
}

bool MFCMessageHandler::ExchangeControlState(ControlInstrumentState * pParam)
{
	// Post the required message, now the main thread is responsible for deleting the new class
	if (::PostMessage(windowHandle, UWM_EXCHANGESTATE, NULL, (LPARAM)pParam) == 0)
	{
		delete pParam;
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


bool MFCMessageHandler::DisplayMessageBoxServer(const std::chrono::system_clock::time_point & tp, const std::wstring & pString)
{
	// Create new pointers to send as a message
	auto time = new std::chrono::system_clock::time_point(tp);
	auto message = new std::wstring(pString);

	// Check if the message box is supposed to alert the user or ask for input
	// Other thread is now responsible for deleting this object
	if (::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOXSERVER, (WPARAM)time, (LPARAM)message) == 0) {
		delete time;
		delete message;
		return false;
	}

	return true;
}

bool MFCMessageHandler::DisplayMessageBox(UINT nType, const std::wstring & pString)
{
	// Create a new pointer 
	UINT * type = new UINT(nType);
	CString * message = new CString(pString.c_str());

	if(::PostMessage(windowHandle, UWM_DISPLAYMESSAGEBOX, (WPARAM)type, (LPARAM)message) == 0) {
		delete type;
		delete message;
		return false;
	}

	return true;
}
