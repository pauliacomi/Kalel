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
	switch (storage.experimentStatus.mainStage)
	{
	case STAGE_UNDEF:
	{
		// Start experiment
		storage.experimentStatus.inProgress = true;

		// Write Time of start
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());
		
		storage.experimentStatus.mainStage = STAGE_VACUUM_SAMPLE;
		break;
	}

	case STAGE_VACUUM_SAMPLE:
		switch (storage.experimentStatus.stepStatus)
		{
		case STEP_STATUS_UNDEF:
			LOG_EVENT(qOK) << MESSAGE_VACUUM_CELL_CLOSE;
			eventUserInput = true;
			storage.automationControl.notify_all();
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
			break;

		case STEP_STATUS_START:
			LOG(logINFO) << MESSAGE_VACUUM_CELL_START;										// Let GUI know the step change
			controls.valveControls.ValveOpen(ID_VALVE_5, true);
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;					// Set next step
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
			break;

		case STEP_STATUS_INPROGRESS:
			if (SubstepsVacuumRef())
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
				++storage.experimentStatus.stepStatus;
			}
			break;

		case STEP_STATUS_INPROGRESS + 1:
			if (SubstepsVacuumLPvol())
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
				++storage.experimentStatus.stepStatus;
			}
			break;

		case STEP_STATUS_INPROGRESS + 2:

			LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_START;

			controls.valveControls.OpenEVsAndPump(true);
			controls.valveControls.ValveOpen(ID_VALVE_7, true);
			controls.valveControls.ValveOpen(ID_VALVE_8, true);
			WaitMinutes(storage.machineSettings.TimeVacuumBottle /60, true);
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
			break;

		case STEP_STATUS_END:
			storage.experimentStatus.mainStage = STAGE_VACUUM_END;
			break;
		
		default:
			break;
		}
		break;

	case STAGE_VACUUM_END:
		LOG(logINFO) << MESSAGE_VACUUM_BOTTLE_END;

		controls.valveControls.CloseAll(true);
		eventShutdown = true;														// end then set the event
		storage.automationControl.notify_all();
		break;

	default:
		break;
	}
}

void Automation::BottleVacuum()
{
	switch (storage.experimentStatus.mainStage)
	{
	case STAGE_UNDEF:
	{
		// Start experiment
		storage.experimentStatus.inProgress = true;

		// Write Time of start
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		storage.experimentStatus.mainStage = STAGE_VACUUM_SAMPLE;
		break;
	}

	case STAGE_VACUUM_SAMPLE:
		switch (storage.experimentStatus.stepStatus)
		{
		case STEP_STATUS_UNDEF:
			LOG_EVENT(qOK) << MESSAGE_VACUUM_BOTTLE_CLOSE;
			eventUserInput = true;
			storage.automationControl.notify_all();
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
			break;

		case STEP_STATUS_START:
			LOG(logINFO) << MESSAGE_VACUUM_CELL_START;										// Let GUI know the step change
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;				// Set next step
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
			break;

		case STEP_STATUS_INPROGRESS:
			if (SubstepsVacuumRef())
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
				++storage.experimentStatus.stepStatus;
			}
			break;

		case STEP_STATUS_INPROGRESS + 1:
			if (SubstepsVacuumBottle())
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
				++storage.experimentStatus.stepStatus;
			}
			break;

		case STEP_STATUS_INPROGRESS + 2:
			if (SubstepsVacuumLPvol())
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
				++storage.experimentStatus.stepStatus;
			}
			break;

		case STEP_STATUS_INPROGRESS + 3:

			LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_START;

			controls.valveControls.OpenEVsAndPump(true);
			controls.valveControls.ValveOpen(ID_VALVE_7, true);
			controls.valveControls.ValveOpen(ID_VALVE_8, true);
			WaitMinutes(storage.machineSettings.TimeVacuumBottle / 60, true);
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
			break;

		case STEP_STATUS_END:
			storage.experimentStatus.mainStage = STAGE_VACUUM_END;
			break;

		default:
			break;
		}
		break;

	case STAGE_VACUUM_END:
		LOG(logINFO) << MESSAGE_VACUUM_BOTTLE_END;

		controls.valveControls.CloseAll(true);
		eventShutdown = true;														// end then set the event
		storage.automationControl.notify_all();
		break;

	default:
		break;
	}
}



