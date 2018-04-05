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
	if (storage.experimentStatus.stepStatus == STEP_STATUS_START) {

		LOG(logINFO) << MESSAGE_EQUILIBRATION_STARTED;																		// Log the step change

		// This is where we start recording
		storage.experimentStatus.isRecording = true;

		// Record experiment start time
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		// Set the time to wait
		WaitMinutes(storage.experimentSettings.dataDivers.time_baseline, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_END;												// Set next step
	}

	if (storage.experimentStatus.stepStatus == STEP_STATUS_END) {

		if (storage.experimentStatus.isWaiting == false) {
			LOG(logINFO) << MESSAGE_EQUILIBRATION_COMPLETE;															// Log the step change
			storage.experimentStatus.mainStage = STAGE_ADSORPTION;											// Set next stage
			storage.experimentStatus.stepStatus = STEP_STATUS_START;										// Reset next step
		}
	}
}
