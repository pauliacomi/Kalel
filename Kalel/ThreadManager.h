#pragma once
#ifndef THREADS_H
#define THREADS_H

#include "afxwin.h"
#include <afxmt.h>

#include "ExperimentSettings.h"
#include "Automation.h"				// Backend for all the automation
#include "ManualActionParam.h"


class ThreadManager
{
public:
	ThreadManager(ExperimentSettings * expD);
	~ThreadManager();

	// Pointer to settings storage
	ManualActionParam * maParam;							// Storage for manual parameters
	ExperimentSettings * experimentSettings;				// Storage for automatic parameters

	CWinThread * m_threadMainControlLoop;					// Reference for main thread
	CWinThread * m_threadManualAction;						// Reference for manual thread

private:
	Automation * automation;								// Main class that deals with the automatic functionality
	CVannes * pVanne;										// Pointers for interfacing with the valves and temperature

	// Public interface methods
public:
	HRESULT ResumeThread();
	HRESULT PauseThread();
	HRESULT ResetThread();
	HRESULT SetModifiedData();
	HRESULT ShutdownThread();

	void ChangementDev(int dev_vanne, int dev_temp);
	void ManualAction();									// When a manual command is issued

	// Private fields
private:
	static UINT ThreadMainWorkerStarter(LPVOID pParam);		// Main worker thread starter
	void ThreadMainWorker();								// Main worker thread function

	static UINT ThreadManualActionStarter(LPVOID pParam);	// Manual command thread starter
	void ThreadManualAction();								// Manual command thread worker
	
};

#endif