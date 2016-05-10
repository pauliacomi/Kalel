#pragma once
#ifndef EXPERIMENT_DATA
#define EXPERIMENT_DATA

class ExperimentData
{
public:
	ExperimentData();
	~ExperimentData();

// Variables
//**************************************************

	bool experimentInProgress;				// Boolean for checking whether the experiment is started or not
	bool experimentRecording;				// Boolean for checking whether the data is being recorded at each measurement
	bool experimentWaiting;					// Boolean for 
	bool experimentCommandsRequested;		// Boolean for 

	// Parameters for storing where program has reached

	int experimentStage;					// Current experiment stage, the main part of the program (equilibration, adsorption, desorption etc.)
	int experimentPreviousStage;			// Previous experiment stage
	int experimentDose;						// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)
	int experimentStepStatus;				// Current step status, for each step this can be STARTING, ENDING or UNDERWAY
	int experimentEquilibrationStatus;		// Current step in equlibration 
	int experimentSubstepStage;				// Current dose substep, can be at injection, equilibration etc
	int experimentMeasurements;				// Number of measurmements made, happen every T_BETWEEN_MEASUREMENT
	int verificationStep;					// The security check steps

	// Time
	float experimentTime;					// Time from the experiment start
	float timeToEquilibrate;				// The amount of time that the waiting + recording functionality will run before returning to an active state

	// Counters
	int injectionAttemptCounter;			// Counter for the injections
	int adsorptionCounter;					// Counter for the number of adsorption settings inputted by the user
	int desorptionCounter;					// Counter for the number of desorption settings inputted by the user

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

typedef CArray<ExperimentData, ExperimentData*> CArrayMesure;

#endif