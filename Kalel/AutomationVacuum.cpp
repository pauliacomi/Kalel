
// This file contains Automation functions that are used to put things under vaccuum


#include "StdAfx.h"
#include "Automation.h"


void Automation::SampleVacuum()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_STAGE_START);													// Let GUI know the step change

		ControlMechanismsCloseAll();								// Close all valves
		ActivatePump();											    // Activate the pump
		ValveOpen(5);												// Open V5

		messageHandler.DisplayMessage(_T("Début Mise sous vide de la haute pression\r\n"));
		messageHandler.DisplayStep(_T("Mise sous vide : Haute Pression"));
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;

		break;


	case STEP_STATUS_INPROGRESS:
		if (experimentLocalData.pressureHigh > GetPressionSecuriteBassePression()) {

			// Open, then close v8 and v7
			ValveOpenClose(8);
			ValveOpenClose(7);
		}
		else
		{
			messageHandler.DisplayMessage(_T("Fin Mise sous vide de la haute pression\r\n"));
			messageHandler.DisplayMessage(_T("Début Mise sous vide de la basse pression\r\n"));
			messageHandler.DisplayStep(_T("Mise sous vide : Basse Pression"));
			ValveOpen(6);

			experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 1;
		}
		break;


	case STEP_STATUS_INPROGRESS + 1:
		if (experimentLocalData.pressureHigh > pression_pompe) {

			// Open, then close v8 and v7
			ValveOpenClose(8);
			ValveOpenClose(7);
		}
		else
		{
			messageHandler.DisplayMessage(_T("Fin Mise sous vide de la basse pression\r\n"));

			ValveOpen(8);
			ValveOpen(7);

			experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 2;
		}
		break;


	case STEP_STATUS_INPROGRESS + 2:
		if (experimentLocalSettings.dataDivers.mise_sous_vide_fin_experience)
			WaitMinutes(experimentLocalSettings.dataDivers.temps_vide);
		else
			WaitMinutes(temps_defaut);

		CString nom_etape = _T("Mise sous vide : Dernier équilibre sous vide");
		experimentLocalData.experimentStage = STEP_STATUS_END;
		break;


	case STEP_STATUS_END:
		if (experimentLocalData.experimentWaiting == false)
		{
			experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Let GUI know the step change

			ControlMechanismsCloseAll();
			experimentLocalData.experimentStage = STAGE_END_AUTOMATIC;
		}
		break;
	}
}

void Automation::BottleVacuum()
{
}
