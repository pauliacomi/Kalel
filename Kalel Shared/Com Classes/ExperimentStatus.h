#pragma once

#include "../classHelpers.h"

class ExperimentStatus
{
public:
	ExperimentStatus();
	ExperimentStatus(const ExperimentStatus & p);										// Constructor does NOT change timestamp
	~ExperimentStatus() = default;
	ExperimentStatus & ExperimentStatus::operator=(const ExperimentStatus & p);			// Equality operator changes timestamp

	void ResetData();
	
protected:
	void Replace(const ExperimentStatus & p);

	///*******************
	///		Vars
	///*******************

public:
	atomic_time_point tp;
	
	///*******************
	///		Global flags
	///*******************

	atomic_ts<bool> inProgress{ tp };								// Boolean stating whether the experiment is started or not
	atomic_ts<bool> isRecording{ tp };								// Boolean stating whether the data is being recorded at each measurement
	atomic_ts<bool> isWaiting{ tp };								// Boolean stating the experiment is currently waiting
	atomic_ts<bool> isWaitingUser{ tp };							// Boolean stating the experiment is currently waiting
	atomic_ts<bool> isRunningAutomation{ tp };						// Boolean stating that any automation is requested (manual,automatic etc)

	
	///*******************
	///		Parameters for storing where program has reached
	///*******************

	atomic_ts<int> mainStage{ tp };									// Current experiment stage, the main part of the program (verification, equilibration, adsorption, desorption etc.)
	atomic_ts<int> stepStatus{ tp };								// Current step status, for each step this can be STARTING, ENDING or IN_PROGRESS
	atomic_ts<int> substepStatus{ tp };								// Current dose substep, can be at injection, equilibration etc
	atomic_ts<int> injectionDose{ tp };								// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)

	// Time
	atomic_ts<unsigned long long> timeStart{ tp };						// When the experiment started
	atomic_ts<unsigned long long> timeEquilibrationStart{ tp };			// Time equilibration started
	atomic_ts<double> timeToEquilibrate{ tp };							// The amount of time that the waiting + recording functionality will run before returning to an active state
	
	// Counters
	atomic_ts<int> injectionAttemptCounter{ tp };						// Counter for the injections
	atomic_ts<int> adsorptionCounter{ tp };								// Counter for the number of adsorption settings (small, medium, large etc) inputted by the user, starts at 0
	atomic_ts<int> desorptionCounter{ tp };								// Counter for the number of desorption settings (small, medium, large etc) inputted by the user, starts at 0
	
	// Pressures used as benchmarks
	atomic_ts<double> pressureInitial{ tp };							// Pressure set as an initial pressure in injections
	atomic_ts<double> pressureFinal{ tp };								// Pressure set as the final pressure in injections
	atomic_ts<double> pressureHighOld{ tp };							// Previous pressure stored for injection checks

};
