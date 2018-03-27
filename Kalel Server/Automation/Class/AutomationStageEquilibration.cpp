#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			EQUILIBRATION
*
*
*/


void Automation::StageEquilibration()
{
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_START) {

		LOG(logINFO) << MESSAGE_EQUILIBRATION_STARTED;																		// Log the step change

		// This is where we start recording
		storage.experimentStatus.experimentRecording = true;

		storage.experimentStatus.timeStart = time(0);																		// Record experiment start time
		storage.timerExperiment.Start();																					// Start global experiment timer	

		// Set the time to wait
		WaitMinutes(storage.experimentSettings.dataDivers.time_baseline, true);

		storage.experimentStatus.experimentStepStatus = STEP_STATUS_END;												// Set next step
	}

	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_END) {

		if (storage.experimentStatus.experimentWaiting == false) {
			LOG(logINFO) << MESSAGE_EQUILIBRATION_COMPLETE;															// Log the step change
			storage.experimentStatus.experimentStage = STAGE_ADSORPTION;											// Set next stage
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;										// Reset next step
		}
	}
}
