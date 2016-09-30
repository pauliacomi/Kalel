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

	void ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	
	int StartClient();
	int ShutdownClient();
	int RestartClient();
	int ResetClient();
	int PauseClient();
	int ResumeClient();

	void SetUserContinue();
	void SetModifiedData();

	static void OutHandshake(http_request* r);
	static void InHandshake(http_response * r);

private:
	Client client;
	UnicodeConv unicodeConverter;
	MFCMessageHandler messageHandler;
};


#endif