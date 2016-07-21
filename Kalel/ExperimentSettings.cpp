#include "StdAfx.h"
#include "ExperimentSettings.h"

#include "DefineStages.h"


ExperimentSettings::ExperimentSettings(void)
: GUIhandle(NULL)
, dataModified(false)
, continueAnyway(false)
, experimentType(EXPERIMENT_TYPE_UNDEF)
{
	// Initialisation of the critical section
	InitializeCriticalSection(&criticalSection);
}

ExperimentSettings::~ExperimentSettings(void)
{
	// Destroy the critical sections
	DeleteCriticalSection(&criticalSection);
}

void ExperimentSettings::ResetData()
{
	experimentType = EXPERIMENT_TYPE_UNDEF;
}

ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p) {
	if (this != p) {  // make sure not same object

		GUIhandle = p->GUIhandle;
		experimentType = p->experimentType;
		dataModified = p->dataModified;

		dataGeneral = p->dataGeneral;
		dataDivers = p->dataDivers;
		dataAdsorption = p->dataAdsorption;
		dataDesorption = p->dataDesorption;

	}
	return *this;    // Return ref for multiple assignment
}
