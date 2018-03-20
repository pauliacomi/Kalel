#include "../Forcelib.h"

#include "ExperimentStatus.h"

#include "../timeHelpers.h"
#include "../Resources/DefineStages.h"

ExperimentStatus::ExperimentStatus()
{
	ResetData();
}

ExperimentStatus::ExperimentStatus(const ExperimentStatus & rhs)
{
	*this = rhs;
}

ExperimentStatus::~ExperimentStatus(void)
{
}


void ExperimentStatus::ResetData()
{
	experimentInProgress = false;
	experimentRecording = false;
	experimentWaiting = false;
	experimentCommandsRequested = true;

	experimentStage = STAGE_UNDEF;
	experimentPreviousStage = STAGE_UNDEF;
	experimentDose = 0;
	experimentStepStatus = STEP_STATUS_UNDEF;
	experimentSubstepStage = STEP_STATUS_START;
	verificationStep = STEP_VERIFICATIONS_UNDEF;

	timeStart = 0;
	timeEquilibrationStart = 0;
	timeToEquilibrate = 0.f;

	injectionAttemptCounter = 0;
	adsorptionCounter = 0;
	desorptionCounter = 0;

	pressureInitial = 0.f;
	pressureFinal = 0.f;
	pressureHighOld = 0.f;
}


ExperimentStatus & ExperimentStatus::operator=(const ExperimentStatus & p)
{
	if (this != &p) {  // make sure not same object
		
		///*******************
		///		Global flags
		///*******************

		experimentInProgress = p.experimentInProgress.load();
		experimentRecording = p.experimentRecording.load();
		experimentWaiting = p.experimentWaiting.load();
		experimentCommandsRequested = p.experimentCommandsRequested.load();

		///*******************
		///		Parameters for storing where program has reached
		///*******************

		experimentStage = p.experimentStage.load();
		experimentPreviousStage = p.experimentPreviousStage.load();
		experimentDose = p.experimentDose.load();
		experimentStepStatus = p.experimentStepStatus.load();
		experimentSubstepStage = p.experimentSubstepStage.load();
		verificationStep = p.verificationStep.load();

		timeStart = p.timeStart.load();
		timeEquilibrationStart = p.timeEquilibrationStart.load();
		timeToEquilibrate = p.timeToEquilibrate.load();

		injectionAttemptCounter = p.injectionAttemptCounter.load();
		adsorptionCounter = p.adsorptionCounter.load();
		desorptionCounter = p.desorptionCounter.load();

		pressureInitial = p.pressureInitial.load();
		pressureFinal = p.pressureFinal.load();
		pressureHighOld = p.pressureHighOld.load();
	}
	return *this;    // Return ref for multiple assignment
}