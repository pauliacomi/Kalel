
// KaollaView_Boutons.cpp : all the code for the buttons in the view except the valve buttons
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "ThreadManager.h"

#include "Dialogue_TypeExperience.h"	// The dialog asking the user for the experiment type
#include "ExperimentPropertySheet.h"		// The dialog asking the user to input the experiment parameters


// When clicking on the Launch button
void CKaollaView::OnBnClickedLancer()
{
	// the start button is blocked
	GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);
	// the stop button is activated
	GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);

	// set one flag to true
	m_mainDocument = CKaollaDoc::GetDocument();
	m_mainDocument->experiment_running=TRUE;
	
	// set another flag to true
	DebloqueMenu();

	// Reset the graph
	GetDocument()->TempsMinimum = -1;
	GetDocument()->MesureMinimum = -1;
	GetDocument()->NumeroEtape = -1;

	// Update the view (KaollaView)
	GetDocument()->UpdateAllViews(this); 

	// Create the experiment type window
	CDialogue_TypeExperience * dialogExperimentType = new CDialogue_TypeExperience();
	if (dialogExperimentType->DoModal() == IDOK)
	{
		// Create the experiment parameter window
		ExperimentPropertySheet * dialogExperimentProperties = new ExperimentPropertySheet(_T(""));

		// Instantiate the correct type of dialog
		switch (dialogExperimentType->TypeExperience)
		{
		case EXPERIMENT_TYPE_MANUAL:
			dialogExperimentProperties->SetProprietiesManual();
			break;
		case EXPERIMENT_TYPE_AUTO:
			dialogExperimentProperties->SetProprietiesAuto();
			break;
		default:
			ASSERT(0);  // Should never be reached
		}

		if (dialogExperimentProperties->DoModal() == IDOK)
		{
			// Launch the threads 
			LancementThreads(GetSafeHwnd());
		}
		else
		{
			// Roll back by calling stop function
			OnBnClickedArreter();
		}
	}
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