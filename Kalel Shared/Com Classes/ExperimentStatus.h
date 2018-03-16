#pragma once

#include <atomic>
#include <chrono>	// for timepoint
#include <mutex>

template<typename T>
class Attribute
{
public:
	Attribute(std::mutex & mtx, std::chrono::system_clock::time_point & tc);
protected:
	std::mutex & mtx;
	std::chrono::system_clock::time_point & tc;
	T val;
public:
	T get() {
		std::lock_guard<std::mutex> lock(mtx);
		return val; 
	}
	void set(T & v) {
		std::lock_guard<std::mutex> lock(mtx);
		tc = timeh::NowTime();
		val = v; 
	}
	T & operator=(const T &rhs);
	T & operator==(const T &rhs);
};

template<typename T>
inline Attribute<T>::Attribute(std::mutex & mtx, std::chrono::system_clock::time_point & tc)
	: mtx(mtx), tc(tc){}

template<typename T>
inline T & Attribute<T>::operator=(const T & rhs)
{
	if (this != &rhs) {
		this->set(rhs)
	}
	return *this;
}

template<typename T>
inline T & Attribute<T>::operator==(const T & rhs)
{
	return val == rhs;
}

class ExperimentStatus
{

	//
	// Functions
	//

public:
	ExperimentStatus(); 
	~ExperimentStatus(void);

	void ResetData();
	
private:
	///*******************
	///		Sync tools
	///*******************

	std::mutex mtx;															// Synchronisation mutex
	std::chrono::system_clock::time_point timeChanged;						// Time when experiment status changed

public:
	///*******************
	///		Global flags
	///*******************

	Attribute<bool> experimentInProgress{ mtx, timeChanged };					// Boolean stating whether the experiment is started or not
	Attribute<bool> experimentRecording{ mtx, timeChanged };					// Boolean stating whether the data is being recorded at each measurement
	Attribute<bool> experimentWaiting{ mtx, timeChanged };						// Boolean stating the experiment is currently waiting
	Attribute<bool> experimentCommandsRequested{ mtx, timeChanged };			// Boolean stating that any automation is requested (manual,automatic etc)

	
	///*******************
	///		Parameters for storing where program has reached
	///*******************

	Attribute<int> experimentStage{ mtx, timeChanged };							// Current experiment stage, the main part of the program (verification, equilibration, adsorption, desorption etc.)
	Attribute<int> verificationStep{ mtx, timeChanged };						// The security check steps
	Attribute<int> experimentStepStatus{ mtx, timeChanged };					// Current step status, for each step this can be STARTING, ENDING or IN_PROGRESS
	Attribute<int> experimentSubstepStage{ mtx, timeChanged };					// Current dose substep, can be at injection, equilibration etc
	Attribute<int> experimentDose{ mtx, timeChanged };							// Current experiment dose that is currently underway (ex: 2nd dose of adsorption step 1)
	Attribute<int> experimentPreviousStage{ mtx, timeChanged };					// Previous experiment stage

	// Time
	Attribute<unsigned long long> timeStart{ mtx, timeChanged };				// When the experiment started
	Attribute<unsigned long long> timeEquilibrationStart{ mtx, timeChanged };	// Time equilibration started
	Attribute<double> timeToEquilibrate{ mtx, timeChanged };					// The amount of time that the waiting + recording functionality will run before returning to an active state
	
	// Counters
	Attribute<int> injectionAttemptCounter{ mtx, timeChanged };					// Counter for the injections
	Attribute<int> adsorptionCounter{ mtx, timeChanged };						// Counter for the number of adsorption settings (small, medium, large etc) inputted by the user, starts at 0
	Attribute<int> desorptionCounter{ mtx, timeChanged };						// Counter for the number of desorption settings (small, medium, large etc) inputted by the user, starts at 0
	
	// Pressures used as benchmarks
	Attribute<double> pressureInitial{ mtx, timeChanged };						// Pressure set as an initial pressure in injections
	Attribute<double> pressureFinal{ mtx, timeChanged };						// Pressure set as the final pressure in injections
	Attribute<double> pressureHighOld{ mtx, timeChanged };						// Previous pressure stored for injection checks
};
