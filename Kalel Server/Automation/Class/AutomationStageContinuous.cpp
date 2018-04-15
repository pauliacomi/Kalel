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
		WaitMinutes(storage.experimentSettings.dataContinuous.temps_etalonnage_debit);

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
		WaitMinutes(storage.experimentSettings.dataContinuous.temps_etalonnage_volume_inter);

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

void Automation::StageContinuousAdsorption()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_CONT_ADSORPTION;														// Log the step change
		storage.experimentStatus.stepStatus = STEP_STATUS_START;
		break;

	case STEP_STATUS_START:

		controls.valveControls.ValveOpen(ID_VALVE_4, false);
		controls.valveControls.ValveOpen(ID_VALVE_3, false);
		controls.valveControls.ValveOpen(ID_VALVE_2, false);
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;									// Set next step

		break;

	case STEP_STATUS_INPROGRESS:

		controls.valveControls.ValveOpen(ID_VALVE_4, false);
		controls.valveControls.ValveOpen(ID_VALVE_3, false);
		controls.valveControls.ValveOpen(ID_VALVE_2, false);
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;									// Set next step

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
