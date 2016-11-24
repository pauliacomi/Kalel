#ifndef EXPERIMENT_SETTINGS
#define EXPERIMENT_SETTINGS
#pragma once

#include "ExperimentHelperClasses.h"

#include <vector>

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


	// Overload equals function
	ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p);

	//
	// Variables
	//


public:
	//HWND GUIhandle;						// Main GUI handle
	int experimentType;					// General parameters

	// Settings storage
	Donnees_General dataGeneral;
	Donnees_Divers dataDivers;
	std::vector<Donnees_Doses> dataAdsorption;
	std::vector<Donnees_Desorption> dataDesorption;
};

#endif