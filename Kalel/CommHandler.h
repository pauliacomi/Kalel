#ifndef COMMHANDLER_H
#define COMMHANDLER_H
#pragma once

#include "../Kalel Shared/Netcode/Client.h"
#include "MFCMessageHandler.h"

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
	void GetData(time_t startTime = 0, long int measurementsMade = 0);
	void GetLog(std::wstring fromTime = _T(""));
	void SetExperimentSettings(std::shared_ptr<const ExperimentSettings> ptr);

	void StartClient();
	void ShutdownClient();
	void RestartClient();
	void ResetClient();
	void PauseClient();
	void ResumeClient();

	void SetUserContinue();

	void ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);

	void TestConn();

private:
	Client client;
	MFCMessageHandler messageHandler;
	
	void ThreadCommand();

	/**********************************************************************************************************************************
	// Local variables: don't like these
	**********************************************************************************************************************************/

	std::string localAddress;								// Address

	bool flagExperimentRequest = false;
	std::string localExperimentStartTime;					// Start time requested for GetExperimentData 
	std::string localMeasurementsMade;						// Number of measurements requested for GetExperimentData

	bool flagLogsRequest = false;
	std::string localLogsTime;
	std::string localInstrumentType;
	std::string localInstrumentNumber;
	std::string localShouldBeActivated;
	std::shared_ptr<const MachineSettings> localMachineSettings;
	std::shared_ptr<const ExperimentSettings> localExperimentSettings;
	int localThreadCommand;

	// Debugging
	int debug_success = 0;
	int debug_fails = 0;

	/**********************************************************************************************************************************
	// Request and response functions
	**********************************************************************************************************************************/
	unsigned Handshake_req(http_request* r);
	unsigned Handshake_resp(http_response * r);
	unsigned GetMachineSettings_req(http_request * r);
	unsigned GetMachineSettings_resp(http_response * r);
	unsigned SetMachineSettings_req(http_request * r);
	unsigned SetMachineSettings_resp(http_response * r);
	unsigned GetData_req(http_request * r);
	unsigned GetData_resp(http_response * r);
	unsigned GetLogs_req(http_request * r);
	unsigned GetLogs_resp(http_response * r);
	unsigned SetExperimentSettings_req(http_request * r);
	unsigned SetExperimentSettings_resp(http_response * r);
	unsigned ThreadCommand_req(http_request * r);
	unsigned ThreadCommand_resp(http_response * r);
	unsigned InstrumentCommand_req(http_request * r);
	unsigned InstrumentCommand_resp(http_response * r);

	// Debugging
	unsigned TestConn_req(http_request * r);
	unsigned TestConn_resp(http_response * r);
};


#endif