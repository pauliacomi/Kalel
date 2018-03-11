#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>		// for timepoint
 
#include "ExperimentHelperClasses.h"

class MachineSettings
{
public:
	MachineSettings();
	MachineSettings(const MachineSettings & p);
	~MachineSettings();

	void ResetData();

	// Overload equals function
	MachineSettings & MachineSettings::operator=(const MachineSettings * p);

	///*******************
	///		Sync tools
	///*******************

	std::chrono::system_clock::time_point timeChanged;		// Time when machine settings changed

	///*******************
	///		Settings
	///*******************

	std::wstring CaloName = std::to_wstring(1);
	std::wstring CaloEntete = std::to_wstring(1);
	std::wstring CheminFichierGeneral = std::to_wstring(1);

	bool ActivationSecurite;								// Used for activating the automatic security
	float PressionSecuriteBassePression;					// Considered the maximum pressure for the low range pressure transmitter
	float PressionSecuriteHautePression;					// Considered the maximum pressure for the high range pressure transmitter

	float PressionLimiteVide;								// Used for determining the pressure considered "good vacuum"
	
	bool hasSonicNozzle;									// Used for user information (no calculations)
	float VolumeRef;										// Used for user information (no calculations)
	float VolumeP6;											// Used for user information (no calculations)

	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available


	// Functions to easily create instruments or readers
	void AddInstrument(Instrument i, unsigned int position = 0);
	void AddReader(Reader r, unsigned int position = 0);
	void AddController(Controller r, unsigned int position = 0);
};