
// KaollaView_Boutons.cpp : all the code for the buttons in the view except the valve buttons
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"


// When clicking on the Launch button
void CKaollaView::OnBnClickedLancer()
{
	// the button is blocked
	GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);

	m_mainDocument = CKaollaDoc::GetDocument();
	m_mainDocument->experiment_running=TRUE;

	DebloqueMenu();


	// Pour le graphe 'par étape' ainsi pour l'instant

	GetDocument()->TempsMinimum = -1;
	GetDocument()->MesureMinimum = -1;
	GetDocument()->NumeroEtape = -1;

	// Update the view (KaollaView)
	GetDocument()->UpdateAllViews(this); 

	// Launch the threads 
	LancementThreads(GetSafeHwnd());

}

// When clicking on the Stop button
void CKaollaView::OnBnClickedArreter()
{

	// the button is blocked
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);

	// signal that this is the experiment end
	m_mainDocument = CKaollaDoc::GetDocument();
	m_mainDocument->experiment_running=FALSE;

	// Stop the threads
	ArretThreads(GetSafeHwnd());
}

// Unblock the stop button
void CKaollaView::UnblockStopButton()
{
	GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);
}

// When the experiment is canceled
void CKaollaView::Annuler()
{
	m_mainDocument = CKaollaDoc::GetDocument();
	m_mainDocument->experiment_running=FALSE;  // FALSE : expérience en cours

	DebloqueMenu();

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);
}


// Clicking the other buttons in the view
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