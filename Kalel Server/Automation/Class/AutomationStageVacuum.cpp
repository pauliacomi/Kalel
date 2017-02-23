#include "Automation.h"

void Automation::StageVacuum(bool separateFunctionality)
{
	
	if (storage.currentData->experimentStepStatus == STEP_STATUS_START
		&& storage.currentData->experimentWaiting == false)
	{
		storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;					// Set next step
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_STAGE_START);				// Let GUI know the step change
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_START);

		controls.valveControls->CloseAll(true);												// Close all valves
		controls.valveControls->PumpActivate(true);											// Activate the pump
		controls.valveControls->ValveOpen(5, true);											// Open V5
		WaitSeconds(TIME_WAIT_PUMP);
	}


	if (storage.currentData->experimentStepStatus == STEP_STATUS_INPROGRESS
		&& storage.currentData->experimentWaiting == false)
	{
		if (storage.currentData->pressureHigh > GetPressionSecuriteBassePression()) {

			// Open, then close v8 and v7
			controls.valveControls->ValveOpen(8, true);
			WaitSeconds(TIME_WAIT_VALVES_SHORT);
			storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS + 1;
		}
		else
		{
			controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_END);
			controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_START);
			controls.valveControls->ValveOpen(6, true);
			WaitSeconds(TIME_WAIT_VALVES_SHORT);

			storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS + 4;
		}
	}

	if (storage.currentData->experimentSubstepStage == STEP_STATUS_INPROGRESS + 1 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(8, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = STEP_STATUS_INPROGRESS + 2;
	}

	if (storage.currentData->experimentSubstepStage == STEP_STATUS_INPROGRESS + 2 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = STEP_STATUS_INPROGRESS + 3;
	}

	if (storage.currentData->experimentSubstepStage == STEP_STATUS_INPROGRESS + 3 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = STEP_STATUS_INPROGRESS;					// Go back to the start
	}





	if (storage.currentData->experimentStepStatus == STEP_STATUS_INPROGRESS + 4
		&& storage.currentData->experimentWaiting == false)
	{
		if (storage.currentData->pressureHigh > pression_pompe) {

			// Open, then close v8 and v7
			controls.valveControls->ValveOpen(8, true);
			WaitSeconds(TIME_WAIT_VALVES_SHORT);
			storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS + 5;
		}
		else
		{
			controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_END);

			controls.valveControls->ValveOpen(8, true);
			controls.valveControls->ValveOpen(7, true);
			WaitSeconds(TIME_WAIT_VALVES);

			storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS + 8;
		}
	}

	if (storage.currentData->experimentSubstepStage == STEP_STATUS_INPROGRESS + 5 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(8, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = STEP_STATUS_INPROGRESS + 6;
	}

	if (storage.currentData->experimentSubstepStage == STEP_STATUS_INPROGRESS + 6 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = STEP_STATUS_INPROGRESS + 7;
	}

	if (storage.currentData->experimentSubstepStage == STEP_STATUS_INPROGRESS + 7 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = STEP_STATUS_INPROGRESS + 4;					// Go back to the start
	}





	if (storage.currentData->experimentStepStatus == STEP_STATUS_INPROGRESS + 8
		&& storage.currentData->experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_FINALOUTGAS_START);

		if (separateFunctionality)
		{
			controls.messageHandler->DisplayMessageBox(MESSAGE_VACUUM_USER_CHOICE, MB_ICONINFORMATION | MB_OK, true);
			h_eventPause = true;
			storage.automationControl.notify_all();
		}
		else
		{
			if (storage.experimentSettings->dataDivers.mise_sous_vide_fin_experience)
				WaitMinutes(storage.experimentSettings->dataDivers.temps_vide);
			else
				WaitMinutes(temps_defaut);
		}
		storage.currentData->experimentStepStatus = STEP_STATUS_END;
	}


	if (storage.currentData->experimentStepStatus == STEP_STATUS_END
		&& storage.currentData->experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_FINALOUTGAS_END);

		storage.currentData->experimentStepStatus = STEP_STATUS_START;													// Let GUI know the step change

		controls.valveControls->CloseAll(true);

		if (separateFunctionality)
		{
			storage.currentData->experimentStage = STAGE_UNDEF;
			shutdownReason = STOP_NORMAL;								// set a normal shutdown
			h_eventReset = true;										// end then set the event
			storage.automationControl.notify_all();									
		}
		else
		{
			controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_STAGE_END);

			storage.currentData->experimentStepStatus = STAGE_END_AUTOMATIC;
		}
	}
}