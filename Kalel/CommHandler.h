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

	void Handshake_req(http_request* r);
	void Handshake_resp(http_response * r);
};


#endif