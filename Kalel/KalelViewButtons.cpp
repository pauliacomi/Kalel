
// KalelView_Boutons.cpp : all the code for the buttons in the view except the valve buttons
//

#include "stdafx.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"

#include "DialogTypeExperiment.h"		// The dialog asking the user for the experiment type
#include "ExperimentPropertySheet.h"	// The dialog asking the user to input the experiment parameters


// When clicking on the Launch button
void CKalelView::OnBnClickedLancer()
{
	if (pApp->serverConnected)
	{
		// Create the experiment type window
		DialogTypeExperiment dialogExperimentType(*machineSettings.get());

		if (dialogExperimentType.DoModal() == IDOK)
		{
			// Create temp settings and save user choice
			if (!tempExpSettings || tempExpSettings->experimentType != dialogExperimentType.TypeExperience) {
				tempExpSettings = std::make_shared<ExperimentSettings>(*experimentSettings);
			}
			tempExpSettings->experimentType = dialogExperimentType.TypeExperience;

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), machineSettings.get());
			dialogExperimentProperties.Initiate(*tempExpSettings);

			if (dialogExperimentProperties.DoModal() == IDOK)
			{
				// Get the data from the dialog
				GetExperimentData(dialogExperimentProperties, *tempExpSettings, true);

				// Send changed settings
				commHandler.SetExperimentSettings(tempExpSettings);
			}
			else
			{
				// Save the data from the dialog
				GetExperimentData(dialogExperimentProperties, *tempExpSettings, true);
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
	if (pApp->serverConnected)
	{
		if (pApp->experimentRunning) {
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
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}


// Clicking the other buttons in the view
void CKalelView::OnBnClickedButtonParametresExperience()
{
	if (pApp->serverConnected)
	{
		if (pApp->experimentRunning) {

			// Initialise temporary experiment settings
			tempExpSettings = std::make_shared<ExperimentSettings>(*experimentSettings);

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), machineSettings.get());
			dialogExperimentProperties.Initiate(*tempExpSettings);

			// Set the dialog to only display the ongoing stages
			int counter = 0;
			if (dataCollection.rbegin()->second->experimentStage == STAGE_ADSORPTION)
			{
				counter = dataCollection.rbegin()->second->adsorptionCounter;
			}
			if (dataCollection.rbegin()->second->experimentStage == STAGE_DESORPTION)
			{
				counter = dataCollection.rbegin()->second->desorptionCounter;
			}

			dialogExperimentProperties.SetProprietiesModif(dataCollection.rbegin()->second->experimentStage, counter);

			// If ok has been clicked
			if (dialogExperimentProperties.DoModal() == IDOK)
			{
				// Get the data from the dialog
				if (GetExperimentData(dialogExperimentProperties, *tempExpSettings, false)) {
					// IF MODIFIED
					// Send changed settings
					commHandler.SetExperimentSettings(tempExpSettings);
					return;
				}
			}
			// Reset temp settings if we get here
			tempExpSettings.reset();
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}

void CKalelView::OnBnClickedPause()
{
	if (pApp->serverConnected)
	{
		commHandler.PauseClient();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}

void CKalelView::OnBnClickedReprise()
{
	if (pApp->serverConnected)
	{
		commHandler.ResumeClient();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}

void CKalelView::OnBnClickedProchaineCommande()
{
	if (pApp->serverConnected)
	{
		//ProchaineCommandeThreads();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}

void CKalelView::OnBnClickedProchaineDose()
{
	if (pApp->serverConnected)
	{
		//ProchaineDoseThreads();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}

void CKalelView::OnBnClickedProchaineEtape()
{
	if (pApp->serverConnected)
	{
		//ProchaineEtapeThreads();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
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


void CKalelView::UpdateButtons() {

	bool eRun = !pApp->menuIsAvailable;

	GetDlgItem(IDC_PROCHAINE_COMMANDE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_DOSE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_ETAPE)->EnableWindow(eRun);
	GetDlgItem(IDC_ARRET_SOUS_VIDE)->EnableWindow(eRun);
	GetDlgItem(IDC_BUTTON_PARAMETRES_EXPERIENCE)->EnableWindow(eRun);
	GetDlgItem(IDC_REPRISE)->EnableWindow(eRun);
	GetDlgItem(IDC_PAUSE)->EnableWindow(eRun);

	buttonStates.Init();

	// the start button
	GetDlgItem(IDC_LANCER)->EnableWindow(!pApp->experimentRunning);
	// the stop button
	GetDlgItem(IDC_ARRETER)->EnableWindow(pApp->experimentRunning);
}


// Copy all data from a property sheet dialog to the local object
bool CKalelView::GetExperimentData(ExperimentPropertySheet & pDialogExperimentProperties, ExperimentSettings & expS, bool initialRequest) {

	if (initialRequest)
	{
		// Copy data across
		ReplaceExperimentSettings(pDialogExperimentProperties, expS);
	}

	else
	{
		// Must check if everything is the same

		bool modified = false;

		if (pDialogExperimentProperties.adsorptionTabs.size() != expS.dataAdsorption.size()
			&& pDialogExperimentProperties.desorptionTabs.size() != expS.dataDesorption.size())
		{
			modified = true;
		}
		else
		{
			if (pDialogExperimentProperties.m_general.allSettings != expS.dataGeneral)
			{
				modified = true;
			}

			if (pDialogExperimentProperties.m_divers.allSettings != expS.dataDivers)
			{
				modified = true;
			}

			for (size_t i = 0; i < pDialogExperimentProperties.adsorptionTabs.size(); i++)
			{
				if (pDialogExperimentProperties.adsorptionTabs[i]->allSettings != expS.dataAdsorption[i])
				{
					modified = true;
				}
			}
			for (size_t i = 0; i < pDialogExperimentProperties.desorptionTabs.size(); i++)
			{
				if (pDialogExperimentProperties.desorptionTabs[i]->allSettings != expS.dataDesorption[i])
				{
					modified = true;
				}
			}
		}

		if (modified)
		{
			// Copy data across
			ReplaceExperimentSettings(pDialogExperimentProperties, expS);
		}

		return modified;
	}

	return true;
}

void CKalelView::ReplaceExperimentSettings(const ExperimentPropertySheet & pDialogExperimentProperties, ExperimentSettings & expS)
{
	expS.dataGeneral = pDialogExperimentProperties.m_general.allSettings;

	if (expS.experimentType == EXPERIMENT_TYPE_AUTO)
	{
		expS.dataDivers = pDialogExperimentProperties.m_divers.allSettings;

		expS.dataAdsorption.clear();
		for (size_t i = 0; i < pDialogExperimentProperties.adsorptionTabs.size(); i++)
		{
			expS.dataAdsorption.push_back(pDialogExperimentProperties.adsorptionTabs[i]->allSettings);
		}
		expS.dataDesorption.clear();
		for (size_t i = 0; i < pDialogExperimentProperties.desorptionTabs.size(); i++)
		{
			expS.dataDesorption.push_back(pDialogExperimentProperties.desorptionTabs[i]->allSettings);
		}
	}
}

