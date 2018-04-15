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


void Automation::StageEquilibration(int time)
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_EQUILIBRATION_STARTED;																		// Log the step change
		storage.experimentStatus.stepStatus = STEP_STATUS_START;
		break;

	case STEP_STATUS_START:
																															// This is where we start recording
		storage.experimentStatus.isRecording = true;

		// Record experiment start time
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		// Set the time to wait
		WaitMinutes(time, true);

		storage.experimentStatus.stepStatus = STEP_STATUS_END;												// Set next step

		break;

	case STEP_STATUS_END:

		LOG(logINFO) << MESSAGE_EQUILIBRATION_COMPLETE;													// Log the step change
		++storage.experimentStatus.mainStage;															// Set next stage
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;										// Reset next step
		break;

	default:
		break;
	}
}
