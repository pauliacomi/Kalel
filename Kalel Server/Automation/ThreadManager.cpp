#include "ThreadManager.h"

#include "../../Kalel Shared/Resources/DefineInstruments.h"

#include "Class/Automation.h"										// Backend for all the automation
#include "Measurement/Measurement.h"								// Backend for measurements
#include "CommonControls.h"
#include "CommonStorage.h"

// STD
#include <atomic>

// --------- Initialisation and destruction -------

ThreadManager::ThreadManager(Storage &h, Controls &c)
	: storage{ h }
	, controls { c }
{
}

ThreadManager::~ThreadManager()
{
	// Can be done externally, but double check
	ShutdownAutomation();
	ShutdownMeasurement();
}





//---------------------------------------------------------------------------
//
// --------- Measurement thread start and shutdown --------------------------
//
//---------------------------------------------------------------------------

unsigned ThreadManager::StartMeasurement() 
{
	if (measurement == nullptr)
	{
		// Create the class to deal with the manual functionality
		measurement.reset(new Measurement(storage, controls));

		// Launch measurement
		measurementThread = std::thread(&Measurement::Execution, measurement.get());

		return 0;
	}
	return 1;
}


unsigned ThreadManager::ShutdownMeasurement()
{
	// Close the worker thread
	if (measurement != nullptr)
	{
		// Signal the thread to exit
		measurement->measuring = false;

		// Join thread
		measurementThread.join();

		// Delete threads
		measurement.reset(nullptr);

		return 0;
	}
	return 1;
}


//--------------------------------------------------------------------------------------
//
// --------- Automation thread start, pausing, resetting, resuming and shutdown --------
//
//--------------------------------------------------------------------------------------


unsigned ThreadManager::StartAutomation() 
{
	// Check existence of the worker thread
	if (automation == nullptr)
	{
		// Create the class to deal with the automatic functionality
		automation.reset(new Automation(storage, controls));

		// Launch functionality
		automationThread = std::thread(&Automation::Execution, automation.get());

		return 0;
	}
	return 1;
}

unsigned ThreadManager::PauseAutomation()
{
	if (automation != nullptr)
	{
		// Signal the thread to resume
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventPause = true;
		storage.automationControl.notify_all();

		return 0;
	}
	return 1;
}

unsigned ThreadManager::ResumeAutomation() 
{
	if (automation != nullptr)
	{
		// Give the threads the start signal
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventResume = true;
		storage.automationControl.notify_all();

		return 0;
	}
	return 1;
}


unsigned ThreadManager::ResetAutomation()
{
	if (automation != nullptr)
	{
		// Signal the thread to reset
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventReset = true;
		storage.automationControl.notify_all();

		return 0;
	}
	return 1;
}


unsigned ThreadManager::SetUserChoice(unsigned int choice)
{
	if (automation != nullptr)
	{
		// Signal the atomic bool as modified
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->userChoice = choice;							// set choice
		automation->eventResume = true;								// then continue
		storage.automationControl.notify_all();

		return 0;
	}
	return 1;
}

// Shutdownfunction will check if thread is running and then send it the shutdown command
unsigned ThreadManager::ShutdownAutomation()
{
	// Close the worker thread
	if (automation != nullptr)
	{
		// Signal the thread to exit
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventShutdown = true;
		storage.automationControl.notify_all();

		// Unlock to continue function
		lk.unlock();

		// Join thread
		automationThread.join();

		// Delete thread
		automation.reset(nullptr);

		return 0;
	}
	return 1;
}


//--------------------------------------------------------------------------------------
//
// --------- Manual actions start ------------------------------------------------------
//
//--------------------------------------------------------------------------------------

unsigned ThreadManager::ThreadManualAction(int instrumentID, bool state)
{
	//start thread
	manualActionThread = std::thread(&ThreadManager::ManualAction, this, instrumentID, state);
	manualActionThread.detach();

	return 0;
}


void ThreadManager::ManualAction(int instrumentID, bool state)
{
	bool actionSuccessful = false;

	actionSuccessful = controls.instruments.ActuateController(instrumentID, state);

	// return
	if (actionSuccessful)
	{
		return;
	}
	else return;
}

ControlInstrumentState ThreadManager::GetInstrumentStates()
{
	ControlInstrumentState state;

	for (auto i : controls.instruments.controllers)
	{
		state[i.first] = i.second.readerfunction();	
	}
	return state;
}
