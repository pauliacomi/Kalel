#include "ThreadManager.h"

#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/log.h"

#include "Class/Automation.h"										// Backend for all the automation
#include "Measurement/Measurement.h"								// Backend for measurements
#include "CommonControls.h"
#include "CommonStorage.h"

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

bool ThreadManager::StartMeasurement() 
{
	if (measurement == nullptr)
	{
		// Create the class to deal with the manual functionality
		measurement.reset(new Measurement(storage, controls));

		// Launch measurement
		measurementThread = std::thread(&Measurement::Execution, measurement.get());

		return true;
	}
	return false;
}


bool ThreadManager::ShutdownMeasurement()
{
	// Close the worker thread
	if (measurement != nullptr)
	{
		// Signal the thread to exit
		std::unique_lock<std::mutex> lk(storage.measurementMutex);
		measurement->eventShutdown = true;
		storage.measurementControl.notify_all();

		// Unlock to continue function
		lk.unlock();

		// Join thread
		measurementThread.join();

		// Delete threads
		measurement.reset(nullptr);

		return true;
	}
	return false;
}


//--------------------------------------------------------------------------------------
//
// --------- Automation thread start, pausing, resetting, resuming and shutdown --------
//
//--------------------------------------------------------------------------------------


bool ThreadManager::StartAutomation()
{
	// Check existence of the worker thread
	if (automation == nullptr)
	{
		// Create the class to deal with the automatic functionality
		automation.reset(new Automation(storage, controls));

		// Launch functionality
		automationThread = std::thread(&Automation::Execution, automation.get());

		return true;
	}
	return false;
}

// Shutdownfunction will check if thread is running and then send it the shutdown command
bool ThreadManager::ShutdownAutomation()
{
	// Close the worker thread
	if (automation != nullptr)
	{
		// Signal the thread to exit
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->shutdownReason = Stop::Complete;
		automation->eventShutdown = true;
		storage.automationControl.notify_all();

		// Unlock to continue function
		lk.unlock();

		// Join thread
		automationThread.join();

		// Delete thread
		automation.reset(nullptr);

		return true;
	}
	return false;
}

bool ThreadManager::PauseAutomation()
{
	if (automation != nullptr)
	{
		// Signal the thread to resume
		std::unique_lock<std::mutex> lk(storage.automationMutex);
		automation->eventPause = true;
		storage.automationControl.notify_all();

		return true;
	}
	return false;
}

bool ThreadManager::ResumeAutomation()
{
	if (automation != nullptr)
	{
		// Give the threads the start signal
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventResume = true;
		storage.automationControl.notify_all();

		return true;
	}
	return false;
}


// Will set the flag to turn off the experiment as cancelled
bool ThreadManager::StopExperiment()
{
	// Close the worker thread
	if (automation != nullptr)
	{
		// Signal the thread to exit
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->shutdownReason = Stop::Cancel;
		automation->eventShutdown = true;
		storage.automationControl.notify_all();

		return true;
	}
	return false;
}

bool ThreadManager::ChangeExperimentSettings()
{
	if (automation != nullptr)
	{
		// Signal the thread to reset
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->eventChangeExpSett = true;
		storage.automationControl.notify_all();

		return true;
	}
	return false;
}


bool ThreadManager::SetUserChoice(unsigned int choice)
{
	if (automation != nullptr)
	{
		// Signal the atomic bool as modified
		std::unique_lock<std::mutex> lk(storage.automationMutex);	// mutex for thread notification
		automation->userChoice = choice;							// set choice
		automation->eventResume = true;								// then continue
		storage.automationControl.notify_all();

		return true;
	}
	return false;
}

bool ThreadManager::NextStageAutomation()
{
	if (automation != nullptr)
	{
		++storage.experimentStatus.mainStage;											// Set next stage
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;						// Reset next step
		storage.experimentStatus.isWaiting = false;
		return true;
	}
	return false;
}

bool ThreadManager::NextStepAutomation()
{
	if (automation != nullptr)
	{
		if (storage.experimentStatus.mainStage == STAGE_AUTO_ADSORPTION ||
			storage.experimentStatus.mainStage == STAGE_AUTO_DESORPTION)
		{
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
			storage.experimentStatus.isWaiting = false;
		}
		return true;
	}
	return false;
}

bool ThreadManager::NextDoseAutomation()
{
	if (automation != nullptr)
	{
		if (storage.experimentStatus.mainStage == STAGE_AUTO_ADSORPTION || 
			storage.experimentStatus.mainStage == STAGE_AUTO_DESORPTION)
		{
			if (storage.experimentStatus.stepStatus == STEP_STATUS_INPROGRESS)
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
				storage.experimentStatus.isWaiting = false;
				return true;
			}
		}
	}
	return false;
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
	if (state)
		LOG(logINFO) << "Manually activated instrument with ID " << instrumentID;
	else	
		LOG(logINFO) << "Manually deactivated instrument with ID " << instrumentID;

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
