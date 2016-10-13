#ifndef COMMHANDLER_H
#define COMMHANDLER_H
#pragma once

#include "Netcode\Client.h"
#include "MFCMessageHandler.h"
#include "unicodeConv.h"

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
	void GetLog();
	void GetPorts();

	int StartClient();
	int ShutdownClient();
	int RestartClient();
	int ResetClient();
	int PauseClient();
	int ResumeClient();

	void SetUserContinue();
	void SetModifiedData();

	void ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);


private:
	Client client;
	MFCMessageHandler messageHandler;

	// don't like these
	std::string localAddress;
	std::string localStartTime;
	std::string localMeasurementsMade;
	std::shared_ptr<const MachineSettings> localSettings;

	unsigned Handshake_req(http_request* r);
	unsigned Handshake_resp(http_response * r);
	unsigned GetMachineSettings_req(http_request * r);
	unsigned GetMachineSettings_resp(http_response * r);
	unsigned SetMachineSettings_req(http_request * r);
	unsigned SetMachineSettings_resp(http_response * r);
	unsigned GetData_req(http_request * r);
	unsigned GetData_resp(http_response * r);
};


#endif