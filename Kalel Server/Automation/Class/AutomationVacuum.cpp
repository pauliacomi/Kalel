#include "Automation.h"


void Automation::SampleVacuum()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_UNDEF) {
		experimentLocalData.experimentStage = STAGE_VACUUM_SAMPLE;
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		experimentLocalData.experimentInProgress = true;
	}

	StageVacuum(true);
}

void Automation::BottleVacuum()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_UNDEF
		&& experimentLocalData.experimentWaiting == false)
	{
		experimentLocalData.experimentStage = STAGE_VACUUM_BOTTLE;
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		experimentLocalData.experimentInProgress = true;
		controls.messageHandler->DisplayMessageBox(MESSAGE_VACUUM_BOTTLE_CLOSE, MB_OK | MB_ICONQUESTION, true);
		::SetEvent(h_eventPause);
	}



	if (experimentLocalData.experimentStepStatus == STEP_STATUS_START
		&& experimentLocalData.experimentWaiting == false)
	{
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_BOTTLE_START);														// Let GUI know the step change
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_START);

		ControlMechanismsCloseAll();								// Close all valves
		ActivatePump();											    // Activate the pump
		ValveOpen(1);												// Open valve 1
		ValveOpen(2);												// Open valve 2
		ValveOpen(3);												// Open valve 3
		ValveOpen(4);												// Open valve 4
		WaitSeconds(TIME_WAIT_PUMP);
	}




	if (experimentLocalData.experimentStepStatus == STEP_STATUS_INPROGRESS
		&& experimentLocalData.experimentWaiting == false)
	{
		if (experimentLocalData.experimentWaiting == false) {
			if (experimentLocalData.pressureHigh > GetPressionSecuriteBassePression()) {

				// Open, then close v8 and v7
				ValveOpen(8);
				WaitSeconds(TIME_WAIT_VALVES_SHORT);
				experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 1;
			}
			else
			{
				controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_END);
				controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_START);
				ValveOpen(4);
				WaitSeconds(TIME_WAIT_VALVES);

				experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 4;
			}
		}
	}

	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 1 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(8);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS + 2;
	}

	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 2 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveOpen(7);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS + 3;
	}

	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 3 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(7);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS;					// Go back to the start
	}




	if (experimentLocalData.experimentStepStatus == STEP_STATUS_INPROGRESS + 4
		&& experimentLocalData.experimentWaiting == false)
	{
		if (experimentLocalData.experimentWaiting == false) {
			if (experimentLocalData.pressureHigh > pression_pompe) {

				// Open, then close v8 and v7
				ValveOpen(8);
				WaitSeconds(TIME_WAIT_VALVES_SHORT);
				experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 5;
			}
			else
			{
				controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_END);

				ValveOpen(8);
				ValveOpen(7);
				WaitSeconds(TIME_WAIT_VALVES);

				experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 8;
			}
		}
	}

	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 5 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(8);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS + 6;
	}

	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 6 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveOpen(7);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS + 7;
	}

	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 7 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(7);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS + 4;					// Go back to the start
	}






	if (experimentLocalData.experimentStepStatus == STEP_STATUS_INPROGRESS + 8
		&& experimentLocalData.experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_FINALOUTGAS_START);
		WaitMinutes(temps_bouteille);
		experimentLocalData.experimentStepStatus = STEP_STATUS_END;
	}


	if (experimentLocalData.experimentStepStatus == STEP_STATUS_END
		&& experimentLocalData.experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_VACUUM_BOTTLE_END);

		experimentLocalData.experimentStage = STAGE_UNDEF;
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Let GUI know the step change

		ControlMechanismsCloseAll();

		shutdownReason = STOP_NORMAL;								// set a normal shutdown
		::SetEvent(h_eventReset);									// end then set the event
	}
}
