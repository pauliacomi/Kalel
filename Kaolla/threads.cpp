
#include "stdafx.h"

#include "Kaolla.h"
#include "KaollaDoc.h"

#include "KaollaView.h"

#include "threads.h"

#include "Manip_AutoGaz.h"


CKaollaView* pKaollaView;

CVannes* pVanne;
CTemperature* pTemperature;

CEvent g_eventStartManip; 
CEvent g_eventStopManip; 
CEvent g_eventStartMiseSousVideAmpoule;
CEvent g_eventStartMiseSousVideBouteille;
CEvent g_eventStartChangementBouteille;
//CEvent g_eventStopMiseSousVideAmpoule;


CManip_AutoGaz manip;


void InitialisationManip()
{
	pVanne = new CVannes();
	pTemperature = new CTemperature(GetPortTemperatures());
	pKaollaView = GetKaollaView();

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

void ChangementDev(int dev_vanne, int dev_temp)
{
	pVanne->SetDevNI_USB_6008(dev_vanne);
	pTemperature->SetDevNI_USB_9211A(dev_temp);
}

UINT LancerThreadProc(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartManip, INFINITE);

	manip.LancementExperience(pParam);

	return 0;
}


UINT ArretThreadProc(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStopManip, INFINITE);

	manip.ArretExperience();

	return 0;
}


int GetEtapeEnCours()
{
	return manip.etape_en_cours;
}


CString GetDonneesExperience()
{
	return manip.GetDonneesExperience();
}

// --------- Menus ----------------------


void MiseSousVideAmpoule()
{
	pKaollaView->fin_experience = false;
	pKaollaView->DebloqueMenu();
	g_eventStartMiseSousVideAmpoule.SetEvent();
//	::WaitForSingleObject(g_eventStopMiseSousVideAmpoule, INFINITE);
}


UINT ThreadMenuMiseSousVideAmpoule(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartMiseSousVideAmpoule, INFINITE);

	manip.MiseSousVideAmpoule();
	
	pKaollaView->fin_experience = true;
	pKaollaView->DebloqueMenu();

	pKaollaView->LancementThreadMSVAmpoule();

	//g_eventStopMiseSousVideAmpoule.SetEvent();

	return 0;
}

void MiseSousVideBouteille()
{
	pKaollaView->fin_experience = false;
	pKaollaView->DebloqueMenu();
	g_eventStartMiseSousVideBouteille.SetEvent();
//	::WaitForSingleObject(g_eventStopMiseSousVideAmpoule, INFINITE);
}


UINT ThreadMenuMiseSousVideBouteille(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartMiseSousVideBouteille, INFINITE);

	manip.MiseSousVideBouteille();
	
	pKaollaView->fin_experience = true;
	pKaollaView->DebloqueMenu();

	pKaollaView->LancementThreadMSVBouteille();

	//g_eventStopMiseSousVideAmpoule.SetEvent();

	return 0;
}

void ChangementBouteille()
{
	pKaollaView->fin_experience = false;
	pKaollaView->DebloqueMenu();
	g_eventStartChangementBouteille.SetEvent();
//	::WaitForSingleObject(g_eventStopMiseSousVideAmpoule, INFINITE);
}


UINT ThreadMenuChangementBouteille(LPVOID pParam)
{
	::WaitForSingleObject(g_eventStartChangementBouteille, INFINITE);

	manip.ChangementBouteille();
	
	pKaollaView->fin_experience = true;
	pKaollaView->DebloqueMenu();

	pKaollaView->LancementThreadChangementBouteille();

	//g_eventStopMiseSousVideAmpoule.SetEvent();

	return 0;
}


// --------- Boutons --------------------

void DemandeModificationExperience()
{
	manip.ModifierParametresExperience();
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


// Boutons des vannes


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


// ----------- Fin des Boutons ------------








// Retourne un pointeur CKaollaView utilisé
// Du coup, on peut avoir les données de KaollaView actualisé automatiquement
CKaollaView* GetKaollaView(void)
{
	CWinApp* pApp = AfxGetApp(); 
	CDocTemplate* pTemplate;
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pTemplate = pApp->GetNextDocTemplate(pos);
		ASSERT(pTemplate);

		// tous les documents du template.
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2)
		{
			CDocument* pDoc = pTemplate->GetNextDoc(pos2);
			ASSERT(pDoc);

			// toutes les vues du document
			POSITION pos3 = pDoc->GetFirstViewPosition();
			while (pos3 != NULL)
			{
				CView* pView = pDoc->GetNextView(pos3);
				ASSERT(pView);
				if (::IsWindow(pView->GetSafeHwnd()))
				{
					if(pView->IsKindOf(RUNTIME_CLASS(CKaollaView)))
					{
						CKaollaView* pKV = static_cast<CKaollaView *>(pView);
						return pKV;
					}
				}
			}// while (pos3 != NULL)
		}// while (pos2)
	}// while (pos != NULL)
}
