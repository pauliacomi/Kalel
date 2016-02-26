
#include "stdafx.h"

#include "Kaolla.h"
#include "KaollaDoc.h"

#include "KaollaView.h"

#include "threads.h"

#include "Manip_AutoGaz.h"


CKaollaView* pKaollaView;
CKaollaDoc* pKaollaDoc;

CVannes* pVanne;
CTemperature* pTemperature;

CEvent g_eventStartManip; 
CEvent g_eventStopManip; 
CEvent g_eventStartMiseSousVideAmpoule;
CEvent g_eventStartMiseSousVideBouteille;
CEvent g_eventStartChangementBouteille;
///CEvent g_eventStopMiseSousVideAmpoule;


CManip_AutoGaz manip;


void InitialisationManip()
{
	//pVanne = new CVannes();
	//pTemperature = new CTemperature(GetPortTemperatures());
	pKaollaView = CKaollaView::GetView();
	pKaollaDoc = CKaollaDoc::GetDocument();

	//manip = CManip_AutoGaz();
	//manip.SetKaollaView(pKaollaView);
	//manip.SetVannes(pVanne);
	//manip.SetTemperature(pTemperature);

	//manip.FermetureDeToutesLesVannes();
	//manip.FermerLesValvesEtLaPompe();
}

void DeleteManip()
{
	//pVanne->FermerToutesLesVannes();
}

void ChangementDev(int dev_vanne, int dev_temp)
{
	//pVanne->SetDevNI_USB_6008(dev_vanne);
	//pTemperature->SetDevNI_USB_9211A(dev_temp);
}

UINT LancerThreadProc(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartManip, INFINITE);

	//manip.LancementExperience(pParam);

	return 0;
}


UINT ArretThreadProc(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStopManip, INFINITE);

	//manip.ArretExperience();

	return 0;
}


int GetEtapeEnCours()
{
	return 1;//manip.etape_en_cours;
}


CString GetDonneesExperience()
{
	return "";// manip.GetDonneesExperience();
}

// --------- Menus ----------------------


void MiseSousVideAmpoule()
{
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();
	g_eventStartMiseSousVideAmpoule.SetEvent();
//	::WaitForSingleObject(g_eventStopMiseSousVideAmpoule, INFINITE);
}


UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartMiseSousVideAmpoule, INFINITE);

	//manip.MiseSousVideAmpoule();
	
	pKaollaDoc->experiment_running = FALSE;
	pKaollaView->DebloqueMenu();

	pKaollaView->LancementThreadMSVAmpoule();

	//g_eventStopMiseSousVideAmpoule.SetEvent();

	return 0;
}

void MiseSousVideBouteille()
{
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();
	g_eventStartMiseSousVideBouteille.SetEvent();
//	::WaitForSingleObject(g_eventStopMiseSousVideAmpoule, INFINITE);
}


UINT ThreadMenuMiseSousVideBouteille(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartMiseSousVideBouteille, INFINITE);

	//manip.MiseSousVideBouteille();
	
	pKaollaDoc->experiment_running = FALSE;
	pKaollaView->DebloqueMenu();

	pKaollaView->LancementThreadMSVBouteille();

	//g_eventStopMiseSousVideAmpoule.SetEvent();

	return 0;
}

void ChangementBouteille()
{
	pKaollaDoc->experiment_running = TRUE;
	pKaollaView->DebloqueMenu();
	g_eventStartChangementBouteille.SetEvent();
//	::WaitForSingleObject(g_eventStopMiseSousVideAmpoule, INFINITE);
}


UINT ThreadMenuChangementBouteille(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartChangementBouteille, INFINITE);

	//manip.ChangementBouteille();
	
	pKaollaDoc->experiment_running = FALSE;
	pKaollaView->DebloqueMenu();

	pKaollaView->LancementThreadChangementBouteille();

	//g_eventStopMiseSousVideAmpoule.SetEvent();

	return 0;
}


// --------- Boutons --------------------

void DemandeModificationExperience()
{
	//manip.ModifierParametresExperience();
}

// Boutons commandes de la manip

void LancementThreads()
{
	g_eventStartManip.SetEvent();
}

void ArretThreads()
{
	g_eventStopManip.SetEvent();
}

void FinAffichageMesure()
{
	//manip.FinAffichage();
}


void PauseThreads()
{
	//manip.DemandePause();
}

void ArretSousVideThreads()
{
	//manip.DemandeMiseSousVide();
}

void ProchaineCommandeThreads()
{
	//manip.ProchaineCommande();
}

void ProchaineDoseThreads()
{
	//manip.ProchaineDose();
}

void ProchaineEtapeThreads()
{
	//manip.ProchaineEtape();
} 

void RepriseThreads()
{
	//manip.Reprise();
}


// Boutons des vannes


bool DemandeOuvertureVanne(int num_vanne)
{
	//manip.Ouverture_Vanne(num_vanne);
	return true;// manip.EstOuvert_Vanne(num_vanne);
}

bool DemandeFermetureVanne(int num_vanne)
{
	//manip.Fermeture_Vanne(num_vanne);
	return true;// manip.EstFerme_Vanne(num_vanne);
}

bool DemandeActivationEV1()
{
	//manip.ActiverEV1();
	return true;// manip.EV1EstActive();
}

bool DemandeDesactivationEV1()
{
	//manip.DesactiverEV1();
	return true;// manip.EV1EstDesactive();
}

bool DemandeActivationEV2()
{
	//manip.ActiverEV2();
	return true;// manip.EV2EstActive();
}

bool DemandeDesactivationEV2()
{
	//manip.DesactiverEV2();
	return true;// manip.EV2EstDesactive();
}

bool DemandeActivationPompe()
{
	//manip.ActiverPompe();
	return true;// manip.PompeEstActive();
}

bool DemandeDesactivationPompe()
{
	//manip.DesactiverPompe();
	return true;// manip.PompeEstDesactive();
}


// ----------- Fin des Boutons ------------
