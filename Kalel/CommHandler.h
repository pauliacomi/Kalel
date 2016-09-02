#ifndef COMMHANDLER_H
#define COMMHANDLER_H
#pragma once

#include "Netcode\Client.h"
#include "unicodeConv.h"

#include <string>

class CommHandler
{
public:
	CommHandler();
	~CommHandler();

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

private:
	Client client;
	UnicodeConv unicodeConverter;
};

#endif