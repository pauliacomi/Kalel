#ifndef COMPTR_H
#define COMPTR_H
#pragma once

// Required to pass the experimental data to the main GUI
#include "../../Kalel Shared/Com Classes/ExperimentData.h"
#include "../../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../Kalel Shared/timeHelpers.h"
#include "../Parameters/Parametres.h"

#include <mutex>
#include <vector>
#include <map>
#include <memory>

typedef std::map<std::chrono::system_clock::time_point, std::string> TextStorage;

class Storage {
public:
	Storage(void);
	~Storage(void);

	//******************************************************************************************
	// Debug logs
	//******************************************************************************************

public:

	TextStorage debugLogs;																		// Logs for debug are stored here
	
	//******************************************************************************************
	// Automation logs
	//******************************************************************************************
public:
	std::mutex autoInfoLogsMutex;																// Synchronisation class, should be used whenever there are writes to the logs
	TextStorage infoLogs;																		// All non-error logs are stored here

public:
	void pushInfoLogs(std::chrono::system_clock::time_point time, std::string value) {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		infoLogs.insert(std::make_pair(time, value));
	}

	TextStorage getInfoLogs() {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		return infoLogs;
	}

	TextStorage getInfoLogs(const std::chrono::system_clock::time_point & tp) {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		auto start = infoLogs.upper_bound(tp);
		auto end = infoLogs;
		return TextStorage(infoLogs.upper_bound(tp), infoLogs.end());
	}

	//******************************************************************************************
	// Requests and error interactions
	//******************************************************************************************
public:
	std::mutex autoReqMutex;																				// Synchronisation class, should be used whenever there are writes to the logs
	TextStorage errorLogs;						// All error logs are stored here

public:
	void pushErrLogs(std::chrono::system_clock::time_point time, std::string value) {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		errorLogs.insert(std::make_pair(time, value));
	}

	TextStorage getErrLogs() {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		return errorLogs;
	}

	TextStorage getErrLogs(const std::chrono::system_clock::time_point & tp) {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		auto start = errorLogs.upper_bound(tp);
		auto end = errorLogs;
		return TextStorage(errorLogs.upper_bound(tp), errorLogs.end());
	}

	//******************************************************************************************
	// Data
	//******************************************************************************************
public:
	std::mutex sharedMutex;																					// Synchronisation class, should be used whenever there are writes
	ExperimentDataStorageArray dataCollection;																// The collection of data from an experiment

public:
	std::shared_ptr<ExperimentData> currentData;

	void pushData(std::chrono::system_clock::time_point time, std::shared_ptr<ExperimentData> value) {
		std::unique_lock<std::mutex> lock(sharedMutex);
		dataCollection.insert(std::make_pair(time, value));
	}

	ExperimentDataStorageArray getData() {
		std::unique_lock<std::mutex> lock(sharedMutex);
		return dataCollection;
	}

	ExperimentDataStorageArray getData(const std::chrono::system_clock::time_point & tp) {
		std::unique_lock<std::mutex> lock(sharedMutex);
		auto start = dataCollection.upper_bound(tp);
		auto end = dataCollection;
		return ExperimentDataStorageArray(dataCollection.upper_bound(tp), dataCollection.end());
	}

	void deleteData() {
		std::unique_lock<std::mutex> lock(sharedMutex);
		dataCollection.clear();
	}

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

#endif