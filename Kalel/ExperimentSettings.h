#pragma once
#ifndef EXPERIMENT_SETTINGS
#define EXPERIMENT_SETTINGS

#include "Classes_experiences.h"
#include <vector>

class ExperimentSettings
{
public:
	ExperimentSettings(void);
	~ExperimentSettings(void);

public:

	// Critical Section
	CRITICAL_SECTION criticalSection;

	// Main GUI handle
	HWND GUIhandle;

	// General parameters
	int experimentType;

	// Data modified boolean
	bool dataModified;
	bool continueAnyway;

	// Experimental data storage

	Donnees_General dataGeneral;
	Donnees_Divers dataDivers;
	vector<Donnees_Doses> dataAdsorption;
	vector<Donnees_Desorption> dataDesorption;

	// Overload equals function
	ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p);
};

#endif