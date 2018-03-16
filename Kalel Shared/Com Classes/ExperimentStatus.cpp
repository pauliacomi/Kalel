#include "../Forcelib.h"

#include "ExperimentStatus.h"

#include "../timeHelpers.h"
#include "../Resources/DefineStages.h"

ExperimentStatus::ExperimentStatus()
{
	ResetData();
}

ExperimentStatus::~ExperimentStatus(void)
{
}


void ExperimentStatus::ResetData()
{
	timeChanged = timeh::NowTime();

	experimentInProgress.set(false);
	experimentRecording.set(false);
	experimentWaiting.set(false);
	experimentCommandsRequested.set(true);

	experimentStage.set(STAGE_UNDEF);
	experimentPreviousStage.set(STAGE_UNDEF);
	experimentDose.set(0);
	experimentStepStatus.set(STEP_STATUS_UNDEF);
	experimentSubstepStage.set(STEP_STATUS_START);
	verificationStep.set(STEP_VERIFICATIONS_UNDEF);

	timeStart.set(0);
	timeEquilibrationStart.set(0);
	timeToEquilibrate.set(0.f);

	injectionAttemptCounter.set(0);
	adsorptionCounter.set(0);
	desorptionCounter.set(0);

	pressureInitial.set(0.f);
	pressureFinal.set(0.f);
	pressureHighOld.set(0.f);
}