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

	// Debug logs
	StampedSafeStorage<std::string> debugLogs;
	
	// Automation logs
	StampedSafeStorage<std::string> infoLogs;

	// Requests and interactions
	StampedSafeStorage<std::string> eventLogs;

	//******************************************************************************************
	// Data
	//******************************************************************************************
public:
	StampedSafeStorage<std::shared_ptr<ExperimentData>>  dataCollection;										// The collection of data from an experiment

public:
	std::shared_ptr<ExperimentData> currentData;

	//******************************************************************************************
	// Machine Settings
	//******************************************************************************************

	std::mutex machineSettingsMutex;
	std::shared_ptr<MachineSettings> machineSettings;															// The machine settings are here

public:
	void setmachineSettings(std::shared_ptr<MachineSettings> i) {
		std::unique_lock<std::mutex> lock(machineSettingsMutex);
		machineSettings = i;
	}

	
	//******************************************************************************************
	// Experiment Status
	//******************************************************************************************

	std::shared_ptr<ExperimentStatus> experimentStatus;

	//******************************************************************************************
	// Experiment Settings
	//******************************************************************************************

	std::mutex experimentSettingsMutex;																			// Synchronisation class, should be used whenever there are writes
	std::shared_ptr<ExperimentSettings> experimentSettings;														// The experiment settings are here

public:
	void setExperimentSettings(std::shared_ptr<ExperimentSettings> i) {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		experimentSettings = i;
	}

	void resetExperimentSettings() {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		experimentSettings->ResetData();
	}

	//******************************************************************************************
	// Control State
	//******************************************************************************************

	std::chrono::system_clock::time_point controlStateChanged;													// Time when control state changed

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
	// Make classes
	machineSettings = std::make_shared<MachineSettings>();
	currentData = std::make_shared<ExperimentData>();
	experimentStatus = std::make_shared<ExperimentStatus>();
	experimentSettings = std::make_shared<ExperimentSettings>();

	// initialise control state times
	controlStateChanged = timeh::NowTime();

	//
	// Check to see whether the parameters file has been created
	if (!ParametersCheck())
	{
		ParametersSet(*machineSettings);		// If not, create it
	}
	else
	{
		ParametersGet(*machineSettings);		// Or get it
	}

	// Set path
	experimentSettings->dataGeneral.chemin = machineSettings->CheminFichierGeneral;
}

inline Storage::~Storage(void)
{
}