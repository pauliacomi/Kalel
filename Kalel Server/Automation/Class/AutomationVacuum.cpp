#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			VACUUMING
*
*
*/

void Automation::SampleVacuum()
{
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_UNDEF) {
		storage.experimentStatus.experimentStage = STAGE_VACUUM_SAMPLE;
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		storage.experimentStatus.experimentInProgress = true;
	}

	StageVacuum(true);
}

void Automation::BottleVacuum()
{
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_UNDEF
		&& storage.experimentStatus.experimentWaiting == false)
	{
		storage.experimentStatus.experimentStage = STAGE_VACUUM_BOTTLE;
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		storage.experimentStatus.experimentInProgress = true;
		LOG(logEVENT) << MESSAGE_VACUUM_BOTTLE_CLOSE;
		
		// Pause
		eventPause = true;
		storage.automationControl.notify_all();
	}



	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_START
		&& storage.experimentStatus.experimentWaiting == false)
	{
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS;				// Set next step
		LOG(logINFO) << MESSAGE_VACUUM_BOTTLE_START;									// Let GUI know the step change
		LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_START;

		controls.valveControls.CloseAll(true);											// Close everything
		controls.valveControls.PumpActivate(true);										// Activate the pump
		controls.valveControls.ValveOpen(VALVE_1, true);								// Open valve 1
		controls.valveControls.ValveOpen(VALVE_2, true);								// Open valve 2
		controls.valveControls.ValveOpen(VALVE_3, true);								// Open valve 3
		controls.valveControls.ValveOpen(VALVE_4, true);								// Open valve 4
		WaitSeconds(storage.machineSettings.TimeWaitPump);
	}




	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_INPROGRESS
		&& storage.experimentStatus.experimentWaiting == false)
	{
		if (storage.experimentStatus.experimentWaiting == false) {
			if (storage.currentData.pressureHigh > storage.machineSettings.readers.find(PRESSURE_LP)->second.safe_max) {

				// Open, then close v8 and v7
				controls.valveControls.ValveOpen(VALVE_8, true);
				WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
				storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS + 1;
			}
			else
			{
				LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_END;
				LOG(logINFO) << MESSAGE_VACUUM_LOWPRESSURE_START;
				controls.valveControls.ValveOpen(VALVE_4, true);
				WaitSeconds(storage.machineSettings.TimeWaitValves);

				storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS + 4;
			}
		}
	}

	if (storage.experimentStatus.experimentSubstepStage == STEP_STATUS_INPROGRESS + 1 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = STEP_STATUS_INPROGRESS + 2;
	}

	if (storage.experimentStatus.experimentSubstepStage == STEP_STATUS_INPROGRESS + 2 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveOpen(VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = STEP_STATUS_INPROGRESS + 3;
	}

	if (storage.experimentStatus.experimentSubstepStage == STEP_STATUS_INPROGRESS + 3 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = STEP_STATUS_INPROGRESS;					// Go back to the start
	}




	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_INPROGRESS + 4
		&& storage.experimentStatus.experimentWaiting == false)
	{
		if (storage.experimentStatus.experimentWaiting == false) {
			if (storage.currentData.pressureHigh > storage.machineSettings.PressurePumpVacuum) {

				// Open, then close v8 and v7
				controls.valveControls.ValveOpen(VALVE_8, true);
				WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
				storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS + 5;
			}
			else
			{
				LOG(logINFO) << MESSAGE_VACUUM_LOWPRESSURE_END;

				controls.valveControls.ValveOpen(VALVE_8, true);
				controls.valveControls.ValveOpen(VALVE_7, true);
				WaitSeconds(storage.machineSettings.TimeWaitValves);

				storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS + 8;
			}
		}
	}

	if (storage.experimentStatus.experimentSubstepStage == STEP_STATUS_INPROGRESS + 5 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = STEP_STATUS_INPROGRESS + 6;
	}

	if (storage.experimentStatus.experimentSubstepStage == STEP_STATUS_INPROGRESS + 6 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveOpen(VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = STEP_STATUS_INPROGRESS + 7;
	}

	if (storage.experimentStatus.experimentSubstepStage == STEP_STATUS_INPROGRESS + 7 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = STEP_STATUS_INPROGRESS + 4;					// Go back to the start
	}






	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_INPROGRESS + 8
		&& storage.experimentStatus.experimentWaiting == false)
	{
		LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_START;
		WaitMinutes(storage.machineSettings.TimeVacuumBottle);
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_END;
	}


	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_END
		&& storage.experimentStatus.experimentWaiting == false)
	{
		LOG(logINFO) << MESSAGE_VACUUM_BOTTLE_END;

		storage.experimentStatus.experimentStage = STAGE_UNDEF;
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;				// Let GUI know the step change

		controls.valveControls.CloseAll(true);										// Close everything

		shutdownReason = STOP_NORMAL;												// set a normal shutdown
		eventReset = true;														// end then set the event
		storage.automationControl.notify_all();
	}
}
