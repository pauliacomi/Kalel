
#include "stdafx.h"

#include "Kaolla.h"
#include "KaollaDoc.h"

#include "KaollaView.h"

#include "threads.h"

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
CWinThread * m_threadLaunchExperiment;
CWinThread * m_threadStopExperiment;
CWinThread * m_threadSampleUnderVaccuum;
CWinThread * m_threadBottleUnderVaccuum;
CWinThread * m_threadChangeBottle;

// Main class that deals with the automatic functionality
CManip_AutoGaz manip;



// --------- Initialisation and destruction (kind of) THIS NEEDS TO BE PERFORMED LOCALLY WITHIN THREADS FOR EACH manip OBJECT -------

void InitialisationManip()
{
	pVanne = new CVannes();
	pTemperature = new CTemperature(GetPortTemperatures());

	pKaollaView = CKaollaView::GetView();
	pKaollaDoc = CKaollaDoc::GetDocument();

	manip = CManip_AutoGaz();
	manip.SetKaollaView(pKaollaView);
	manip.SetVannes(pVanne);
	manip.SetTemperature(pTemperature);

	manip.FermetureDeToutesLesVannes();
	manip.FermerLesValvesEtLaPompe();
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
	manip.ArretExperience();

	return 0;
}

UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam)
{
	// Get window handler and check for validity
	const HWND hMainFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hMainFrame != NULL);

	// Launch required functionality
	manip.MiseSousVideAmpoule();

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
	manip.MiseSousVideBouteille();

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
	manip.ChangementBouteille();

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


// --------- Direct instrument manipulation ----------------

bool DemandeOuvertureVanne(int num_vanne)
{
	manip.Ouverture_Vanne(num_vanne);
	return manip.EstOuvert_Vanne(num_vanne);
}

bool DemandeFermetureVanne(int num_vanne)
{
	manip.Fermeture_Vanne(num_vanne);
	return manip.EstFerme_Vanne(num_vanne);
}

bool DemandeActivationEV1()
{
	manip.ActiverEV1();
	return manip.EV1EstActive();
}

bool DemandeDesactivationEV1()
{
	manip.DesactiverEV1();
	return manip.EV1EstDesactive();
}

bool DemandeActivationEV2()
{
	manip.ActiverEV2();
	return manip.EV2EstActive();
}

bool DemandeDesactivationEV2()
{
	manip.DesactiverEV2();
	return manip.EV2EstDesactive();
}

bool DemandeActivationPompe()
{
	manip.ActiverPompe();
	return manip.PompeEstActive();
}

bool DemandeDesactivationPompe()
{
	manip.DesactiverPompe();
	return manip.PompeEstDesactive();
}


// --------- End ------------
