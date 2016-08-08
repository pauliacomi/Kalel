#pragma once
#ifndef EXPERIMENT_SETTINGS
#define EXPERIMENT_SETTINGS

#include "Classes_experiences.h"
#include <vector>

class ExperimentSettings
{
public:
	ExperimentSettings();
	ExperimentSettings(int initialAdsorptions, int initialDesorptions);
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
	LRESULT continueResult;

	// Settings storage

	Donnees_General dataGeneral;
	Donnees_Divers dataDivers;
	vector<Donnees_Doses> dataAdsorption;
	vector<Donnees_Desorption> dataDesorption;

	void ResetData();
	void ResetData(int initialAdsorptions, int initialDesorptions);

	// Overload equals function
	ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p);
};

#endif