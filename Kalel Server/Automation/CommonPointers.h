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
#include <deque>
#include <memory>
#include <chrono>

typedef std::map<std::chrono::system_clock::time_point, std::string> TextStorage;

class Storage {
public:
	Storage(void);
	~Storage(void);

	//**********
	// Server logs
	//**********

public:
	std::mutex serverLogsMtx;																					// Mutex for the server logs					
	std::vector<std::string> serverLogs;																		// Logs from the server are stored here

	//**********
	// Automation logs
	//**********
private:
	std::mutex autoInfoLogsMutex;																				// Synchronisation class, should be used whenever there are writes to the logs
	TextStorage automationInfoLogs;					// All non-error logs are stored here

public:
	void pushInfoLogs(std::chrono::system_clock::time_point time, std::string value) {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		automationInfoLogs.insert(std::make_pair(time, value));
	}

	TextStorage getInfoLogs() {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		return automationInfoLogs;
	}

	TextStorage getInfoLogs(const std::chrono::system_clock::time_point & tp) {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		auto start = automationInfoLogs.upper_bound(tp);
		auto end = automationInfoLogs;
		return TextStorage(automationInfoLogs.upper_bound(tp), automationInfoLogs.end());
	}

	//**********
	// Requests and error interactions
	//**********
private:
	std::mutex autoReqMutex;																				// Synchronisation class, should be used whenever there are writes to the logs
	TextStorage automationErrorLogs;						// All error logs are stored here

public:
	void pushErrLogs(std::chrono::system_clock::time_point time, std::string value) {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		automationErrorLogs.insert(std::make_pair(time, value));
	}

	TextStorage getErrLogs() {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		return automationErrorLogs;
	}

	TextStorage getErrLogs(const std::chrono::system_clock::time_point & tp) {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		auto start = automationErrorLogs.upper_bound(tp);
		auto end = automationErrorLogs;
		return TextStorage(automationErrorLogs.upper_bound(tp), automationErrorLogs.end());
	}

	//**********
	// Data
	//**********
private:
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

	//**********
	// Machine Settings
	//**********

	std::chrono::system_clock::time_point machineSettingsChanged;												// Time when machine settings changed
	std::mutex machineSettingsMutex;
	std::shared_ptr<MachineSettings> machineSettings;															// The machine settings are here

public:
	void setmachineSettings(std::shared_ptr<MachineSettings> i) {
		std::unique_lock<std::mutex> lock(machineSettingsMutex);
		machineSettings = i;
		machineSettingsChanged = timeh::NowTime();
	}

	//**********
	// Experiment Settings
	//**********

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


	//**********
	// Automation control
	//**********

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
	ParametersGet(*machineSettings);

	currentData = std::make_shared<ExperimentData>();
	experimentSettings = std::make_shared<ExperimentSettings>();
	newExperimentSettings = std::make_shared<ExperimentSettings>();

	machineSettingsChanged = timeh::NowTime();
	experimentSettingsChanged = timeh::NowTime();
}

inline Storage::~Storage(void)
{
}

#endif