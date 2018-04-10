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

void Automation::StageVacuum()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_VACUUM_STAGE_START;											// Let GUI know the step change
		controls.valveControls.CloseAll(true);												// Close everything
		controls.valveControls.ValveOpen(ID_VALVE_5, true);									// Open V5
		storage.experimentStatus.stepStatus = STEP_STATUS_START;							// Set next step
		break;

	case STEP_STATUS_START:
		LOG(logINFO) << MESSAGE_VACUUM_HIGHPRESSURE_START;
		controls.valveControls.OpenEVsAndPump(true);											// Activate the pump
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

	case STEP_STATUS_INPROGRESS + 2:
		if (SubstepsVacuumLPvol())
		{
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
			++storage.experimentStatus.stepStatus;
		}
		break;

	case STEP_STATUS_INPROGRESS + 3:

		LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_START;

		if (storage.experimentSettings.dataDivers.mise_sous_vide_fin_experience)
			WaitMinutes(storage.experimentSettings.dataDivers.temps_vide);
		else
			WaitMinutes(storage.machineSettings.TimeVacuumEndDefault);
		storage.experimentStatus.stepStatus = STEP_STATUS_END;
		break;

	case STEP_STATUS_END:

		controls.valveControls.CloseAll(true);

		LOG(logINFO) << MESSAGE_VACUUM_FINALOUTGAS_END;
		LOG(logINFO) << MESSAGE_VACUUM_STAGE_END;
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;													// Let GUI know the step change
		++storage.experimentStatus.mainStage;
		break;

	default:
		break;
	}
}