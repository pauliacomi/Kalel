#include "../stdafx.h"
#include "ThreadManager.h"

#include "../../Kalel Shared/Resources/DefineInstruments.h"

#include "Class/Automation.h"										// Backend for all the automation
#include "Measurement/Measurement.h"								// Backend for measurements
#include "../../Kalel Shared/Com Classes/ManualActionParam.h"		// Manual command struct details


// Netcode
#include "../../Kalel Shared/Netcode/Server.h"

// STD
#include <atomic>

// --------- Initialisation and destruction -------

ThreadManager::ThreadManager(Storage &h)
	: storage{ h }

	, m_threadMainControlLoop(nullptr)
	, m_threadManualAction (nullptr)

	, automation(nullptr)
	, measurement(nullptr)
	
	, maParam(nullptr)
{
	// Create objects from controls class
	controls.fileWriter = std::make_shared<FileWriter>();
	controls.messageHandler = std::make_shared<MessageHandler>(h);
	controls.valveControls = std::make_shared<ValveController>(*controls.messageHandler);
}

ThreadManager::~ThreadManager()
{
}





//---------------------------------------------------------------------------
//
// --------- Thread start, pausing, resetting, resuming and shutdown --------
//
//---------------------------------------------------------------------------

unsigned ThreadManager::StartMeasurement() 
{
	if (measurement != nullptr)
	{
		measurement = new Measurement(storage, controls);
		measurementThread = std::thread(&Measurement::Execution, measurement);
	}
	else
	{
		return 1;
	}
	return 0;
}

unsigned ThreadManager::StartAutomation() 
{
	// Check existence of the worker thread
	if (m_threadMainControlLoop == nullptr)
	{

		// Create the thread in a suspended state
		m_threadMainControlLoop = AfxBeginThread(ThreadMainWorkerStarter, this, NULL, NULL, CREATE_SUSPENDED, NULL);

		// Make sure thread is not accidentally deleted
		m_threadMainControlLoop->m_bAutoDelete = FALSE;

		// Now resume the thread
		m_threadMainControlLoop->ResumeThread();

	}
	else
	{
		return 1;
	}
	return 0;
}

unsigned ThreadManager::ResumeAutomation() 
{
	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to start
		::SetEvent(automation->h_eventResume);
	}
	else
	{
		return 1;
	}
	return 0;
}

unsigned ThreadManager::PauseAutomation() 
{
	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to resume
		::SetEvent(automation->h_eventPause);
	}
	else
	{
		return 1;
	}
	return 0;
}


unsigned ThreadManager::ResetAutomation()
{
	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to reset
		::SetEvent(automation->h_eventReset);
	}
	else
	{
		return 1;
	}
	return 0;
}


unsigned ThreadManager::SetModifiedData()
{
	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Set the atomic bool as modified
		automation->sb_settingsModified = true;
	}
	else
	{
		return 1;
	}
	return 0;
}

unsigned ThreadManager::SetUserContinue()
{
	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Set the atomic bool as modified
		automation->sb_userContinue = true;
	}
	else
	{
		return 1;
	}
	return 0;
}

// ShutdownThread function will check if thread is running and then send it the shutdown command
// If the thread does not quit in a short time it will be forcefully closed. Check if this is an error when using the function.
unsigned ThreadManager::ShutdownAutomation()
{
	unsigned hr = 1;

	// Close the worker thread
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to exit
		::SetEvent(automation->h_eventShutdown);

		// thread may be suspended, so resume before shutting down
		::ResumeThread(m_threadMainControlLoop);

		// Wait for the thread to exit. If it doesn't shut down on its own, throw an error
		if (WaitForSingleObject(m_threadMainControlLoop->m_hThread, INFINITE) == WAIT_TIMEOUT)
		{
			hr = 1;
		}

		// Delete threads
		delete automation;
		automation = nullptr;
		delete m_threadMainControlLoop;
		m_threadMainControlLoop = nullptr;

		hr = 0;
	}

	return hr;
}




void ThreadManager::ThreadManualAction()
{
	bool actionSuccessful = false;

	// Check for validity
	ASSERT(maParam != NULL);

	// Create local copy so we don't have to depend on main one which can get deleted
	ManualActionParam * localMP = new ManualActionParam(maParam);


	// Launch required functionality
	switch (localMP->instrumentType)
	{
	case INSTRUMENT_VALVE:
		if (localMP->shouldBeActivated)
			actionSuccessful = controls.valveControls->ValveOpen(localMP->instrumentNumber, false);
		else
			actionSuccessful = controls.valveControls->ValveClose(localMP->instrumentNumber, false);
		break;

	case INSTRUMENT_EV:
		if (localMP->shouldBeActivated)
			actionSuccessful = controls.valveControls->EVActivate(localMP->instrumentNumber, false);
		else
			actionSuccessful = controls.valveControls->EVDeactivate(localMP->instrumentNumber, false);
		break;

	case INSTRUMENT_PUMP:
		if (localMP->shouldBeActivated)
			actionSuccessful = controls.valveControls->PumpActivate(false);
		else
			actionSuccessful = controls.valveControls->PumpDeactivate(false);
		break;

	default:
		ASSERT(0); // Should never reach this
		break;
	}

	// Ask for the app to show the change, pass the locally created object which must be deleted there
	// ::PostMessage(localMP->windowHandle, UWM_UPDATEBUTTONS, (WPARAM)localMP, actionSuccessful);
}





//--------------------------------------------------------------------
//
// --------- Thread start functions --------
//
//--------------------------------------------------------------------

//
// Automation actions
//

UINT ThreadManager::ThreadMainWorkerStarter(LPVOID pParam)
{
	// Start the function from the main class, then check for validity
	ThreadManager* maParam = static_cast<ThreadManager*>(pParam);

	// Launch required functionality
	maParam->ThreadMainWorker();

	// Reset and end thread
	return 0;
}

void ThreadManager::ThreadMainWorker()
{
	// Create the class to deal with the automatic functionality
	automation = new Automation(storage, controls);

	// Launch functionality
	automation->Execution();
}


//
// Manual actions
//

UINT ThreadManager::ThreadManualActionStarter(LPVOID pParam)
{
	// Start the function from the main class, then check for validity
	ThreadManager* threadManager = static_cast<ThreadManager*>(pParam);

	// Launch required functionality
	threadManager->ThreadManualAction();

	// End thread
	return 0;
}

unsigned ThreadManager::ManualAction()
{
	//start thread
	m_threadManualAction = AfxBeginThread(ThreadManualActionStarter, this);

	return 0;
}
