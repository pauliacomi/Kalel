#ifndef COMMHANDLER_H
#define COMMHANDLER_H
#pragma once

#include "MFCMessageHandler.h"

#include "../Kalel Shared/Netcode/Client.h"
#include "../Kalel Shared/Com Classes/MachineSettings.h"
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"

#include <string>


class CommHandler
{
public:
	CommHandler();
	~CommHandler();

	void SetHandle(HWND h);

	void Connect(std::wstring address);
	void SaveAddress(std::wstring address);
	void Sync();

	void GetMachineSettings();
	void SetMachineSettings(std::shared_ptr<const MachineSettings> ptr);
	
	void GetExperimentSettings();
	void SetExperimentSettings(std::shared_ptr<const ExperimentSettings> ptr);
	
	void GetControlInstrumentState();
	void ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	
	void GetData(std::string fromTime = R"()");
	void GetLog(std::wstring fromTime = _T(""));
	void GetRequests(std::wstring fromTime = _T(""));;

	void StartClient();
	void ShutdownClient();
	void RestartClient();
	void ResetClient();
	void PauseClient();
	void ResumeClient();
	void SetUserContinue();

	void FunctionSampleVacuum();
	void FunctionBottleVacuum();
	void FunctionChangeBottle();

	void TestConn();

private:
	Client client;
	MFCMessageHandler messageHandler;
	
	void ThreadCommand();
	void FunctionalityCommand();

	/**********************************************************************************************************************************
	// Local variables: don't like these
	**********************************************************************************************************************************/

	std::string localAddress;								// Address

	std::atomic_bool flagExperimentRequest = false;
	std::string localExperimentTime;						// Start time requested for GetExperimentData 

	std::atomic_bool flagLogsRequest = false;
	std::string localLogsTime;

	std::atomic_bool flagReqRequest = false;
	std::string localReqTime;

	ControlInstrumentStateData localInstrumentState;

	std::shared_ptr<const MachineSettings> localMachineSettings;
	std::shared_ptr<const ExperimentSettings> localExperimentSettings;
	int localThreadCommand = 0;
	int localFunctionalityCommand = 0;

	// Debugging
	int debug_success = 0;
	int debug_fails = 0;

	/**********************************************************************************************************************************
	// Request and response functions
	**********************************************************************************************************************************/
	// Handshake/ping
	unsigned Handshake_req(http_request* r);
	unsigned Handshake_resp(http_response * r);
	
	// Machine settings set
	unsigned GetMachineSettings_req(http_request * r);
	unsigned GetMachineSettings_resp(http_response * r);

	// Machine settings get
	unsigned SetMachineSettings_req(http_request * r);
	unsigned SetMachineSettings_resp(http_response * r);

	// Experiment Settings Gets
	unsigned GetExperimentSettings_req(http_request * r);
	unsigned GetExperimentSettings_resp(http_response * r);

	// Experiment Settings Set
	unsigned SetExperimentSettings_req(http_request * r);
	unsigned SetExperimentSettings_resp(http_response * r);

	// Instrument state get
	unsigned GetInstrumentState_req(http_request * r);
	unsigned GetInstrumentState_resp(http_response * r);

	// Instrument state set
	unsigned SetInstrumentState_req(http_request * r);
	unsigned SetInstrumentState_resp(http_response * r);
	
	// Data sync
	unsigned GetData_req(http_request * r);
	unsigned GetData_resp(http_response * r);

	// Logs sync
	unsigned GetLogs_req(http_request * r);
	unsigned GetLogs_resp(http_response * r);

	// Request/Error sync
	unsigned GetRequest_req(http_request * r);
	unsigned GetRequest_resp(http_response * r);
	
	// Automation/experiment commands
	unsigned ThreadCommand_req(http_request * r);
	unsigned ThreadCommand_resp(http_response * r);
	
	// Functionality commands
	unsigned FunctionalityCommand_req(http_request * r);
	unsigned FunctionalityCommand_resp(http_response * r);

	// Debugging
	unsigned TestConn_req(http_request * r);
	unsigned TestConn_resp(http_response * r);
};


#endif