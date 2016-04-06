
#include "stdafx.h"

#include "Kaolla.h"
#include "KaollaDoc.h"
#include "KaollaView.h"
#include "ThreadManager.h"

#include "ManualActionParam.h"

#include "Manip_AutoGaz.h"

#include "MainThread.h" // Custom thread event


// Pointers to the view and document
CKaollaView* pKaollaView;
CKaollaDoc* pKaollaDoc;

// Pointers for interfacing with the valves and temperature
CVannes* pVanne;
CTemperature* pTemperature;


HANDLE m_hStartMainThreadEvent;					// Event handle for giving the main thread the go-ahead. THIS SHOULD NOT BE HERE DUH

// Threads
CWinThread * m_threadManualAction;
CWinThread * m_threadLaunchExperiment;
CWinThread * m_threadStopExperiment;
CWinThread * m_threadSampleUnderVaccuum;
CWinThread * m_threadBottleUnderVaccuum;
CWinThread * m_threadChangeBottle;

// Main class that deals with the automatic functionality
CManip_AutoGaz manip;


// --------- Initialisation and destruction -------

ThreadManager::ThreadManager(LPVOID pParam)
	: m_threadMainControlLoop(NULL)
	, m_hShutdownEvent(::CreateEvent(NULL, TRUE, FALSE, NULL))
{
	// Create the required objects. Might be better to be done in the manip directly
	pVanne = new CVannes();
	pTemperature = new CTemperature(GetPortTemperatures());

	// Instantiate the handle of the event for giving the thread a go-ahead
	m_hStartMainThreadEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

	//Create the thread
	CreateMainThread(pParam);
}

ThreadManager::~ThreadManager()
{
	// close all valves
	pVanne->FermerToutesLesVannes();

	// signal the thread to exit
	ShutdownThread();
	CloseHandle(m_hShutdownEvent);
}


// --------- Thread creation and destruction -------

// A function that will create the main thread.
// This should not run if the thread is running, consider adding a check for it.
HRESULT ThreadManager::CreateMainThread(LPVOID pParam)
{
	HRESULT hr = S_OK;
	
	m_threadMainControlLoop = CreateThread(NULL, NULL, ThreadMainWorker, pParam, NULL, NULL);

	return hr;
}

HRESULT ThreadManager::SetStartEvent() {

	HRESULT hr = S_OK;

	if (::SetEvent(m_hStartMainThreadEvent) == FALSE) {
		hr = S_FALSE;
	}

	return hr;
}

// ShutdownThread function will check if thread is running and then send it the shutdown command
// If the thread does not quit in a short time it will be forcefully closed. Check if this is an error when using the function.
HRESULT ThreadManager::ShutdownThread()
{
	HRESULT hr = S_OK;

	// Close the worker thread
	if (NULL != m_threadMainControlLoop)
	{
		// Signal the thread to exit
		SetEvent(m_threadMainControlLoop);

		// thread may be suspended, so resume before shutting down
		ResumeThread(m_threadMainControlLoop);

		// Wait for the thread to exit. If it doesn't shut down
		// on its own, force it closed with Terminate thread
		if (WAIT_TIMEOUT == WaitForSingleObject(m_threadMainControlLoop, 1000))
		{
			TerminateThread(m_threadMainControlLoop, -1000);
			hr = S_FALSE;
		}

		// Close the handle and NULL it out
		CloseHandle(m_threadMainControlLoop);
		m_threadMainControlLoop = NULL;
	}

	// Reset the shutdown event
	ResetEvent(m_hShutdownEvent);

	return hr;
}


// --------- Thread functions -------

DWORD WINAPI ThreadManager::ThreadMainWorker(LPVOID pParam) // please redo in MFC
{
	// Get custom parameter class, then check for validity
	HWND maParam = static_cast<HWND>(pParam);
	ASSERT(maParam != NULL);
	bool actionSuccessful = false;

	// Create the class to deal with the automatic functionality
	CManip_AutoGaz manipAuto;
	manipAuto.SetKaollaView(pKaollaView);
	manipAuto.SetVannes(pVanne);
	manipAuto.SetTemperature(pTemperature);

	// Wait for the required event
	WaitForSingleObject(m_hStartMainThreadEvent, INFINITE);

	// Launch required functionality
	

	// Reset and end thread
	return 0;
}


// ----------------------- END -----------------------





// --------- Changing the instrument parameters -------

void ThreadManager::ChangementDev(int dev_vanne, int dev_temp)
{
	pVanne->SetDevNI_USB_6008(dev_vanne);
	pTemperature->SetDevNI_USB_9211A(dev_temp);
}


// --------- Functions to return experiment information -------

int ThreadManager::GetEtapeEnCours()
{
	return manip.etape_en_cours;
}


CString ThreadManager::GetDonneesExperience()
{
	return manip.GetDonneesExperience();
}



////// below this line is the old implementation

//void InitialisationManip()
//{
//	//manip = CManip_AutoGaz();
//	//manip.SetKaollaView(pKaollaView);
//	//manip.SetVannes(pVanne);
//	//manip.SetTemperature(pTemperature);
//
//	//manip.FermetureDeToutesLesVannes();
//	//manip.FermerLesValvesEtLaPompe();
//}

void ChangementDev(int dev_vanne, int dev_temp)
{
	pVanne->SetDevNI_USB_6008(dev_vanne);
	pTemperature->SetDevNI_USB_9211A(dev_temp);
}


