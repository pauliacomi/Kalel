// This class is used to make the interaction between threads and MFC-derived objects 
// as smooth as possible. All messages will be defined in the header and then will be 
// used to sent PostMessage commends to the specified window

#pragma once
#ifndef MFCMESSAGEHANDLER_H
#define MFCMESSAGEHANDLER_H

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
						// Make sure that it is set to a value that the actual parameters can never take

// Required to pass the experimental data to the main GUI
#include "../Kalel Shared/Com Classes/ExperimentData.h"	
#include "../Kalel Shared/Com Classes/MachineSettings.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"

#include <string>
#include <map>

class MFCMessageHandler
{
	// Constructor and destructor
public:
	MFCMessageHandler();
	virtual ~MFCMessageHandler();
	
	// Window handle for message postback
	HWND windowHandle;

public:
	bool setHandle(HWND h);
	// PostMessage functions
	bool ConnectionComplete();
	bool GotMachineSettings(const MachineSettings &pParam);
	bool OnSetMachineSettings();
	bool ExchangeData(std::deque<ExperimentData*> * pParam);
	bool ExchangeLogs(std::map<std::wstring, std::wstring> * pParam);
	bool ExchangeControlState(ControlInstrumentState* pParam);
	bool ExperimentStart();
	bool ExperimentEnd();
	bool ThreadShutdown();
	bool DisplayMessage(int pParam, int pInt1 = default_val, int pInt2 = default_val, double pDouble = default_val);								// Simple display message which takes an int

	bool DisplayMessage(int pParam, std::wstring m);

	// Messageboxes
	bool DisplayMessageBox(int message, UINT nType, bool blocksProgram, double pDouble1 = default_val, double pDouble2 = default_val);
	bool DisplayMessageBox(int pParam, UINT nType, bool blocksProgram, std::wstring pString);
};


#endif // !MFCMESSAGEHANDLER_H