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
	Storage(void);
	~Storage(void);

	//******************************************************************************************
	// Logs
	//******************************************************************************************
public:
	// Debug logs
	StampedSafeStorage<std::string> debugLogs;
	
	// Automation logs
	StampedSafeStorage<std::string> infoLogs;

	// Requests and interactions
	StampedSafeStorage<std::string> eventLogs;


	//******************************************************************************************
	// Machine Settings
	//******************************************************************************************

	std::mutex machineSettingsMutex;
	MachineSettings machineSettings;															// The machine settings are here

public:
	void setmachineSettings(MachineSettings m) {
		std::unique_lock<std::mutex> lock(machineSettingsMutex);
		machineSettings = m;
	}


	//******************************************************************************************
	// Experiment Settings
	//******************************************************************************************

	std::mutex experimentSettingsMutex;																			// Synchronisation class, should be used whenever there are writes
	ExperimentSettings experimentSettings;														// The experiment settings are here

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
	// Data
	//******************************************************************************************
public:
	StampedSafeStorage<ExperimentData>  dataCollection;															// The collection of data from an experiment

	ExperimentData currentData;

	//******************************************************************************************
	// Experiment Status
	//******************************************************************************************
public:																											// A collection of info about the experiment progress 
	ExperimentStatus experimentStatus;

	//******************************************************************************************
	// Control State
	//******************************************************************************************

	std::chrono::system_clock::time_point controlStateChanged = timeh::NowTime();								// Time when control state changed

	//******************************************************************************************
	// Automation control
	//******************************************************************************************

	// mutex for thread notification
	std::mutex automationMutex;
	std::condition_variable automationControl;
};

// Initializer for class
inline Storage::Storage(void)
{
	//
	// Check to see whether the parameters file has been created
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

inline Storage::~Storage(void)
{
}