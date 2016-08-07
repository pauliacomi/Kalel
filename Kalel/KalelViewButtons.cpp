
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
		EnterCriticalSection(&experimentSettings->criticalSection);
		experimentSettings->experimentType = dialogExperimentType.TypeExperience;
		LeaveCriticalSection(&experimentSettings->criticalSection);

		// Create dialog
		ExperimentPropertySheet dialogExperimentProperties(_T(""));

		EnterCriticalSection(&experimentSettings->criticalSection);
		dialogExperimentProperties.Initiate(experimentSettings);
		LeaveCriticalSection(&experimentSettings->criticalSection);

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
			GetDocument()->GraphReset(NULL, NULL);

			// Get the data from the dialog
			EnterCriticalSection(&experimentSettings->criticalSection);
			GetExperimentData(&dialogExperimentProperties, true);
			LeaveCriticalSection(&experimentSettings->criticalSection);

			// Raise the flag for data modified
			experimentSettings->dataModified = true;
		}
		else
		{
			// Save the data from the dialog
			EnterCriticalSection(&experimentSettings->criticalSection);
			GetExperimentData(&dialogExperimentProperties, true);
			LeaveCriticalSection(&experimentSettings->criticalSection);

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

		EnterCriticalSection(&experimentSettings->criticalSection);
		dialogExperimentProperties.Initiate(experimentSettings);
		LeaveCriticalSection(&experimentSettings->criticalSection);

		int counter = 0;
		if (experimentData->experimentStage = STAGE_ADSORPTION)
		{
			counter = experimentData->adsorptionCounter;
		}
		if (experimentData->experimentStage = STAGE_DESORPTION)
		{
			counter = experimentData->desorptionCounter;
		}

		EnterCriticalSection(&experimentSettings->criticalSection);
		dialogExperimentProperties.SetProprietiesModif(experimentData->experimentStage, counter);
		LeaveCriticalSection(&experimentSettings->criticalSection);

		if (dialogExperimentProperties.DoModal() == IDOK)
		{
			// Get the data from the dialog
			EnterCriticalSection(&experimentSettings->criticalSection);
			GetExperimentData(&dialogExperimentProperties, false);
			LeaveCriticalSection(&experimentSettings->criticalSection);
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
	threadManager->ResumeThread();
}


// Copy all data from a property sheet dialog to the local object
void CKalelView::GetExperimentData(ExperimentPropertySheet * pDialogExperimentProperties, bool initialRequest) {

	if (initialRequest)
	{
		// Copy data across
		experimentSettings->dataGeneral = pDialogExperimentProperties->m_general.allSettings;

		if (experimentSettings->experimentType == EXPERIMENT_TYPE_AUTO)
		{
			experimentSettings->dataDivers = pDialogExperimentProperties->m_divers.allSettings;
		
			experimentSettings->dataAdsorption.clear();
			for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
			{
				experimentSettings->dataAdsorption.push_back(pDialogExperimentProperties->adsorptionTabs[i]->allSettings);
			}
			experimentSettings->dataDesorption.clear();
			for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
			{
				experimentSettings->dataDesorption.push_back(pDialogExperimentProperties->desorptionTabs[i]->allSettings);
			}
		}
	}

	else
	{
		// Must check if everything is the same
		
		bool modified = false;

		if (pDialogExperimentProperties->adsorptionTabs.size() != experimentSettings->dataAdsorption.size() 
			&& pDialogExperimentProperties->desorptionTabs.size() != experimentSettings->dataDesorption.size())
		{
			modified = true;
		}
		else
		{
			if (pDialogExperimentProperties->m_general.allSettings != experimentSettings->dataGeneral)
			{
				modified = true;
			}

			if (pDialogExperimentProperties->m_divers.allSettings != experimentSettings->dataDivers)
			{
				modified = true;
			}

			for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
			{
				if (pDialogExperimentProperties->adsorptionTabs[i]->allSettings != experimentSettings->dataAdsorption[i])
				{
					modified = true;
				}
			}
			for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
			{
				if (pDialogExperimentProperties->desorptionTabs[i]->allSettings != experimentSettings->dataDesorption[i])
				{
					modified = true;
				}
			}
		}
		
		if (modified)
		{
			// Raise the flag for data modified
			experimentSettings->dataModified = true;

			// Copy data across
			experimentSettings->dataGeneral = pDialogExperimentProperties->m_general.allSettings;

			if (experimentSettings->experimentType == EXPERIMENT_TYPE_AUTO)
			{
				experimentSettings->dataDivers = pDialogExperimentProperties->m_divers.allSettings;

				experimentSettings->dataAdsorption.clear();
				for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
				{
					experimentSettings->dataAdsorption.push_back(pDialogExperimentProperties->adsorptionTabs[i]->allSettings);
				}
				experimentSettings->dataDesorption.clear();
				for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
				{
					experimentSettings->dataDesorption.push_back(pDialogExperimentProperties->desorptionTabs[i]->allSettings);
				}
			}
		}
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