bool Automation::SubstepsVacuumRef()
{
	switch (storage.experimentStatus.substepStatus)
	{
	case SUBSTEP_STATUS_START:
		LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_START;
		controls.valveControls.OpenEVsAndPump(false);											// Activate the pump
		WaitSeconds(storage.machineSettings.TimeWaitPump, true);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;						// Set next step
		break;

	case SUBSTEP_STATUS_CHECK:
	{
		auto p_lp = storage.machineSettings.readers.find(PRESSURE_LP);
		if (p_lp != storage.machineSettings.readers.end())
		{
			if (storage.currentData.pressureHigh < p_lp->second.safe_max) {
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
				break;
			}
		}
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
		break;
	}

	// Open, then close v8 and v7
	case SUBSTEP_STATUS_INPROGRESS:
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 1:
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 2:
		controls.valveControls.ValveOpen(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 3:
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;		// Go back to the start
		break;

	case SUBSTEP_STATUS_END:
		LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_END;
		return true;
		break;
	}
	return false;
}

bool Automation::SubstepsVacuumLPvol()
{
	switch (storage.experimentStatus.substepStatus)
	{
	case SUBSTEP_STATUS_START:
		LOG(logINFO) << MESSAGE_VACUUM_LOWPRESSURE_START;
		controls.valveControls.OpenEVsAndPump(true);
		controls.valveControls.ValveClose(ID_VALVE_7, false);
		controls.valveControls.ValveClose(ID_VALVE_8, false);
		controls.valveControls.ValveOpen(ID_VALVE_6, true);
		WaitSeconds(storage.machineSettings.TimeWaitPump, true);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;						// Set next step
		break;

	case SUBSTEP_STATUS_CHECK:
	{
		if (storage.currentData.pressureHigh < storage.machineSettings.PressurePumpVacuum) {

			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
			break;
		}
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
		break;
	}

	// Open, then close v8 and v7
	case SUBSTEP_STATUS_INPROGRESS:
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 1:
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 2:
		controls.valveControls.ValveOpen(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 3:
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;		// Go back to the start
		break;

	case SUBSTEP_STATUS_END:
		LOG(logINFO) << MESSAGE_VACUUM_LOWPRESSURE_END;
		return true;
		break;
	}
	return false;
}


bool Automation::SubstepsVacuumBottle()
{
	switch (storage.experimentStatus.substepStatus)
	{
	case SUBSTEP_STATUS_START:
		LOG(logINFO) << MESSAGE_VACUUM_BOTTLECONN_START;
		controls.valveControls.OpenEVsAndPump(true);
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		controls.valveControls.ValveOpen(ID_VALVE_4, true);								// Activate the pump
		WaitSeconds(storage.machineSettings.TimeWaitPump, true);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;						// Set next step
		break;

	case SUBSTEP_STATUS_CHECK:
	{
		if (storage.currentData.pressureHigh < storage.machineSettings.PressurePumpVacuum) {

			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
			break;
		}
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
		break;
	}

	case SUBSTEP_STATUS_INPROGRESS:
		controls.valveControls.ValveOpen(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 1:
		controls.valveControls.ValveClose(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 2:
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		++storage.experimentStatus.substepStatus;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 3:
		controls.valveControls.ValveClose(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;		// Go back to the start
		break;

	case SUBSTEP_STATUS_END:
		controls.valveControls.ValveOpen(ID_VALVE_1, true);
		controls.valveControls.ValveOpen(ID_VALVE_2, true);
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitPump, true);

		LOG(logINFO) << MESSAGE_VACUUM_BOTTLECONN_END;
		return true;
		break;
	}
	return false;
}
