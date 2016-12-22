
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
		DialogTypeExperiment dialogExperimentType;
		dialogExperimentType.PassSettings(*machineSettings.get());

		if (dialogExperimentType.DoModal() == IDOK)
		{
			// Save user choice
			experimentSettings->experimentType = dialogExperimentType.TypeExperience;

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), machineSettings.get());
			dialogExperimentProperties.Initiate(*experimentSettings);

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
				commHandler.SetExperimentSettings(experimentSettings);
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
	if (pApp->serverConnected)
	{
		if (dataCollection.back()->experimentInProgress) {
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

			// Create dialog
			ExperimentPropertySheet dialogExperimentProperties(_T(""), machineSettings.get());
			dialogExperimentProperties.Initiate(*experimentSettings);

			int counter = 0;
			if (dataCollection.back()->experimentStage == STAGE_ADSORPTION)
			{
				counter = dataCollection.back()->adsorptionCounter;
			}
			if (dataCollection.back()->experimentStage == STAGE_DESORPTION)
			{
				counter = dataCollection.back()->desorptionCounter;
			}

			dialogExperimentProperties.SetProprietiesModif(dataCollection.back()->experimentStage, counter);

			if (dialogExperimentProperties.DoModal() == IDOK)
			{
				// Get the data from the dialog
				GetExperimentData(&dialogExperimentProperties, false);
			}
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
}
