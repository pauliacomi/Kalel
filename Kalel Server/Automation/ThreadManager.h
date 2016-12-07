#ifndef THREADS_H
#define THREADS_H
#pragma once

#include "CommonControls.h"

#include <thread>

class Automation;
class Measurement;
class ManualActionParam;

class ThreadManager
{
public:
	ThreadManager(Storage &h);
	~ThreadManager();

private:
	// Threads
	CWinThread * m_threadMainControlLoop;					// Reference for main thread
	CWinThread * m_threadManualAction;						// Reference for manual thread
	std::thread measurementThread;							// c++11 thread for measurement

	// Thread object pointers
	Automation * automation;								// Main class that deals with the automatic functionality
	Measurement * measurement;								// Main class that deals with the measurement functionality

	// Storage/controls
	Storage & storage;										// reference to storage class
	Controls controls;
	ManualActionParam * maParam;							// Storage for manual parameters



	// Public interface methods
public:
	unsigned StartMeasurement();


	unsigned StartAutomation();
	unsigned ResumeAutomation();
	unsigned PauseAutomation();
	unsigned ResetAutomation();
	unsigned SetModifiedData();
	unsigned SetUserContinue();
	unsigned ShutdownAutomation();

	unsigned ManualAction();									// When a manual command is issued


	// Private functions
private:
	static UINT ThreadMainWorkerStarter(LPVOID pParam);		// Main worker thread starter
	void ThreadMainWorker();								// Main worker thread function

	static UINT ThreadManualActionStarter(LPVOID pParam);	// Manual command thread starter
	void ThreadManualAction();								// Manual command thread worker
	
};

#endif