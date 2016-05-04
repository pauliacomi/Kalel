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

	// Main GUI handle
	HWND GUIhandle;

	// General parameters
	bool experimentInProgress;
	bool dataModified;
	int experimentType;

	// Parameters for storing where program has reached

	int experimentStage;					// Current experiment stage, the main part of the program (equilibration, adsorption, desorption etc.)
	int experimentPreviousStage;			// Previous experiment stage
	int experimentDose;						// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)
	int experimentStepStatus;				// Current step status, for each step this can be STARTING, ENDING or UNDERWAY
	int experimentEquilibrationStatus;		// 
	int experimentSubstepStage;				// Current dose substep, can be at injection, equilibration etc
	int experimentMeasurement;				// Number of measurmements made, happen every T_BETWEEN_MEASUREMENT
	int verificationStep;					// The security check steps

	// Time
	float experimentTime;					// Time from the experiment start
	float timeToEquilibrate;				// Number of measurmements made, happen every T_BETWEEN_MEASUREMENT

	// Counters
	int injectionAttemptCounter;			// Counter for the injections
	int adsorptionCounter;					// Counter for the number of adsorption settings inputted by the user
	int desorptionCounter;					// Counter for the number of desorption settings inputted by the user

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
	double pressureHigh;					// Pressure recorded from the high PT
	double pressureLow;						// Pressure recorded from the low PT
	double pressureInitial;					// Pressure set as an initial pressure in injections
	double pressureFinal;					// Pressure set as the final pressure in injections
	double pressureHighOld;					// Previous pressure stored for injection checks
	
	// Temperature
	double temperatureCalo;
	double temperatureCage;
	double temperatureRoom;
	

	// Overload equals function
	ExperimentData & ExperimentData::operator=(const ExperimentData * p);
};

