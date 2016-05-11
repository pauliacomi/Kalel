#include "stdafx.h"
#include "ExperimentData.h"
#include "Define_Stages.h"

ExperimentData::ExperimentData()
{
}


ExperimentData::~ExperimentData()
{
}

void ExperimentData::ResetData()
{
	experimentInProgress = false;
	experimentRecording = false;
	experimentWaiting = false;
	experimentCommandsRequested = false;

	experimentStage = STAGE_UNDEF;
	experimentPreviousStage = STAGE_UNDEF;
	experimentDose = 0;
	experimentStepStatus = STEP_STATUS_UNDEF;
	experimentSubstepStage = STEP_STATUS_START;
	experimentMeasurements = 0;
	verificationStep = STEP_VERIFICATIONS_UNDEF;

	experimentTime = 0.f;
	timeToEquilibrate = 0.f;
	timeToEquilibrateCurrent = 0.f;

	injectionAttemptCounter = 0;
	adsorptionCounter = 0;
	desorptionCounter = 0;

	resultCalorimeter = 0.f;

	pressureHigh = 0.f;
	pressureLow = 0.f;
	pressureInitial = 0.f;
	pressureFinal = 0.f;
	pressureHighOld = 0.f;

	temperatureCalo = 0.f;
	temperatureCage = 0.f;
	temperatureRoom = 0.f;
}

ExperimentData & ExperimentData::operator=(const ExperimentData * p) {
	if (this != p) {  // make sure not same object

		experimentDose = p->experimentDose;

		resultCalorimeter = p->resultCalorimeter;
		pressureHigh = p->pressureHigh;
		pressureLow = p->pressureLow;
		pressureInitial = p->pressureInitial;
		pressureFinal = p->pressureFinal;
		pressureLow = p->pressureLow;
		temperatureCalo = p->temperatureCalo;
		temperatureCage = p->temperatureCage;
		temperatureRoom = p->temperatureRoom;

		experimentTime = p->experimentTime;

	}
	return *this;    // Return ref for multiple assignment
}
