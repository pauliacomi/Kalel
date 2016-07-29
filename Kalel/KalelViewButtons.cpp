
// KalelView_Boutons.cpp : all the code for the buttons in the view except the valve buttons
//

#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"

#include "ThreadManager.h"

#include "DialogTypeExperiment.h"		// The dialog asking the user for the experiment type


// When clicking on the Launch button
void CKalelView::OnBnClickedLancer()
{
	// Create the experiment type window
	DialogTypeExperiment dialogExperimentType;

	if (dialogExperimentType.DoModal() == IDOK)
	{
		// Save user choice
		experimentSettings->experimentType = dialogExperimentType.TypeExperience;

		// Create dialog
		ExperimentPropertySheet dialogExperimentProperties(_T(""));
		dialogExperimentProperties.Initiate(experimentSettings);

		if (dialogExperimentProperties.DoModal() == IDOK)
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
			GetExperimentData(&dialogExperimentProperties, true);

			// Launch the threads
			threadManager->StartThread();
		}
		else
		{
			// Save the data from the dialog
			GetExperimentData(&dialogExperimentProperties, true);

			// Roll back by calling stop function
			Annuler(NULL,NULL);
		}
	}
}

// When clicking on the Stop button
void CKalelView::OnBnClickedArreter()
{
	threadManager->ResetThread();
}


// Clicking the other buttons in the view
void CKalelView::OnBnClickedButtonParametresExperience()
{
	if (pApp->experimentRunning) {

		// Create dialog
		ExperimentPropertySheet dialogExperimentProperties(_T(""));

		dialogExperimentProperties.SetProprietiesModif(experimentData.experimentStage);

		if (dialogExperimentProperties.DoModal() == IDOK)
		{
			// Get the data from the dialog
			GetExperimentData(&dialogExperimentProperties, false);
		}
	}
}

void CKalelView::OnBnClickedArretSousVide()
{
	//ArretSousVideThreads();
}

void CKalelView::OnBnClickedPause()
{
	threadManager->PauseThread();
}

void CKalelView::OnBnClickedProchaineCommande()
{
	//ProchaineCommandeThreads();
}

void CKalelView::OnBnClickedProchaineDose()
{
	//ProchaineDoseThreads();
}

void CKalelView::OnBnClickedProchaineEtape()
{
	//ProchaineEtapeThreads();
}

void CKalelView::OnBnClickedReprise()
{
	threadManager->StartThread();
}


// Copy all data from a property sheet dialog to the local object
void CKalelView::GetExperimentData(ExperimentPropertySheet * pDialogExperimentProperties, bool initialRequest) {

	if (initialRequest)
	{
		// Raise the flag for data modified
		experimentSettings->dataModified = true;

		// Just copy data accross
		experimentSettings->dataGeneral = pDialogExperimentProperties->m_general.allSettings;

		if (experimentSettings->experimentType == EXPERIMENT_TYPE_AUTO)
		{
			experimentSettings->dataDivers = pDialogExperimentProperties->m_divers.allSettings;
			for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
			{
				experimentSettings->dataAdsorption[i] = pDialogExperimentProperties->adsorptionTabs[i]->allSettings;
			}
			for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
			{
				experimentSettings->dataDesorption[i] = pDialogExperimentProperties->desorptionTabs[i]->allSettings;
			}
		}
	}

	else
	{
		// Use a critical section to avoid data races
		EnterCriticalSection(&experimentSettings->criticalSection);

		// Must check if everything is the same
		for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
		{
			if (true)
			{

			}
			experimentSettings->dataAdsorption.push_back(pDialogExperimentProperties->adsorptionTabs[i]->allSettings);
		}
		for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
		{
			experimentSettings->dataDesorption.push_back(pDialogExperimentProperties->desorptionTabs[i]->allSettings);
		}

		// Leave the critical section
		LeaveCriticalSection(&experimentSettings->criticalSection);
	}

}

void CKalelView::UpdateButtons() {

	bool eRun = !pApp->menuIsAvailable;

	GetDlgItem(IDC_PROCHAINE_COMMANDE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_DOSE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_ETAPE)->EnableWindow(eRun);
	GetDlgItem(IDC_ARRET_SOUS_VIDE)->EnableWindow(eRun);
	GetDlgItem(IDC_BUTTON_PARAMETRES_EXPERIENCE)->EnableWindow(eRun);
	GetDlgItem(IDC_REPRISE)->EnableWindow(eRun);
	GetDlgItem(IDC_PAUSE)->EnableWindow(eRun);
}
