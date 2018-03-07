#pragma once

#include "../../Kalel Shared/Com Classes/ControlInstrumentState.h"

#include <thread>

class Automation;
class Measurement;
class Storage;
class Controls;

class ThreadManager
{
public:
	ThreadManager(Storage &h, Controls & c);
	~ThreadManager();

private:
	// Threads
	std::thread manualActionThread;							// c++11 thread for manual actions
	std::thread measurementThread;							// c++11 thread for measurement
	std::thread automationThread;							// c++11 thread for automation

	// Thread object pointers
	std::unique_ptr<Automation> automation;					// Main class that deals with the automatic functionality
	std::unique_ptr<Measurement> measurement;				// Main class that deals with the measurement functionality

	// Storage/controls
	Storage & storage;										// reference to storage class
	Controls & controls;									// reference to controls class

	// Public interface methods
public:
	unsigned StartMeasurement();
	unsigned ShutdownMeasurement();

	unsigned StartAutomation();
	unsigned ResumeAutomation();
	unsigned PauseAutomation();
	unsigned ResetAutomation();
	unsigned SetUserContinue();
	unsigned ShutdownAutomation();

	unsigned ThreadManualAction(int instrumentType, int instrumentNumber, bool state);		// When a manual command is issued
	void ManualAction(int instrumentType, int instrumentNumber, bool state);
	ControlInstrumentState GetInstrumentStates();
};