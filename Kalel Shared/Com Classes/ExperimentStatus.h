#pragma once

#include <atomic>

class ExperimentStatus
{

	//
	// Functions
	//

public:
	ExperimentStatus(); 
	ExperimentStatus(const ExperimentStatus & p);
	~ExperimentStatus(void);

	void ResetData();
	

	// Overload equals function
	ExperimentStatus & ExperimentStatus::operator=(const ExperimentStatus * p);

	///*******************
	///		Global flags
	///*******************

	std::atomic_bool experimentInProgress;							// Boolean stating whether the experiment is started or not
	std::atomic_bool experimentRecording;							// Boolean stating whether the data is being recorded at each measurement
	std::atomic_bool experimentWaiting;								// Boolean stating the experiment is currently waiting
	std::atomic_bool experimentCommandsRequested;					// Boolean stating that any automation is requested (manual,automatic etc)

	
	///*******************
	///		Parameters for storing where program has reached
	///*******************

	std::atomic_int experimentStage;								// Current experiment stage, the main part of the program (verification, equilibration, adsorption, desorption etc.)
	std::atomic_int verificationStep;								// The security check steps
	std::atomic_int experimentStepStatus;							// Current step status, for each step this can be STARTING, ENDING or IN_PROGRESS
	std::atomic_int experimentSubstepStage;							// Current dose substep, can be at injection, equilibration etc
	std::atomic_int experimentDose;									// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)
	std::atomic_int experimentPreviousStage;						// Previous experiment stage

	// Time
	std::atomic_ullong timeStart;									// When the experiment started
	std::atomic<double> timeToEquilibrate;							// The amount of time that the waiting + recording functionality will run before returning to an active state
	std::atomic<double> timeToEquilibrateCurrent;					// The current time of waiting
	
	// Counters
	std::atomic_int injectionAttemptCounter;						// Counter for the injections
	std::atomic_int adsorptionCounter;								// Counter for the number of adsorption settings (small, medium, large etc) inputted by the user, starts at 0
	std::atomic_int desorptionCounter;								// Counter for the number of desorption settings (small, medium, large etc) inputted by the user, starts at 0
	
	// Pressures used as benchmarks
	std::atomic<double> pressureInitial;							// Pressure set as an initial pressure in injections
	std::atomic<double> pressureFinal;								// Pressure set as the final pressure in injections
	std::atomic<double> pressureHighOld;							// Previous pressure stored for injection checks

	///******************************************************************************************************************
	///
	///
	///
	///		Functions
	///
	///
	///******************************************************************************************************************


	bool GetexperimentInProgress()			const;					// Boolean stating whether the experiment is started or not
	bool GetexperimentRecording()			const;					// Boolean stating whether the data is being recorded at each measurement
	bool GetexperimentWaiting()				const;					// Boolean stating the experiment is currently waiting
	bool GetexperimentCommandsRequested()	const;					// Boolean stating that any automation is requested (manual,automatic etc)
	int GetexperimentStage()				const;					// Current experiment stage, the main part of the program (verification, equilibration, adsorption, desorption etc.)
	int GetverificationStep()				const;					// The security check steps
	int GetexperimentStepStatus()			const;					// Current step status, for each step this can be STARTING, ENDING or IN_PROGRESS
	int GetexperimentSubstepStage()			const;					// Current dose substep, can be at injection, equilibration etc
	int GetexperimentDose()					const;					// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)
	int GetexperimentPreviousStage()		const;					// Previous experiment stage
	unsigned long long GettimeStart()		const;					// When the experiment started
	double GettimeToEquilibrate()			const;					// The amount of time that the waiting + recording functionality will run before returning to an active state
	double GettimeToEquilibrateCurrent()	const;					// The current time of waiting
	int GetinjectionAttemptCounter()		const;					// Counter for the injections
	int GetadsorptionCounter()				const;					// Counter for the number of adsorption settings (small, medium, large etc) inputted by the user, starts at 0
	int GetdesorptionCounter()				const;					// Counter for the number of desorption settings (small, medium, large etc) inputted by the user, starts at 0
	double GetpressureInitial()				const;					// Pressure set as an initial pressure in injections
	double GetpressureFinal()				const;					// Pressure set as the final pressure in injections
	double GetpressureHighOld()				const;					// Previous pressure stored for injection checks

	void SetexperimentInProgress(bool a)						;
	void SetexperimentRecording(bool a)							;
	void SetexperimentWaiting(bool a)							;
	void SetexperimentCommandsRequested(bool a)					;
	void SetexperimentStage(int a)								;
	void SetverificationStep(int a)								;
	void SetexperimentStepStatus(int a)							;
	void SetexperimentSubstepStage(int a)						;
	void SetexperimentDose(int a)								;
	void SetexperimentPreviousStage(int a)						;
	void SettimeStart(unsigned long long  a)					;
	void SettimeToEquilibrate(double a)							;
	void SettimeToEquilibrateCurrent(double a)					;
	void SetinjectionAttemptCounter(int a)						;
	void SetadsorptionCounter(int a)							;
	void SetdesorptionCounter(int a)							;
	void SetpressureInitial(double a)							;
	void SetpressureFinal(double a)								;
	void SetpressureHighOld(double a)							;
};
