#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			Continuous measurement
*
*
*/

void Automation::StageContinuousFlowrate()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_CONT_FLOWRATE;															// Log the step change

		storage.experimentStatus.stepStatus = STEP_STATUS_START;
		break;

	case STEP_STATUS_START:

		controls.valveControls.ValveOpen(ID_VALVE_4, true);
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		controls.valveControls.ValveOpen(ID_VALVE_2, true);
		WaitMinutes(storage.experimentSettings.dataContinuous.temps_etalonnage_debit, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_END;											// Set next step

		break;

	case STEP_STATUS_END:

		LOG(logINFO) << MESSAGE_CONT_FLOWRATE_COMPLETE;													// Log the step change
		++storage.experimentStatus.mainStage;															// Set next stage
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;										// Reset next step
		break;

	default:
		break;
	}
}

void Automation::StageContinuousDeadvolume()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_CONT_DEADVOLUME;														// Log the step change
		storage.experimentStatus.stepStatus = STEP_STATUS_START;
		break;

	case STEP_STATUS_START:
		
		controls.valveControls.ValveOpen(ID_VALVE_4, false);
		controls.valveControls.ValveOpen(ID_VALVE_3, false);
		controls.valveControls.ValveOpen(ID_VALVE_2, false);
		controls.valveControls.ValveOpen(ID_VALVE_5, true);
		WaitMinutes(storage.experimentSettings.dataContinuous.temps_etalonnage_volume_inter, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_END;											// Set next step

		break;

	case STEP_STATUS_END:

		LOG(logINFO) << MESSAGE_CONT_DEADVOLUME_COMPLETE;												// Log the step change
		++storage.experimentStatus.mainStage;															// Set next stage
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;										// Reset next step
		break;

	default:
		break;
	}
}

void Automation::StageContVacuum()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_VACUUM_STAGE_START;											// Let GUI know the step change
		controls.valveControls.ValveClose(ID_VALVE_2, false);
		controls.valveControls.ValveClose(ID_VALVE_3, false);
		controls.valveControls.ValveOpen(ID_VALVE_4, false);
		controls.valveControls.ValveOpen(ID_VALVE_5, false);
		storage.experimentStatus.stepStatus = STEP_STATUS_START;							// Set next step
		break;

	case STEP_STATUS_START:
		controls.valveControls.OpenEVsAndPump(true);										// Activate the pump
		WaitSeconds(storage.machineSettings.TimeWaitPump, true);
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;						// Set next step
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

		if (storage.experimentSettings.dataDivers.mise_sous_vide_fin_experience)
			WaitMinutes(storage.experimentSettings.dataDivers.temps_vide, true);
		else
			WaitMinutes(storage.machineSettings.TimeWaitPump * 10, true);
		storage.experimentStatus.stepStatus = STEP_STATUS_END;
		break;

	case STEP_STATUS_END:

		controls.valveControls.ValveClose(ID_VALVE_7, true);
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		controls.valveControls.CloseEVsAndPump(true);

		LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_END;
		LOG(logINFO) << MESSAGE_VACUUM_STAGE_END;
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;													// Let GUI know the step change
		++storage.experimentStatus.mainStage;
		break;

	default:
		break;
	}
}

void Automation::StageContinuousAdsorption()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_CONT_ADSORPTION;														// Log the step change
		storage.experimentStatus.stepStatus = STEP_STATUS_START;
		break;

	case STEP_STATUS_START:

		LOG_EVENT(qOK) << MESSAGE_CONT_ADSORPTION_START;

		eventUserInput = true;
		storage.automationControl.notify_all();

		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;									// Set next step

		break;


	case STEP_STATUS_INPROGRESS:

		controls.valveControls.CloseEVsAndPump(false);
		controls.valveControls.ValveOpen(ID_VALVE_5, false);
		controls.valveControls.ValveOpen(ID_VALVE_4, false);
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		controls.valveControls.ValveOpen(ID_VALVE_2, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 1;								// Set next step

		break;

	case STEP_STATUS_INPROGRESS + 1:

		if (storage.currentData.pressureHigh > storage.experimentSettings.dataContinuous.pression_finale_adsorption_continue)
		{
			storage.experimentStatus.stepStatus = STEP_STATUS_END;										// Set next step
		}

		break;

	case STEP_STATUS_END:

		LOG(logINFO) << MESSAGE_CONT_ADSORPTION_COMPLETE;												// Log the step change
		++storage.experimentStatus.mainStage;															// Set next stage
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;										// Reset next step
		break;

	default:
		break;
	}
}
