// This class is used to make the interaction between threads and MFC-derived objects 
// as smooth as possible. All messages will be defined in the header and then will be 
// used to sent PostMessage commends to the specified window

#pragma once

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
						// Make sure that it is set to a value that the actual parameters can never take

// Required to pass the experimental data to the main GUI
#include "../Kalel Shared/Com Classes/ExperimentData.h"	
#include "../Kalel Shared/Com Classes/ExperimentStatus.h"	
#include "../Kalel Shared/Com Classes/MachineSettings.h"
#include "../Kalel Shared/Com Classes/ControlInstrumentState.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"

#include <string>
#include <map>
#include <deque>

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
	bool Disconnection();
	bool SyncComplete();

	bool OnSetMachineSettings();
	bool ExchangeMachineSettings(MachineSettings *pParam);

	bool OnSetExperimentSettings();
	bool ExchangeExperimentSettings(ExperimentSettings *pParam);
	bool ExchangeExperimentStatus(ExperimentStatus *pParam);

	bool ExchangeData(StampedSafeStorage<ExperimentData>::Base * pParam);
	bool ExchangeLogs(StampedSafeStorage<std::wstring>::Base * pParam);
	bool ExchangeRequests(StampedSafeStorage<std::wstring>::Base * pParam);

	bool ExchangeControlState(ControlInstrumentState *pParam);
	bool ExchangeControlStateSpecific(const ControlInstrumentStateData& pParam);

	// Messageboxes
	bool DisplayMessageBox(UINT nType, const std::wstring & pString);
	bool DisplayMessageBoxServer(const std::chrono::system_clock::time_point & tp, const std::wstring & pString);
};