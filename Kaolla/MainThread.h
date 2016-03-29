#pragma once
#ifndef MAINTHREAD_H
#define MAINTHREAD_H

// MainThread, a custom implementation of the CWinThread class to be used for the application main thread

//// CURRENTLY UNUSED

class MainThread : public CWinThread
{
	DECLARE_DYNCREATE(MainThread)

protected:
	MainThread();           // protected constructor used by dynamic creation
	virtual ~MainThread();

public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	enum { Error, Running, Shutdown, Timeout };
	DWORD Wait();
	void ShutdownThread();

protected:
	DECLARE_MESSAGE_MAP()
	
	// Events 
	HANDLE ShutdownEvent;
	HANDLE PauseEvent;
};



#endif // !MAINTHREAD_H