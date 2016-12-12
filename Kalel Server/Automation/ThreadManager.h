#ifndef THREADS_H
#define THREADS_H
#pragma once

#include "CommonControls.h"
#include "../../Kalel Shared/Com Classes/ControlInstrumentState.h"

#include <thread>

class Automation;
class Measurement;

class ThreadManager
{
public:
	ThreadManager(Storage &h);
	~ThreadManager();

private:
	// Threads
	CWinThread * m_threadMainControlLoop;					// Reference for main thread
	std::thread manualActionThread;							// c++11 thread for manual actions
	std::thread measurementThread;							// c++11 thread for measurement

	// Thread object pointers
	Automation * automation;								// Main class that deals with the automatic functionality
	Measurement * measurement;								// Main class that deals with the measurement functionality

	// Storage/controls
	Storage & storage;										// reference to storage class
	Controls controls;

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

	unsigned ThreadManualAction(int instrumentType, int instrumentNumber, bool state);		// When a manual command is issued
	void ManualAction(int instrumentType, int instrumentNumber, bool state);
	ControlInstrumentState GetInstrumentStates();

	// Private functions
private:
	static UINT ThreadMainWorkerStarter(LPVOID pParam);		// Main worker thread starter
	void ThreadMainWorker();								// Main worker thread function
};

#endif