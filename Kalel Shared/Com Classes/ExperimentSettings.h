#pragma once

#include "ExperimentHelperClasses.h"

#include <vector>
#include <chrono>	// for timepoint

class ExperimentSettings
{
public:
	ExperimentSettings();
	ExperimentSettings(const ExperimentSettings& );												// Constructor does NOT change timestamp
	~ExperimentSettings() = default;
	ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings & p);			// Equality operator changes timestamp

	void ResetData();

protected:
	void Replace(const ExperimentSettings & p);

	///*******************
	///		Vars
	///*******************
public:
	std::chrono::system_clock::time_point tp;				// Time when experiment settings changed

	int experimentType;					// Experiment type

	// Settings storage
	data_general dataGeneral;
	data_other dataDivers;
	std::vector<data_adsorption> dataAdsorption;
	std::vector<data_desorption> dataDesorption;
};
