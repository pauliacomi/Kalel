
// KalelView_Boutons.cpp : all the code for the buttons in the view except the valve buttons
//

#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"

#include "DialogTypeExperiment.h"		// The dialog asking the user for the experiment type
#include "ExperimentPropertySheet.h"	// The dialog asking the user to input the experiment parameters

#include "TabDoses.h"					// For manipulating the settings object
#include "TabDesorption.h"				// For manipulating the settings object

// When clicking on the Launch button
void CKalelView::OnBnClickedLancer()
{
	if (machineSettings.synced)
	{
		// Create the experiment type window
		DialogTypeExperiment dialogExperimentType;

		if (dialogExperimentType.DoModal() == IDOK)
		{
			// Save user choice
			experimentSettings->experimentType = dialogExperimentType.TypeExperience;

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), &machineSettings);
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

				// Get the data from the dialog
				GetExperimentData(&dialogExperimentProperties, true);

				// Raise the flag for data modified
				commHandler.SetModifiedData();
			}
			else
			{
				// Save the data from the dialog
				GetExperimentData(&dialogExperimentProperties, true);

				// Roll back by calling stop function
				CancelBeforeStarting(NULL, NULL);
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}

// When clicking on the Stop button
void CKalelView::OnBnClickedArreter()
{
	if (experimentData->experimentInProgress) {
		int result = AfxMessageBox(PROMPT_CANCELEXP, MB_ICONQUESTION | MB_YESNO);
		switch (result)
		{
		case IDYES:
			commHandler.ResetClient();
			break;
		
		case IDNO:
			break;

		default:
			ASSERT(0);
			break;
		}
	}
}


// Clicking the other buttons in the view
void CKalelView::OnBnClickedButtonParametresExperience()
{
	if (pApp->experimentRunning) {

		// Create dialog
		ExperimentPropertySheet dialogExperimentProperties(_T(""), &machineSettings);
		dialogExperimentProperties.Initiate(experimentSettings);

		int counter = 0;
		if (experimentData->experimentStage == STAGE_ADSORPTION)
		{
			counter = experimentData->adsorptionCounter;
		}
		if (experimentData->experimentStage == STAGE_DESORPTION)
		{
			counter = experimentData->desorptionCounter;
		}

		dialogExperimentProperties.SetProprietiesModif(experimentData->experimentStage, counter);

		if (dialogExperimentProperties.DoModal() == IDOK)
		{
			// Get the data from the dialog
			GetExperimentData(&dialogExperimentProperties, false);
		}
	}
}

void CKalelView::OnBnClickedPause()
{
	commHandler.PauseClient();
}

void CKalelView::OnBnClickedReprise()
{
	commHandler.ResumeClient();
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

void CKalelView::OnBnClickedArretSousVide()
{
	/*if (experimentData->experimentInProgress) {
		int result = AfxMessageBox(PROMPT_CANCELEXP, MB_ICONQUESTION | MB_YESNO);
		switch (result)
		{
		case IDYES:
			threadManager->ResetThread();
			break;

		case IDNO:
			break;

		default:
			ASSERT(0);
			break;
		}
	}*/
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
			commHandler.SetModifiedData();

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
