#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"


using namespace std;



// gestionnaires de messages pour CKaollaView

// Lorsqu'on clique sur le bouton 'Lancer'
void CKaollaView::OnBnClickedLancer()
{
	// Le bouton 'Lancer' est bloqué
	GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);

	fin_experience=FALSE;  // FALSE : expérience en cours

	//pApp->disponibilite_menu = fin_experience;
	DebloqueMenu();


	// Pour le graphe 'par étape' ainsi pour l'instant

	GetDocument()->TempsMinimum = -1;
	GetDocument()->MesureMinimum = -1;
	GetDocument()->NumeroEtape = -1;
	// Réactualise cette View (ici KaollaView)
	GetDocument()->UpdateAllViews(this); 

	LancementThreads();

}

// Lorsqu'on clique sur le bouton 'Arreter'
void CKaollaView::OnBnClickedArreter()
{
	// On bloque le bouton d'arrêt
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);

	// On signale que c'ets la fin de l'expérience
	fin_experience=TRUE;

	ArretThreads();
}

void CKaollaView::ConfirmationLancement()
{
	AfxBeginThread(ArretThreadProc, GetSafeHwnd());

	// On débloque le bouton 'Arreter'
	// L'expérience peut maintenant être arrêter à tout moment
	GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);
}


void CKaollaView::Annuler()
{
	fin_experience=TRUE;  // FALSE : expérience en cours
	//pApp->disponibilite_menu = fin_experience;
	DebloqueMenu();

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);
	AfxBeginThread(LancerThreadProc, GetSafeHwnd());
}



void CKaollaView::OnBnClickedButtonParametresExperience()
{
	DemandeModificationExperience();
}


void CKaollaView::OnBnClickedArretSousVide()
{
	ArretSousVideThreads();
}

void CKaollaView::OnBnClickedPause()
{
	PauseThreads();
}

void CKaollaView::OnBnClickedProchaineCommande()
{
	ProchaineCommandeThreads();
}

void CKaollaView::OnBnClickedProchaineDose()
{
	ProchaineDoseThreads();
}

void CKaollaView::OnBnClickedProchaineEtape()
{
	ProchaineEtapeThreads();
}

void CKaollaView::OnBnClickedReprise()
{
	RepriseThreads();
}