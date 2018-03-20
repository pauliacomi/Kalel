#pragma once

#include "MFCMessageHandler.h"

#include "../Kalel Shared/Netcode/http_client.h"
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

	void GetMachineSettings(std::string fromTime = R"()");
	void SetMachineSettings(const MachineSettings &ptr);
	
	void GetExperimentSettings(std::string fromTime = R"()");
	void SetExperimentSettings(const ExperimentSettings &ptr);
	
	void GetControlInstrumentState(std::string fromTime = R"()");
	void ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	
	void GetExperimentStatus(std::string fromTime = R"()");
	void GetData(std::string fromTime = R"()");
	void GetLog(std::string fromTime = R"()");
	void GetRequests(std::string fromTime = R"()");

	void StartClient();
	void ShutdownClient();
	void RestartClient();
	void ResetClient();
	void PauseClient();
	void ResumeClient();
	void SetUserChoice();

	void FunctionSampleVacuum();
	void FunctionBottleVacuum();
	void FunctionChangeBottle();

private:
	Client client;
	MFCMessageHandler messageHandler;
	
	void ThreadCommand(int command);
	void FunctionalityCommand(int functionality);
	void UserChoice(int choice);

	/**********************************************************************************************************************************
	// Local variables: don't like these
	**********************************************************************************************************************************/

	std::string localAddress;								// Server address

	ControlInstrumentStateData localInstrumentState;

	// Sync flags
	std::atomic_bool flagSyncRequest = false;
	std::atomic_bool flagReqRequest = false;
	std::atomic_bool flagLogsRequest = false;
	std::atomic_bool flagExperimentRequest = false;
	std::atomic_bool flagMachineSettingsRequest = false;
	std::atomic_bool flagExperimentSettingsRequest = false;
	std::atomic_bool flagInstrumentStateRequest = false;

	/**********************************************************************************************************************************
	// Request and response functions
	**********************************************************************************************************************************/
	// Handshake/ping
	unsigned Handshake_req(http_request* r);
	unsigned Handshake_resp(http_response * r);

	// Machine settings set
	unsigned GetMachineSettings_req(http_request * r, std::string fromTime);
	unsigned GetMachineSettings_resp(http_response * r);

	// Machine settings get
	unsigned SetMachineSettings_req(http_request * r, MachineSettings ms);
	unsigned SetMachineSettings_resp(http_response * r);

	// Experiment Settings Gets
	unsigned GetExperimentSettings_req(http_request * r, std::string fromTime);
	unsigned GetExperimentSettings_resp(http_response * r);

	// Experiment Settings Set
	unsigned SetExperimentSettings_req(http_request * r, ExperimentSettings ms);
	unsigned SetExperimentSettings_resp(http_response * r);

	// Experiment Status Gets
	unsigned GetExperimentStatus_req(http_request * r, std::string fromTime);
	unsigned GetExperimentStatus_resp(http_response * r);

	// Instrument state get
	unsigned GetInstrumentState_req(http_request * r, std::string fromTime);
	unsigned GetInstrumentState_resp(http_response * r);

	// Instrument state set
	unsigned SetInstrumentState_req(http_request * r, int instrumentType, int instrumentNumber, int instrumentState);
	unsigned SetInstrumentState_resp(http_response * r);
	
	// Data sync
	unsigned GetData_req(http_request * r, std::string fromTime);
	unsigned GetData_resp(http_response * r);

	// Logs sync
	unsigned GetLogs_req(http_request * r, std::string fromTime);
	unsigned GetLogs_resp(http_response * r);

	// Request/Error sync
	unsigned GetRequest_req(http_request * r, std::string fromTime);
	unsigned GetRequest_resp(http_response * r);
	
	// Automation/experiment commands
	unsigned ThreadCommand_req(http_request * r, int command);
	unsigned ThreadCommand_resp(http_response * r);
	
	// Functionality commands
	unsigned FunctionalityCommand_req(http_request * r, int functionality);
	unsigned FunctionalityCommand_resp(http_response * r);

	// User choice commands
	unsigned UserChoice_req(http_request * r, int choice);
	unsigned UserChoice_resp(http_response * r);
};