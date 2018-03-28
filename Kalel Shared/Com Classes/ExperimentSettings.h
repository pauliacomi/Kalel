#pragma once

#include "ExperimentHelperClasses.h"

#include <vector>
#include <chrono>	// for timepoint

class ExperimentSettings
{

	//
	// Functions
	//

public:
	ExperimentSettings();
	ExperimentSettings(int initialAdsorptions, int initialDesorptions);
	~ExperimentSettings(void);

	void ResetData();
	void ResetData(int initialAdsorptions, int initialDesorptions);

	ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p);

	//
	// Variables
	//
public:
	std::chrono::system_clock::time_point tp;				// Time when experiment settings changed

	int experimentType;					// Experiment type

	// Settings storage
	data_general dataGeneral;
	data_other dataDivers;
	std::vector<data_adsorption> dataAdsorption;
	std::vector<data_desorption> dataDesorption;
};
