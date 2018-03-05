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

	std::chrono::system_clock::time_point machineSettingsChanged;												// Time when machine settings changed
	std::mutex machineSettingsMutex;
	std::shared_ptr<MachineSettings> machineSettings;															// The machine settings are here

public:
	void setmachineSettings(std::shared_ptr<MachineSettings> i) {
		std::unique_lock<std::mutex> lock(machineSettingsMutex);
		machineSettings = i;
		machineSettingsChanged = timeh::NowTime();
		lock.unlock();
	}

	//******************************************************************************************
	// Control State
	//******************************************************************************************
	
	std::chrono::system_clock::time_point controlStateChanged;													// Time when control state changed
	
	//******************************************************************************************
	// Experiment Status
	//******************************************************************************************

	std::chrono::system_clock::time_point experimentStatusChanged;												// Time when experiment status changed
	std::shared_ptr<ExperimentStatus> experimentStatus;

	//******************************************************************************************
	// Experiment Settings
	//******************************************************************************************

	std::chrono::system_clock::time_point experimentSettingsChanged;											// Time when experiment settings changed
	std::mutex experimentSettingsMutex;																			// Synchronisation class, should be used whenever there are writes
	std::shared_ptr<ExperimentSettings> experimentSettings;														// The experiment settings are here
	std::mutex newexperimentSettingsMutex;																		// Synchronisation class, should be used whenever there are writes
	std::shared_ptr<ExperimentSettings> newExperimentSettings;													// The new experiment settings

public:
	void setExperimentSettings(std::shared_ptr<ExperimentSettings> i) {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		experimentSettings = i;
		experimentSettingsChanged = timeh::NowTime();
	}

	void resetExperimentSettings() {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		experimentSettings->ResetData();
		experimentSettingsChanged = timeh::NowTime();
	}

	void setnewExperimentSettings(std::shared_ptr<ExperimentSettings> i) {
		std::unique_lock<std::mutex> lock(newexperimentSettingsMutex);
		newExperimentSettings = i;
	}


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
	// Populate Machine Settings
	machineSettings = std::make_shared<MachineSettings>();
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

	currentData = std::make_shared<ExperimentData>();
	experimentStatus = std::make_shared<ExperimentStatus>();
	experimentSettings = std::make_shared<ExperimentSettings>();
	newExperimentSettings = std::make_shared<ExperimentSettings>();

	// initialise control state times
	controlStateChanged = timeh::NowTime();
	machineSettingsChanged = timeh::NowTime();
	experimentSettingsChanged = timeh::NowTime();

	// Set path
	experimentSettings->dataGeneral.chemin = machineSettings->CheminFichierGeneral;
	newExperimentSettings->dataGeneral.chemin = machineSettings->CheminFichierGeneral;
}

inline Storage::~Storage(void)
{
}