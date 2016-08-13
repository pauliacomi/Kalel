
// This file contains Automation functions that are used to put things under vaccuum


#include "StdAfx.h"
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
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_UNDEF:
		experimentLocalData.experimentStage = STAGE_VACUUM_BOTTLE;
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		experimentLocalData.experimentInProgress = true;
		messageHandler.DisplayMessageBox(MESSAGE_VACUUM_BOTTLE_CLOSE, MB_OK | MB_ICONQUESTION, true);
		::SetEvent(h_eventPause);

		break;

	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_VACUUM_BOTTLE_START);														// Let GUI know the step change
		messageHandler.DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_START);

		ControlMechanismsCloseAll();								// Close all valves
		ActivatePump();											    // Activate the pump
		ValveOpen(1);												// Open valve 1
		ValveOpen(2);												// Open valve 2
		ValveOpen(3);												// Open valve 3
		ValveOpen(4);												// Open valve 4
		WaitSeconds(TIME_WAIT_PUMP);
		break;


	case STEP_STATUS_INPROGRESS:
		if (experimentLocalData.experimentWaiting == false) {
			if (experimentLocalData.pressureHigh > GetPressionSecuriteBassePression()) {

				// Open, then close v8 and v7
				ValveOpenClose(8);
				ValveOpenClose(7);
				WaitSeconds(TIME_WAIT_VALVES);
			}
			else
			{
				messageHandler.DisplayMessage(MESSAGE_VACUUM_HIGHPRESSURE_END);
				messageHandler.DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_START);
				ValveOpen(4);
				WaitSeconds(TIME_WAIT_VALVES);

				experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 1;
			}
		}
		break;


	case STEP_STATUS_INPROGRESS + 1:
		if (experimentLocalData.experimentWaiting == false) {
			if (experimentLocalData.pressureHigh > pression_pompe) {

				// Open, then close v8 and v7
				ValveOpenClose(8);
				ValveOpenClose(7);
				WaitSeconds(TIME_WAIT_VALVES);
			}
			else
			{
				messageHandler.DisplayMessage(MESSAGE_VACUUM_LOWPRESSURE_END);

				ValveOpen(8);
				ValveOpen(7);
				WaitSeconds(TIME_WAIT_VALVES);

				experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS + 2;
			}
		}
		break;


	case STEP_STATUS_INPROGRESS + 2:
		if (experimentLocalData.experimentWaiting == false) {
			messageHandler.DisplayMessage(MESSAGE_VACUUM_FINALOUTGAS_START);
			WaitMinutes(temps_bouteille);
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}
		break;


	case STEP_STATUS_END:
		if (experimentLocalData.experimentWaiting == false)
		{
			messageHandler.DisplayMessage(MESSAGE_VACUUM_BOTTLE_END);

			experimentLocalData.experimentStage = STAGE_UNDEF;
			experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Let GUI know the step change

			ControlMechanismsCloseAll();

			shutdownReason = STOP_NORMAL;								// set a normal shutdown
			::SetEvent(h_eventReset);									// end then set the event
		}
		break;
	}
}
