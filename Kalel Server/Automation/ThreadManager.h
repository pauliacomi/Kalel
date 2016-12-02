#ifndef THREADS_H
#define THREADS_H
#pragma once

#include <thread>
#include "../MessageHandler.h"

#include "../Backend/Wrapper Classes/Vannes.h"

class Automation;
class Measurement;
class ExperimentSettings;
class ManualActionParam;

class ThreadManager
{
public:
	ThreadManager(Storage &h);
	~ThreadManager();

private:
	// Pointer to settings storage
	ManualActionParam * maParam;							// Storage for manual parameters
	std::shared_ptr<ExperimentSettings> experimentSettings;	// Storage for automatic parameters

	CWinThread * m_threadMainControlLoop;					// Reference for main thread
	std::thread * m_threadMeasurement;						// Reference for main thread
	CWinThread * m_threadManualAction;						// Reference for manual thread

	Storage * handles;										// pointer to storage class
	Automation * automation;								// Main class that deals with the automatic functionality
	Measurement * measurement;								// Main class that deals with the measurement functionality

	//------------------------------------------------------------
	// Objects 
	//------------------------------------------------------------
	// Create the required objects
	CVannes pVanne;
	MessageHandler messageHandler;						// This class will send all the messages out

	// Threads
	std::thread measurementThread;						// c++11 thread for measurement


	// Public interface methods
public:
	unsigned StartMeasurement();
	HRESULT StartThread();
	HRESULT ResumeThread();
	HRESULT PauseThread();
	HRESULT ResetThread();
	HRESULT SetModifiedData();
	HRESULT SetUserContinue();
	HRESULT ShutdownThread();

	void ManualAction();									// When a manual command is issued

	// Private fields
private:
	static UINT ThreadMainWorkerStarter(LPVOID pParam);		// Main worker thread starter
	void ThreadMainWorker();								// Main worker thread function

	static UINT ThreadManualActionStarter(LPVOID pParam);	// Manual command thread starter
	void ThreadManualAction();								// Manual command thread worker
	
};

#endif