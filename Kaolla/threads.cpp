
#include "stdafx.h"

#include "Kaolla.h"
#include "KaollaDoc.h"
#include "KaollaView.h"
#include "threads.h"

#include "ManualActionParam.h"

#include "Manip_AutoGaz.h"

// Pointers to the view and document
CKaollaView* pKaollaView;
CKaollaDoc* pKaollaDoc;

// Pointers for interfacing with the valves and temperature
CVannes* pVanne;
CTemperature* pTemperature;

// Events for the automatic functionality
// -

// Threads
CWinThread * m_threadManualAction;
CWinThread * m_threadLaunchExperiment;
CWinThread * m_threadStopExperiment;
CWinThread * m_threadSampleUnderVaccuum;
CWinThread * m_threadBottleUnderVaccuum;
CWinThread * m_threadChangeBottle;

// Main class that deals with the automatic functionality
CManip_AutoGaz manip;
CManip manualManip;


// --------- Initialisation and destruction (kind of) THIS NEEDS TO BE PERFORMED LOCALLY WITHIN THREADS FOR EACH manip OBJECT -------

void InitialisationManip()
{
	//pVanne = new CVannes();
	//pTemperature = new CTemperature(GetPortTemperatures());

	//pKaollaView = CKaollaView::GetView();
	//pKaollaDoc = CKaollaDoc::GetDocument();

	//manip = CManip_AutoGaz();
	//manip.SetKaollaView(pKaollaView);
	//manip.SetVannes(pVanne);
	//manip.SetTemperature(pTemperature);

	//manip.FermetureDeToutesLesVannes();
	//manip.FermerLesValvesEtLaPompe();
}

void InitializeObjects()
{
	pVanne = new CVannes();
	pTemperature = new CTemperature(GetPortTemperatures());

	pKaollaView = CKaollaView::GetView();
	pKaollaDoc = CKaollaDoc::GetDocument();

	manualManip.SetKaollaView(pKaollaView);
	manualManip.SetVannes(pVanne);
	manualManip.SetTemperature(pTemperature);

	manualManip.FermetureDeToutesLesVannes();
	manualManip.FermerLesValvesEtLaPompe();
}

void DeleteManip()
{
	pVanne->FermerToutesLesVannes();
}


// --------- Changing the instrument parameters -------

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
	//manip.LancementExperience(pParam);

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
