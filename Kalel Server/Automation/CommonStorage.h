#pragma once

// Required to pass the experimental data to the main GUI
#include "../../Kalel Shared/Com Classes/ExperimentData.h"
#include "../../Kalel Shared/Com Classes/ExperimentStatus.h"
#include "../../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../Kalel Shared/timeHelpers.h"
#include "../../Kalel Shared/classHelpers.h"
#include "../Parameters/Parametres.h"

#include <mutex>
#include <memory>


class Storage {
public:
	Storage() {
		if (!ParametersCheck())
		{
			ParametersSet(machineSettings);		// If not, create it
		}
		else
		{
			ParametersGet(machineSettings);		// Or get it
		}

		// Set path
		experimentSettings.dataGeneral.chemin = machineSettings.DefaultPath;
	}
	~Storage() {};


	//******************************************************************************************
	// Machine Settings
	//
	// Represents all the settings specific to the machine.
	//
	// Access		- it is accessed both by the measurement and automation threads continuously 
	//
	// Changes		- it can be changed by modifying parameters file and restarting
	//				- or by sending a full replacement from a client
	//
	// Warnings		- when changed the settings file should be updated
	//				- when changed the instruments might change (see CommonControls)
	//				- when changed the threads might clash unless properly signaled
	//
	// Solutions	- 
	//
	//******************************************************************************************

	MachineSettings machineSettings;

	//******************************************************************************************
	// Experiment Settings
	//
	// Represents all the settings specific to the experiment running.
	//
	// Access		- it is accessed by the automation thread continuously 
	//
	// Changes		- it can be changed by sending a full replacement from a client
	//				- it is reset at the end of an experiment
	//
	// Warnings		- when changed the measurement files should be updated with the changed settings
	//				- when changed the threads might clash unless properly signaled
	//
	// Solutions	- a mutex and two functions are provided for setting and resetting
	//
	//******************************************************************************************

	std::mutex experimentSettingsMutex;								
	ExperimentSettings experimentSettings;	

public:
	void setExperimentSettings(ExperimentSettings es) {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		experimentSettings = es;
	}

	void resetExperimentSettings() {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		experimentSettings.ResetData();
	}

	//******************************************************************************************
	// Experiment Status
	//
	// Describes the status of the experiment in progress.
	//
	// Access		- The automation thread writes to this class
	//				- The measurement thread reads from this class
	//				- The client both reads and can request some writes to this class
	//
	// Warnings		- The class is accessed by multiple threads
	//
	// Solutions	- All variables inside are atomic
	//
	//******************************************************************************************
public:
	ExperimentStatus experimentStatus;

	//******************************************************************************************
	// Control State
	//
	// Describes the status of the control instruments (valves, pumps etc).
	// Generated on the fly, whenever required therefore only time of change stored here.
	//
	//******************************************************************************************

	std::chrono::system_clock::time_point controlStateChanged = timeh::NowTime();

	//******************************************************************************************
	// Data
	//
	// A collection of all the data recorded.
	//
	// Access		- The array is only for storage and is accessed on client request. 
	// 				- The measurement thread deletes some of the storage if no experiment is running. 
	//				- The measurement and automation threads access the 'currentData'
	//
	// Changes		- It cannot be changed by a client, only internally
	//
	// Warnings		- The current data is accessed by automation and measurement simultaneously
	//				- The array is pushed to and retreived simultaneously
	//
	// Solutions	- Current data is stored in a struct of atomic variables -> no clashes
	// 				- Array of data is stored in a mutex-protected class -> no clashes
	//
	//******************************************************************************************

public:
	ExperimentAtomicData currentData;
	StampedSafeStorage<ExperimentData>  dataCollection;	

	//******************************************************************************************
	// Logs
	//
	// A collection of all the logs recorded.
	//
	// Access		- The arrays are only for storage and is accessed on client request.
	//
	// Changes		- Logging framework pushes to these arrays
	//				- It can be deleted on client request
	//
	// Warnings		- The arrays are accessed by multiple threads
	//
	// Solutions	- Array of data is stored in a mutex-protected class -> no clashes
	//
	//******************************************************************************************
public:
	// Debug logs
	StampedSafeStorage<std::string> debugLogs;

	// Automation logs
	StampedSafeStorage<std::string> infoLogs;

	// Requests and interactions
	StampedSafeStorage<std::string> eventLogs;


	//******************************************************************************************
	// Timers
	//
	//******************************************************************************************
	timeh::timer timerExperiment;											// Class for measuring the time from the experiment start
	timeh::timer timerMeasurement;											// Class for measuring the time between each measurement
	timeh::timer timerWaiting;												// Class for measuring the time to wait

	//******************************************************************************************
	// Automation control
	//
	// The mutex and condition variable used for automation thread signalling
	//
	//******************************************************************************************

	std::mutex automationMutex;
	std::condition_variable automationControl;


	std::mutex measurementMutex;
	std::condition_variable measurementControl;
};
