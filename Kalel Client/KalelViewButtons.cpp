
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
				tempExpSettings = std::make_unique<ExperimentSettings>(*experimentSettings);
			}
			tempExpSettings->experimentType = dialogExperimentType.TypeExperience;

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), machineSettings.get());
			dialogExperimentProperties.Initiate(*tempExpSettings);

			if (dialogExperimentProperties.DoModal() == IDOK)
			{
				// Get the data from the dialog
				dialogExperimentProperties.GetExperimentData(*tempExpSettings, true);

				// Send changed settings
				commHandler.SetExperimentSettings(*tempExpSettings);
			}
			else
			{
				// Save the data from the dialog
				dialogExperimentProperties.GetExperimentData(*tempExpSettings, true);
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
				commHandler.StopExperiment();
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
			tempExpSettings = std::make_unique<ExperimentSettings>(*experimentSettings);

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), machineSettings.get());
			dialogExperimentProperties.Initiate(*tempExpSettings);

			// Set the dialog to only display the ongoing stages
			dialogExperimentProperties.SetProprietiesModif(experimentStatus->mainStage, experimentStatus->stepCounter);

			// If ok has been clicked
			if (dialogExperimentProperties.DoModal() == IDOK)
			{
				// Get the data from the dialog
				if (dialogExperimentProperties.GetExperimentData(*tempExpSettings, false)) {
					// IF MODIFIED
					// Send changed settings
					commHandler.SetExperimentSettings(*tempExpSettings);
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
		commHandler.NextCommand();
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
		commHandler.NextSubstep();
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
		commHandler.NextStep();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
}


void CKalelView::UpdateButtons() {

	bool eRun = !pApp->menuIsAvailable;

	GetDlgItem(IDC_PROCHAINE_COMMANDE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_DOSE)->EnableWindow(eRun);
	GetDlgItem(IDC_PROCHAINE_ETAPE)->EnableWindow(eRun);
	GetDlgItem(IDC_BUTTON_PARAMETRES_EXPERIENCE)->EnableWindow(eRun);
	GetDlgItem(IDC_REPRISE)->EnableWindow(eRun);
	GetDlgItem(IDC_PAUSE)->EnableWindow(eRun);

	buttonStates.Init();

	// the start button
	GetDlgItem(IDC_LANCER)->EnableWindow(!pApp->experimentRunning);
	// the stop button
	GetDlgItem(IDC_ARRETER)->EnableWindow(pApp->experimentRunning);
}

