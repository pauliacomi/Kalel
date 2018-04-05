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

void Automation::StageVacuum(bool separateFunctionality)
{
	
	if (storage.experimentStatus.stepStatus == STEP_STATUS_START
		&& storage.experimentStatus.isWaiting == false)
	{
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;			// Set next step
		LOG(logINFO) << MESSAGE_VACUUM_STAGE_START;											// Let GUI know the step change
		LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_START;

		controls.valveControls.CloseAll(true);												// Close all valves
		controls.valveControls.PumpActivate(true);											// Activate the pump
		controls.valveControls.ValveOpen(ID_VALVE_5, true);									// Open V5
		WaitSeconds(storage.machineSettings.TimeWaitPump, true);
	}


	if (storage.experimentStatus.stepStatus == STEP_STATUS_INPROGRESS
		&& storage.experimentStatus.isWaiting == false)
	{
		if (storage.currentData.pressureHigh > storage.machineSettings.readers.find(PRESSURE_LP)->second.safe_max) {

			// Open, then close v8 and v7
			controls.valveControls.ValveOpen(ID_VALVE_8, true);
			WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 1;
		}
		else
		{
			LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_END;
			LOG(logINFO) << MESSAGE_VACUUM_LOWPRESSURE_START;
			controls.valveControls.ValveOpen(ID_VALVE_6, true);
			WaitSeconds(storage.machineSettings.TimeWaitValvesShort);

			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 4;
		}
	}

	if (storage.experimentStatus.substepStatus == STEP_STATUS_INPROGRESS + 1 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = STEP_STATUS_INPROGRESS + 2;
	}

	if (storage.experimentStatus.substepStatus == STEP_STATUS_INPROGRESS + 2 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = STEP_STATUS_INPROGRESS + 3;
	}

	if (storage.experimentStatus.substepStatus == STEP_STATUS_INPROGRESS + 3 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = STEP_STATUS_INPROGRESS;					// Go back to the start
	}





	if (storage.experimentStatus.stepStatus == STEP_STATUS_INPROGRESS + 4
		&& storage.experimentStatus.isWaiting == false)
	{
		if (storage.currentData.pressureHigh > storage.machineSettings.PressurePumpVacuum) {

			// Open, then close v8 and v7
			controls.valveControls.ValveOpen(ID_VALVE_8, true);
			WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 5;
		}
		else
		{
			LOG(logINFO) << MESSAGE_VACUUM_LOWPRESSURE_END;

			controls.valveControls.ValveOpen(ID_VALVE_8, true);
			controls.valveControls.ValveOpen(ID_VALVE_7, true);
			WaitSeconds(storage.machineSettings.TimeWaitValves);

			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 8;
		}
	}

	if (storage.experimentStatus.substepStatus == STEP_STATUS_INPROGRESS + 5 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = STEP_STATUS_INPROGRESS + 6;
	}

	if (storage.experimentStatus.substepStatus == STEP_STATUS_INPROGRESS + 6 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = STEP_STATUS_INPROGRESS + 7;
	}

	if (storage.experimentStatus.substepStatus == STEP_STATUS_INPROGRESS + 7 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = STEP_STATUS_INPROGRESS + 4;					// Go back to the start
	}





	if (storage.experimentStatus.stepStatus == STEP_STATUS_INPROGRESS + 8
		&& storage.experimentStatus.isWaiting == false)
	{
		LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_START;

		if (separateFunctionality)
		{
			LOG_EVENT(qOK) << MESSAGE_VACUUM_USER_CHOICE;
			eventPause = true;
			storage.automationControl.notify_all();
		}
		else
		{
			if (storage.experimentSettings.dataDivers.mise_sous_vide_fin_experience)
				WaitMinutes(storage.experimentSettings.dataDivers.temps_vide);
			else
				WaitMinutes(storage.machineSettings.TimeVacuumEndDefault);
		}
		storage.experimentStatus.stepStatus = STEP_STATUS_END;
	}


	if (storage.experimentStatus.stepStatus == STEP_STATUS_END
		&& storage.experimentStatus.isWaiting == false)
	{
		LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_END;

		storage.experimentStatus.stepStatus = STEP_STATUS_START;													// Let GUI know the step change

		controls.valveControls.CloseAll(true);

		if (separateFunctionality)
		{
			storage.experimentStatus.mainStage = STAGE_UNDEF;
			shutdownReason = Stop::Normal;								// set a normal shutdown
			eventShutdown = true;										// end then set the event
			storage.automationControl.notify_all();									
		}
		else
		{
			LOG(logINFO) << MESSAGE_VACUUM_STAGE_END;

			storage.experimentStatus.stepStatus = STAGE_END_AUTOMATIC;
		}
	}
}