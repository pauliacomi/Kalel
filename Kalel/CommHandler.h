#pragma once
class CommHandler
{
public:
	CommHandler();
	~CommHandler();

	void ManualCommand(HWND hwid, int instrumentType, int instrumentNumber, bool shouldBeActivated);
	
	LRESULT StartClient();
	LRESULT ShutdownClient();
	LRESULT RestartClient();
	LRESULT ResetClient();
	LRESULT PauseClient();
	LRESULT ResumeClient();

	void SetUserContinue();
	void SetModifiedData();
};

