#ifndef COMPTR_H
#define COMPTR_H
#pragma once

// Required to pass the experimental data to the main GUI
#include "../../Kalel Shared/Com Classes/ExperimentData.h"
#include "../../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../../Kalel Shared/Com Classes/MachineSettings.h"


#include <mutex>
#include <vector>
#include <map>
#include <deque>
#include <memory>
#include <chrono>

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
	std::map<std::chrono::system_clock::time_point, std::string> automationInfoLogs;					// All non-error logs are stored here

public:
	void pushInfoLogs(std::chrono::system_clock::time_point time, std::string value) {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		automationInfoLogs.insert(std::make_pair(time, value));
	}

	std::map<std::chrono::system_clock::time_point, std::string> getInfoLogs() {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		return automationInfoLogs;
	}

	//**********
	// Requests and error interactions
	//**********
private:
	std::mutex autoReqMutex;																				// Synchronisation class, should be used whenever there are writes to the logs
	std::map<std::chrono::system_clock::time_point, std::string> automationErrorLogs;						// All error logs are stored here

public:
	void pushErrLogs(std::chrono::system_clock::time_point time, std::string value) {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		automationErrorLogs.insert(std::make_pair(time, value));
	}

	std::map<std::chrono::system_clock::time_point, std::string> getErrLogs() {
		std::unique_lock<std::mutex> lock(autoReqMutex);
		return automationErrorLogs;
	}

	//**********
	// Data
	//**********
private:
	std::mutex sharedMutex;																					// Synchronisation class, should be used whenever there are writes to the deque
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

	void deleteData() {
		std::unique_lock<std::mutex> lock(sharedMutex);
		dataCollection.clear();
	}

	//**********
	// Settings
	//**********

	std::mutex machineSettingsMutex;
	std::shared_ptr<MachineSettings> machineSettings;															// The machine settings are here


	std::mutex experimentSettingsMutex;																			// Synchronisation class, should be used whenever there are writes to the deque
	std::shared_ptr<ExperimentSettings> experimentSettings;														// The experiment settings are here
	std::shared_ptr<ExperimentSettings> newExperimentSettings;													// The new experiment settings

public:
	void setnewExperimentSettings(std::shared_ptr<ExperimentSettings> i) {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		newExperimentSettings = i;
	}


	//**********
	// Automation control
	//**********

	std::mutex automationMutex;
	std::condition_variable automationControl;
};

// Initializer for class
inline Storage::Storage(void)
{
	machineSettings = std::make_shared<MachineSettings>();
	currentData = std::make_shared<ExperimentData>();
	experimentSettings = std::make_shared<ExperimentSettings>();
	newExperimentSettings = std::make_shared<ExperimentSettings>();
}

inline Storage::~Storage(void)
{
}

#endif