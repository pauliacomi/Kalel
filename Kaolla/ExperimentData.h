#pragma once

#include "Classes_experiences.h"
#include <vector>

class ExperimentData
{
public:
	ExperimentData();
	~ExperimentData();

// Variables
//**************************************************

	// General parameters
	int experimentType;
	int experimentStep;
	

	// Experimental data storage

	Donnees_General dataGeneral;
	Donnees_Divers dataDivers;
	vector<Donnees_Doses> dataAdsorption;
	Donnees_Desorption dataDesorption;

	// Text
	CString textMessages;
	CString textMeasurements;

	// Pressure
	double pressureHigh;
	double pressureLow;
	double pressureInitial;
	double pressureFinal;
	
	// Temperature
	double temperatureCalo;
	double temperatureCage;
	double temperatureRoom;
	
	// Time
	double time;

};

