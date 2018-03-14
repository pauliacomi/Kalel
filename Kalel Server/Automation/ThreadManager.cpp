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
	}
	else
	{
		return 1;
	}
	return 0;
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
	}
	return 0;
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
	}
	else
	{
		return 1;
	}
	return 0;
}

unsigned ThreadManager::ResumeAutomation() 
{
	if (automation != nullptr)
	{
		// Give the threads the start signal
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventResume = true;
		storage.automationControl.notify_all();
	}
	else
	{
		return 1;
	}
	return 0;
}

unsigned ThreadManager::PauseAutomation() 
{
	if (automation != nullptr)
	{
		// Signal the thread to resume
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventPause = true;
		storage.automationControl.notify_all();
	}
	else
	{
		return 1;
	}
	return 0;
}


unsigned ThreadManager::ResetAutomation()
{
	if (automation != nullptr)
	{
		// Signal the thread to reset
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventReset = true;
		storage.automationControl.notify_all();
	}
	else
	{
		return 1;
	}
	return 0;
}


unsigned ThreadManager::SetUserContinue()
{
	if (automation != nullptr)
	{
		// Signal the atomic bool as modified
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->sb_userContinue = true;
		storage.automationControl.notify_all();
	}
	else
	{
		return 1;
	}
	return 0;
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

	}
	return 0;
}


//--------------------------------------------------------------------------------------
//
// --------- Manual actions start ------------------------------------------------------
//
//--------------------------------------------------------------------------------------

unsigned ThreadManager::ThreadManualAction(int instrumentType, int instrumentNumber, bool state)
{
	//start thread
	manualActionThread = std::thread(&ThreadManager::ManualAction, this, instrumentType, instrumentNumber, state);
	manualActionThread.detach();

	return 0;
}


void ThreadManager::ManualAction(int instrumentType, int instrumentNumber, bool state)
{
	bool actionSuccessful = false;

	actionSuccessful = controls.instruments.ActuateController(instrumentType + instrumentNumber, state);

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
		switch (i.second.type)
		{
		case CONTROLLER_VALVE:
			state.valves[i.second.identifier -1] = i.second.readerfunction();
			break;
		case CONTROLLER_EV:
			state.EVs[i.second.identifier - 1] = i.second.readerfunction();
			break;
		case CONTROLLER_PUMP:
			state.pumps[i.second.identifier - 1] = i.second.readerfunction();
			break;
		default:
			break;
		}
	
	}
	return state;
}
