
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
}
