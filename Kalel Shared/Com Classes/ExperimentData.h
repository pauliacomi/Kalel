#ifndef EXPERIMENT_DATA
#define EXPERIMENT_DATA
#pragma once

#include <vector>
#include <ctime>


class ExperimentData
{
public:
	ExperimentData();
	~ExperimentData();


	///*******************
	///		Global flags
	///*******************

	bool experimentInProgress;				// Boolean stating whether the experiment is started or not
	bool experimentRecording;				// Boolean stating whether the data is being recorded at each measurement
	bool experimentWaiting;					// Boolean stating the experiment is currently waiting
	bool experimentCommandsRequested;		// Boolean stating that any automation is requested (manual,automatic etc)

	
	///*******************
	///		Parameters for storing where program has reached
	///*******************

	int experimentStage;					// Current experiment stage, the main part of the program (verification, equilibration, adsorption, desorption etc.)
	int verificationStep;					// The security check steps
	int experimentStepStatus;				// Current step status, for each step this can be STARTING, ENDING or IN_PROGRESS
	int experimentSubstepStage;				// Current dose substep, can be at injection, equilibration etc
	int experimentDose;						// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)
	int experimentGraphPoints;				// Number of measurmements made, happen every T_BETWEEN_MEASUREMENT
	int experimentPreviousStage;			// Previous experiment stage

	// Time
	time_t timeStart;						// When the experiment started
	long int measurementsMade;				// Number of measurements recorded since the experiment was started
	double timeElapsed;						// Time from the experiment start
	double timeToEquilibrate;				// The amount of time that the waiting + recording functionality will run before returning to an active state
	double timeToEquilibrateCurrent;		// The current time of waiting
	
	// Counters
	int injectionAttemptCounter;			// Counter for the injections
	int adsorptionCounter;					// Counter for the number of adsorption settings (small, medium, large etc) inputted by the user, starts at 0
	int desorptionCounter;					// Counter for the number of desorption settings (small, medium, large etc) inputted by the user, starts at 0
	
	// Pressures used as benchmarks
	double pressureInitial;					// Pressure set as an initial pressure in injections
	double pressureFinal;					// Pressure set as the final pressure in injections
	double pressureHighOld;					// Previous pressure stored for injection checks


	///*******************
	///		Recorded Data
	///*******************

	// Calorimeter
	double resultCalorimeter;

	// Pressure
	double pressureHigh;					// Pressure recorded from the high PT
	double pressureLow;						// Pressure recorded from the low PT
	
	// Temperature
	double temperatureCalo;					// Temperature recorded by the Calorimeter
	double temperatureCage;					// Temperature recorded in the enclosure
	double temperatureRoom;					// Temperature recorded in the room
	


	///*******************
	///		Functions
	///*******************

	// Function to reset
	void ResetData();

	// Overload equals function
	ExperimentData & ExperimentData::operator=(const ExperimentData * p);
};

typedef std::vector<ExperimentData*> MeasurementsArray;

#endif