// This file contains Automation functions that are used to put things under vaccuum


#include "StdAfx.h"
#include "Automation.h"

void Automation::StageVacuum(bool separateFunctionality)
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_VACUUM_STAGE_START);														// Let GUI know the step change
		messageHandler.DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_START);

		ControlMechanismsCloseAll();								// Close all valves
		ActivatePump();											    // Activate the pump
		ValveOpen(5);												// Open V5

		break;


	case STEP_STATUS_INPROGRESS:
		if (experimentLocalData.pressureHigh > GetPressionSecuriteBassePression()) {

			// Open, then close v8 and v7
			ValveOpenClose(8);
			ValveOpenClose(7);
		}
		else
		{
			messageHandler.DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_END);
			messageHandler.DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_START);
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
			messageHandler.DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_END);

			ValveOpen(8);
			ValveOpen(7);

			experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 2;
		}
		break;


	case STEP_STATUS_INPROGRESS + 2:

		messageHandler.DisplayMessage(MESSAGE_VACUUM_FINALOUTGAS_START);

		if (separateFunctionality)
		{
			messageHandler.DisplayMessageBox(MESSAGE_VACUUM_USER_CHOICE, MB_ICONINFORMATION | MB_OK, true);
			SetEvent(h_eventPause);
		}
		else
		{
			if (experimentLocalSettings.dataDivers.mise_sous_vide_fin_experience)
				WaitMinutes(experimentLocalSettings.dataDivers.temps_vide);
			else
				WaitMinutes(temps_defaut);
		}

		experimentLocalData.experimentStage = STEP_STATUS_END;
		break;


	case STEP_STATUS_END:
		if (experimentLocalData.experimentWaiting == false)
		{
			messageHandler.DisplayMessage(MESSAGE_VACUUM_FINALOUTGAS_END);

			experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Let GUI know the step change

			ControlMechanismsCloseAll();

			messageHandler.DisplayMessage(MESSAGE_VACUUM_STAGE_END);

			experimentLocalData.experimentStage = STAGE_END_AUTOMATIC;
		}
		break;
	}
}