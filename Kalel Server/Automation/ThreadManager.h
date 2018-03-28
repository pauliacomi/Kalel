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
	bool StartMeasurement();
	bool ShutdownMeasurement();

	bool StartAutomation();
	bool PauseAutomation();
	bool ResumeAutomation();
	bool ShutdownAutomation();
	bool SetUserChoice(unsigned int choice);
	bool ChangeExperimentSettings();

	unsigned ThreadManualAction(int instrumentID, bool state);		// When a manual command is issued
	void ManualAction(int instrumentID, bool state);
	ControlInstrumentState GetInstrumentStates();
};