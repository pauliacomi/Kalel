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
	void GetData();
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
	UnicodeConv unicodeConverter;
	MFCMessageHandler messageHandler;
	std::string localAddress;

	std::shared_ptr<const MachineSettings> localSettings;

	void Handshake_req(http_request* r);
	void Handshake_resp(http_response * r);
	void GetMachineSettings_req(http_request * r);
	void GetMachineSettings_resp(http_response * r);
	void SetMachineSettings_req(http_request * r);
	void SetMachineSettings_resp(http_response * r);
};


#endif