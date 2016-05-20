
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
			// the start button is blocked
			GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);
			// the stop button is activated
			GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);

			// Block menu and set running flag
			pApp->experimentRunning = true;
			pApp->menuIsAvailable = false;
			UpdateButtons();
			
			// Reset the graph
			GetDocument()->GraphInitialize(NULL, NULL);

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
	if (pApp->experimentRunning) {

		dialogExperimentProperties->SetProprietiesModif(experimentData.experimentStage);

		if (dialogExperimentProperties->DoModal() == IDOK)
		{
			// Get the data from the dialog
			GetExperimentData(dialogExperimentProperties);
		}
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
void CKaollaView::GetExperimentData(ExperimentPropertySheet * dialogExperimentProperties) {

	// Use a critical section to avoid data races
	EnterCriticalSection(&experimentSettings->criticalSection);

	// Raise the flag for data modified
	experimentSettings->dataModified = true;

	// Copy data accross
	experimentSettings->dataGeneral = dialogExperimentProperties->m_general.allSettings;
	if (experimentSettings->experimentType == EXPERIMENT_TYPE_AUTO)
	{
		experimentSettings->dataDivers = dialogExperimentProperties->m_divers.allSettings;
		for (size_t i = 0; i < dialogExperimentProperties->adsorptionTabs.size(); i++)
		{
			experimentSettings->dataAdsorption.push_back(dialogExperimentProperties->adsorptionTabs[i]->allSettings);
		}
		for (size_t i = 0; i < dialogExperimentProperties->desorptionTabs.size(); i++)
		{
			experimentSettings->dataDesorption.push_back(dialogExperimentProperties->desorptionTabs[i]->allSettings);
		}
	}

	// Leave the critical section
	LeaveCriticalSection(&experimentSettings->criticalSection);

}

void CKaollaView::UpdateButtons() {

	bool eRun = !pApp->menuIsAvailable;

	GetDlgItem(IDC_PROCHAINE_COMMANDE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_DOSE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_ETAPE)->EnableWindow(eRun);
	GetDlgItem(IDC_ARRET_SOUS_VIDE)->EnableWindow(eRun);
	GetDlgItem(IDC_BUTTON_PARAMETRES_EXPERIENCE)->EnableWindow(eRun);
	GetDlgItem(IDC_REPRISE)->EnableWindow(eRun);
	GetDlgItem(IDC_PAUSE)->EnableWindow(eRun);
}
