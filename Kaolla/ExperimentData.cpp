#include "stdafx.h"
#include "ExperimentData.h"


ExperimentData::ExperimentData()
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
