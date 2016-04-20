
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
	GetDocument()->experiment_running=TRUE;
	
	// set another flag to true
	DebloqueMenu(NULL, NULL);

	// Reset the graph
	GetDocument()->GraphInitialize(NULL,NULL);

	// Update the view (KaollaView)
	GetDocument()->UpdateAllViews(this); 

	// Create the experiment type window
	CDialogue_TypeExperience * dialogExperimentType = new CDialogue_TypeExperience();
	if (dialogExperimentType->DoModal() == IDOK)
	{
		// Save user choice
		experimentData->experimentType = dialogExperimentType->TypeExperience;

		// Create the experiment parameter window
		ExperimentPropertySheet * dialogExperimentProperties = new ExperimentPropertySheet(_T(""));

		// Instantiate the correct type of dialog
		switch (experimentData->experimentType)
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
			// Get the data from the dialog
			GetExperimentData(dialogExperimentProperties);

			// Launch the threads
			threadManager->SetStartEvent();
		}
		else
		{
			// Roll back by calling stop function
			Annuler(NULL,NULL);
		}
	}
}

// When clicking on the Stop button
void CKaollaView::OnBnClickedArreter()
{
	// Ask the thread to shutdown top the threads
	threadManager->ShutdownThread();
}


// Clicking the other buttons in the view
void CKaollaView::OnBnClickedButtonParametresExperience()
{
	//DemandeModificationExperience();
}

void CKaollaView::OnBnClickedArretSousVide()
{
	//ArretSousVideThreads();
}

void CKaollaView::OnBnClickedPause()
{
	//PauseThreads();
}

void CKaollaView::OnBnClickedProchaineCommande()
{
	//ProchaineCommandeThreads();
}

void CKaollaView::OnBnClickedProchaineDose()
{
	//ProchaineDoseThreads();
}

void CKaollaView::OnBnClickedProchaineEtape()
{
	//ProchaineEtapeThreads();
}

void CKaollaView::OnBnClickedReprise()
{
	//RepriseThreads();
}


// Copy all data from a property sheet dialog to the local object
void CKaollaView::GetExperimentData(ExperimentPropertySheet * dialogExperimentProperties) {
	experimentData->dataGeneral = dialogExperimentProperties->m_general.allSettings;
	if (experimentData->experimentType == EXPERIMENT_TYPE_AUTO)
	{
		experimentData->dataDivers = dialogExperimentProperties->m_divers.allSettings;
		for (size_t i = 0; i < dialogExperimentProperties->adsorptionTabs.size(); i++)
		{
			experimentData->dataAdsorption.push_back(dialogExperimentProperties->adsorptionTabs[i]->allSettings);
		}
		experimentData->dataDesorption = dialogExperimentProperties->m_desorption.allSettings;
	}
}