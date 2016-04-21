#pragma once

#include "Classes_experiences.h"
#include <vector>

class ExperimentData
{
public:
	ExperimentData();
	~ExperimentData();

// Variables
//**************************************************

	// Critical Section
	CRITICAL_SECTION criticalSection;

	// General parameters
	bool experimentInProgress;
	bool dataModified;
	int experimentType;

	int verificationStep;					// The security check steps
	int stepStatus;							// The progress within each step
	int experimentDose;						// The dose that is currently underway
	int experimentMeasurement;				// Number of measurmements the machine has made
	int experimentStage;					// Current experiment stage
	int experimentPreviousStage;			// Current experiment stage

	// Experimental data storage

	Donnees_General dataGeneral;
	Donnees_Divers dataDivers;
	vector<Donnees_Doses> dataAdsorption;
	Donnees_Desorption dataDesorption;

	// Text
	CString textMessages;
	CString textMeasurements;

	// Calorimeter
	double resultCalorimeter;

	// Pressure
	double pressureHigh;
	double pressureLow;
	double pressureInitial;
	double pressureFinal;
	
	// Temperature
	double temperatureCalo;
	double temperatureCage;
	double temperatureRoom;
	
	// Time
	int experimentTime;						// Time from the experiment start

	// Overload equals function
	ExperimentData & ExperimentData::operator=(const ExperimentData * p);
};

