#ifndef COMMHANDLER_H
#define COMMHANDLER_H
#pragma once

class CommHandler
{
public:
	CommHandler();
	~CommHandler();

	void ManualCommand(int instrumentType, int instrumentNumber, bool shouldBeActivated);
	
	int StartClient();
	int ShutdownClient();
	int RestartClient();
	int ResetClient();
	int PauseClient();
	int ResumeClient();

	void SetUserContinue();
	void SetModifiedData();
};

#endif