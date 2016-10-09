#include "../Forcelib.h"
#include "ExperimentData.h"
#include "../Resources/DefineStages.h"	// For the names of the stages/experiment types

ExperimentData::ExperimentData()
{
	ResetData();
}


ExperimentData::~ExperimentData()
{
}

void ExperimentData::ResetData()
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
	experimentGraphPoints = 0;
	verificationStep = STEP_VERIFICATIONS_UNDEF;

	timeStart = NULL;
	measurementsMade = 0;
	timeElapsed = 0.f;
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

		timeElapsed = p->timeElapsed;

	}
	return *this;    // Return ref for multiple assignment
}