// --------- Functions to return experiment information -------

int GetEtapeEnCours()
{
	return manip.etape_en_cours;
}


CString GetDonneesExperience()
{
	return manip.GetDonneesExperience();
}

// --------- Thread start functions -------

void ManualAction(LPVOID pParam)
{
	// lock the menu
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();

	//start thread
	m_threadManualAction = AfxBeginThread(ThreadManualAction, pParam);
}

void LancementThreads(LPVOID pParam)
{
	m_threadLaunchExperiment = AfxBeginThread(LancerThreadProc, pParam);
}

void ArretThreads(LPVOID pParam)
{
	m_threadStopExperiment = AfxBeginThread(ArretThreadProc, pParam);
}

void MiseSousVideAmpoule(LPVOID pParam)
{
	// lock the menu
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();
	
	//start thread
	m_threadSampleUnderVaccuum = AfxBeginThread(ThreadMenuMiseSousVideAmpoule, pParam);
}

void MiseSousVideBouteille(LPVOID pParam)
{
	// lock the menu
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();

	//start thread
	m_threadBottleUnderVaccuum = AfxBeginThread(ThreadMenuMiseSousVideBouteille, pParam);
}

void ChangementBouteille(LPVOID pParam)
{
	// lock the menu
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();

	//start thread
	m_threadChangeBottle = AfxBeginThread(ThreadMenuChangementBouteille, pParam);
}


/// -----------------Threads------------------

UINT ThreadManualAction(LPVOID pParam)													//return manualManip.EstOuvert_Vanne(num_vanne); <- these were returned as well in the same functions
{
	// Get custom parameter class, then check for validity
	ManualActionParam *maParam = static_cast<ManualActionParam*>(pParam);
	ASSERT(maParam != NULL);
	bool actionSuccessful = false;

	// Create the temporary class to deal with the command, can use pVanne directly but should be careful about any security machanisms
	CManip manualManip;
	manualManip.SetKaollaView(pKaollaView);
	manualManip.SetVannes(pVanne);
	manualManip.SetTemperature(pTemperature);

	// Launch required functionality
	switch (maParam->instrumentType)
	{
	case INSTRUMENT_VALVE:
		if(maParam->shouldBeActivated)
			actionSuccessful = manualManip.Ouverture_Vanne(maParam->instrumentNumber);
		else
			actionSuccessful = manualManip.Fermeture_Vanne(maParam->instrumentNumber);
	case INSTRUMENT_EV:
		if (maParam->shouldBeActivated)
			actionSuccessful = manualManip.ActiverEV(maParam->instrumentNumber);
		else
			actionSuccessful = manualManip.DesactiverEV(maParam->instrumentNumber);
	case INSTRUMENT_PUMP:
		if (maParam->shouldBeActivated)
			actionSuccessful = manualManip.ActiverPompe();
		else
			actionSuccessful = manualManip.DesactiverPompe();
	default:
		ASSERT(0); // Should never reach this
		break;
	}

	// Ask for the app to show the change
	::PostMessage(maParam->windowHandle, WM_UPDATEBUTTONS, (WPARAM)maParam, actionSuccessful);

	// Debug success
	if (actionSuccessful == false)
		return 404;
	return 0;
}

UINT LancerThreadProc(LPVOID pParam)
{
	// Get window handler and check for validity
	const HWND hMainFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hMainFrame != NULL);

	// Launch required functionality
	manip.LancementExperience(pParam);

	return 0;
}

UINT ArretThreadProc(LPVOID pParam)
{
	const HWND hMainFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hMainFrame != NULL);

	// Launch required functionality
	//manip.ArretExperience();

	return 0;
}

UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam)
{
	// Get window handler and check for validity
	const HWND hMainFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hMainFrame != NULL);

	// Launch required functionality
	//manip.MiseSousVideAmpoule();

	// When thread finishes, let main window know to unlock menu
	::PostMessage(hMainFrame, WM_THREADFINISHEDREG, NULL, NULL);

	return 0;
}


UINT ThreadMenuMiseSousVideBouteille(LPVOID pParam)
{
	// Get window handler and check for validity
	const HWND hMainFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hMainFrame != NULL);

	// Launch required functionality
	//manip.MiseSousVideBouteille();

	// When thread finishes, let main window know to unlock menu
	::PostMessage(hMainFrame, WM_THREADFINISHEDREG, NULL, NULL);

	return 0;
}


UINT ThreadMenuChangementBouteille(LPVOID pParam)
{
	// Get window handler and check for validity
	const HWND hMainFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hMainFrame != NULL);

	// Launch required functionality
	//manip.ChangementBouteille();

	// When thread finishes, let main window know to unlock menu
	::PostMessage(hMainFrame, WM_THREADFINISHEDREG, NULL, NULL);

	return 0;
}


// --------- Thread interaction / modification functions -------

void DemandeModificationExperience()
{
	manip.ModifierParametresExperience();
}

void FinAffichageMesure()
{
	manip.FinAffichage();
}

void PauseThreads()
{
	manip.DemandePause();
}

void ArretSousVideThreads()
{
	manip.DemandeMiseSousVide();
}

void ProchaineCommandeThreads()
{
	manip.ProchaineCommande();
}

void ProchaineDoseThreads()
{
	manip.ProchaineDose();
}

void ProchaineEtapeThreads()
{
	manip.ProchaineEtape();
} 

void RepriseThreads()
{
	manip.Reprise();
}

// --------- End ------------
