#include "../stdafx.h"
#include "ThreadManager.h"

#include "../../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../../Kalel Shared/Com Classes/ManualActionParam.h"
#include "../../Kalel Shared/Resources/DefineInstruments.h"

#include "FileWriter.h"												// Class responsible for writing to files
#include "Class/Automation.h"										// Backend for all the automation
#include "Measurement/Measurement.h"								// Backend for measurements


// Netcode
#include "../../Kalel Shared/Netcode/Server.h"

// STD
#include <atomic>

// --------- Initialisation and destruction -------

ThreadManager::ThreadManager(Storage &h)
	: m_threadMainControlLoop(nullptr)
	, m_threadManualAction (nullptr)

	, maParam(nullptr)
	, experimentSettings(nullptr)

	, automation(nullptr)
{
	// 
	handles = &h;
}

ThreadManager::~ThreadManager()
{
}





//---------------------------------------------------------------------------
//
// --------- Thread start, pausing, resetting, resuming and shutdown --------
//
//---------------------------------------------------------------------------

HRESULT ThreadManager::StartThread() {

	HRESULT hr = S_OK;
	
	// Close the worker thread
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
		hr = S_FALSE;
	}

	return hr;
}

HRESULT ThreadManager::ResumeThread() {

	HRESULT hr = S_OK;

	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to start
		::SetEvent(automation->h_eventResume);
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}

HRESULT ThreadManager::PauseThread() {

	HRESULT hr = S_OK;

	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to resume
		::SetEvent(automation->h_eventPause);
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}


HRESULT ThreadManager::ResetThread()
{
	HRESULT hr = S_OK;

	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Signal the thread to reset
		::SetEvent(automation->h_eventReset);
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}


HRESULT ThreadManager::SetModifiedData()
{
	HRESULT hr = S_OK;

	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Set the atomic bool as modified
		automation->sb_settingsModified = true;
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}

HRESULT ThreadManager::SetUserContinue()
{
	HRESULT hr = S_OK;

	// Check if the thread exists
	if (m_threadMainControlLoop != nullptr)
	{
		// Set the atomic bool as modified
		automation->sb_userContinue = true;
	}
	else
	{
		hr = S_FALSE;
	}

	return hr;
}

// ShutdownThread function will check if thread is running and then send it the shutdown command
// If the thread does not quit in a short time it will be forcefully closed. Check if this is an error when using the function.
HRESULT ThreadManager::ShutdownThread()
{
	HRESULT hr = S_FALSE;

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
			hr = S_FALSE;
		}

		// Delete threads
		delete automation;
		automation = nullptr;
		delete m_threadMainControlLoop;
		m_threadMainControlLoop = nullptr;

		hr = S_OK;
	}

	return hr;
}







//--------------------------------------------------------------------
//
// --------- Thread start functions --------
//
//--------------------------------------------------------------------



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
	automation = new Automation(*handles);

	// Launch functionality
	automation->Execution();
}


//
// Manual actions
//
//

void ThreadManager::ManualAction()
{
	//start thread
	m_threadManualAction = AfxBeginThread(ThreadManualActionStarter, this);
}


UINT ThreadManager::ThreadManualActionStarter(LPVOID pParam)
{
	// Start the function from the main class, then check for validity
	ThreadManager* threadManager = static_cast<ThreadManager*>(pParam);

	// Launch required functionality
	threadManager->ThreadManualAction();

	// End thread
	return 0;
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
			actionSuccessful = pVanne.Ouvrir(localMP->instrumentNumber);
		else
			actionSuccessful = pVanne.Fermer(localMP->instrumentNumber);
		break;

	case INSTRUMENT_EV:
		if (localMP->shouldBeActivated)
			actionSuccessful = pVanne.ActiverEV(localMP->instrumentNumber);
		else
			actionSuccessful = pVanne.DesactiverEV(localMP->instrumentNumber);
		break;

	case INSTRUMENT_PUMP:
		if (localMP->shouldBeActivated)
			actionSuccessful = pVanne.ActiverPompe();
		else
			actionSuccessful = pVanne.DesactiverPompe();
		break;

	default:
		ASSERT(0); // Should never reach this
		break;
	}

	// Ask for the app to show the change, pass the locally created object which must be deleted there
	// ::PostMessage(localMP->windowHandle, UWM_UPDATEBUTTONS, (WPARAM)localMP, actionSuccessful);
}
