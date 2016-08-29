#pragma once
#ifndef EXPERIMENT_SETTINGS
#define EXPERIMENT_SETTINGS

#include "Classes_experiences.h"
#include "../Com Classes/MachineSettings.h"
#include <vector>

class ExperimentSettings
{

	//
	// Functions
	//

public:
	ExperimentSettings(const MachineSettings & machineSettings);
	ExperimentSettings(int initialAdsorptions, int initialDesorptions, const MachineSettings & machineSettings);
	~ExperimentSettings(void);

	void ResetData();
	void ResetData(int initialAdsorptions, int initialDesorptions);


	// Overload equals function
	ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p);

	//
	// Variables
	//


protected:
	MachineSettings * settings;

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