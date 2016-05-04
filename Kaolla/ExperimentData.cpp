#include "stdafx.h"
#include "ExperimentData.h"


ExperimentData::ExperimentData()
	: experimentStage(0)
	, experimentPreviousStage(0)
	, experimentDose(0)
	, experimentStepStatus(0)
	, experimentEquilibrationStatus(0)
	, experimentSubstepStage(0)
	, experimentMeasurement(0)
	, verificationStep(0)

	, experimentTime(0)
	, timeToEquilibrate(0)

	, injectionAttemptCounter(0)
	, adsorptionCounter(0)
	, desorptionCounter(0)

	, resultCalorimeter(0)

	, pressureHigh(0)
	, pressureLow(0)
	, pressureInitial(0)
	, pressureFinal(0)
	, pressureHighOld(0)

	, temperatureCalo(0)
	, temperatureCage(0)
	, temperatureRoom(0)

	, GUIhandle(NULL)
{
	// Initialisation of the critical section
	InitializeCriticalSection(&criticalSection);
}


ExperimentData::~ExperimentData()
{
	// Destroy the critical sections
	DeleteCriticalSection(&criticalSection);
}

ExperimentData & ExperimentData::operator=(const ExperimentData * p) {
	if (this != p) {  // make sure not same object

		GUIhandle = p->GUIhandle;

		experimentInProgress = p->experimentInProgress;
		experimentType = p->experimentType;
		experimentDose = p->experimentDose;

		dataGeneral = p->dataGeneral;
		dataDivers = p->dataDivers;
		dataDesorption = p->dataDesorption;

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
