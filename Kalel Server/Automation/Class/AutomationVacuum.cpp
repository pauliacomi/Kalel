#include "Automation.h"


void Automation::SampleVacuum()
{
	if (storage.currentData->experimentStepStatus == STEP_STATUS_UNDEF) {
		storage.currentData->experimentStage = STAGE_VACUUM_SAMPLE;
		storage.currentData->experimentStepStatus = STEP_STATUS_START;
		storage.currentData->experimentInProgress = true;
	}

	StageVacuum(true);
}

void Automation::BottleVacuum()
{
	if (storage.currentData->experimentStepStatus == STEP_STATUS_UNDEF
		&& storage.currentData->experimentWaiting == false)
	{
		storage.currentData->experimentStage = STAGE_VACUUM_BOTTLE;
		storage.currentData->experimentStepStatus = STEP_STATUS_START;
		storage.currentData->experimentInProgress = true;
		controls.messageHandler->DisplayMessageBox(MESSAGE_VACUUM_BOTTLE_CLOSE, MB_OK | MB_ICONQUESTION, true);
		
		// Pause
		eventPause = true;
		storage.automationControl.notify_all();
	}



	if (storage.currentData->experimentStepStatus == STEP_STATUS_START
		&& storage.currentData->experimentWaiting == false)
	{
		storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;				// Set next step
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_BOTTLE_START);			// Let GUI know the step change
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_START);

		controls.valveControls->CloseAll(true);											// Close everything
		controls.valveControls->PumpActivate(true);										// Activate the pump
		controls.valveControls->ValveOpen(1, true);										// Open valve 1
		controls.valveControls->ValveOpen(2, true);										// Open valve 2
		controls.valveControls->ValveOpen(3, true);										// Open valve 3
		controls.valveControls->ValveOpen(4, true);										// Open valve 4
		WaitSeconds(TIME_WAIT_PUMP);
	}




	if (storage.currentData->experimentStepStatus == STEP_STATUS_INPROGRESS
		&& storage.currentData->experimentWaiting == false)
	{
		if (storage.currentData->experimentWaiting == false) {
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
				controls.valveControls->ValveOpen(4, true);
				WaitSeconds(TIME_WAIT_VALVES);

				storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS + 4;
			}
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
		if (storage.currentData->experimentWaiting == false) {
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
		WaitMinutes(temps_bouteille);
		storage.currentData->experimentStepStatus = STEP_STATUS_END;
	}


	if (storage.currentData->experimentStepStatus == STEP_STATUS_END
		&& storage.currentData->experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_BOTTLE_END);

		storage.currentData->experimentStage = STAGE_UNDEF;
		storage.currentData->experimentStepStatus = STEP_STATUS_START;				// Let GUI know the step change

		controls.valveControls->CloseAll(true);										// Close everything

		shutdownReason = STOP_NORMAL;												// set a normal shutdown
		eventReset = true;														// end then set the event
		storage.automationControl.notify_all();
	}
}
