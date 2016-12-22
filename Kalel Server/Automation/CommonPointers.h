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

class Storage {
	//**********
	// Logs
	//**********
public:
	std::mutex serverLogsMtx;											// Mutex for the server logs					
	std::vector<std::string> serverLogs;								// Logs from the server are stored here

private:
	std::mutex autoInfoLogsMutex;										// Synchronisation class, should be used whenever there are writes to the logs
	std::map<std::string, std::string> automationInfoLogs;				// All non-error logs are stored here
	std::map<std::string, std::string> automationErrorLogs;				// All error logs are stored here

public:
	void pushInfoLogs(std::string one, std::string two) {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		automationInfoLogs.insert(std::make_pair(one, two));
	}

	std::map<std::string, std::string> getInfoLogs() {
		std::unique_lock<std::mutex> lock(autoInfoLogsMutex);
		return automationInfoLogs;
	}

	//**********
	// Data
	//**********
private:
	std::mutex sharedMutex;												// Synchronisation class, should be used whenever there are writes to the deque
	std::deque<std::shared_ptr<ExperimentData>> dataCollection;			// The collection of data from an experiment

public:
	std::shared_ptr<ExperimentData> currentData;

	void pushData(std::shared_ptr<ExperimentData> i) {
		std::unique_lock<std::mutex> lock(sharedMutex);
		dataCollection.push_back(i);
	}

	std::deque<std::shared_ptr<ExperimentData>> getData() {
		std::unique_lock<std::mutex> lock(sharedMutex);
		return dataCollection;
	}

	//**********
	// Settings
	//**********

	std::mutex machineSettingsMutex;
	std::shared_ptr<MachineSettings> machineSettings;					// The machine settings are here


	std::mutex experimentSettingsMutex;									// Synchronisation class, should be used whenever there are writes to the deque
	std::shared_ptr<ExperimentSettings> experimentSettings;				// The experiment settings are here
	std::shared_ptr<ExperimentSettings> newExperimentSettings;			// The new experiment settings

public:
	void setexperimentSettings(std::shared_ptr<ExperimentSettings> i) {
		std::unique_lock<std::mutex> lock(experimentSettingsMutex);
		newExperimentSettings = i;
	}
};



#endif