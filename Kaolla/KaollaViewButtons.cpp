
// KaollaView_Boutons.cpp : all the code for the buttons in the view except the valve buttons
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "ThreadManager.h"

#include "Dialogue_TypeExperience.h"		// The dialog asking the user for the experiment type


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
		experimentSettings->experimentType = dialogExperimentType->TypeExperience;
		
		// Instantiate the correct type of dialog
		switch (experimentSettings->experimentType)
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
			threadManager->StartThread();
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
	threadManager->ShutdownThread();
}


// Clicking the other buttons in the view
void CKaollaView::OnBnClickedButtonParametresExperience()
{
	dialogExperimentProperties->SetProprietiesModif(experimentData.experimentStage);

	if (dialogExperimentProperties->DoModal() == IDOK)
	{
		// Get the data from the dialog
		GetExperimentData(dialogExperimentProperties);
	}
}

void CKaollaView::OnBnClickedArretSousVide()
{
	//ArretSousVideThreads();
}

void CKaollaView::OnBnClickedPause()
{
	threadManager->PauseThread();
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
	threadManager->StartThread();
}


// Copy all data from a property sheet dialog to the local object
void CKaollaView::GetExperimentData(ExperimentPropertySheet * pDialogExperimentProperties) {

	// Use a critical section to avoid data races
	EnterCriticalSection(&experimentSettings->criticalSection);

	// Raise the flag for data modified
	experimentSettings->dataModified = true;

	// Copy data accross
	experimentSettings->dataGeneral = pDialogExperimentProperties->m_general.allSettings;
	if (experimentSettings->experimentType == EXPERIMENT_TYPE_AUTO)
	{
		experimentSettings->dataDivers = pDialogExperimentProperties->m_divers.allSettings;
		for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
		{
			experimentSettings->dataAdsorption.push_back(pDialogExperimentProperties->adsorptionTabs[i]->allSettings);
		}
		for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
		{
			experimentSettings->dataDesorption.push_back(pDialogExperimentProperties->desorptionTabs[i]->allSettings);
		}
	}

	// Leave the critical section
	LeaveCriticalSection(&experimentSettings->criticalSection);

}