#pragma once

#include "ExperimentHelperClasses.h"

#include <string>
#include <map>
#include <chrono>		// for timepoint


class MachineSettings
{
public:
	MachineSettings();
	MachineSettings(const MachineSettings&);												// Constructor does NOT change timestamp
	~MachineSettings() = default;
	MachineSettings & operator=(const MachineSettings & p);									// Equality operator changes timestamp

	void ResetData();

protected:
	void Replace(const MachineSettings & p);

	///*******************
	///		Vars
	///*******************

public:

	std::chrono::system_clock::time_point tp;

	///*******************
	///		Settings
	///*******************

	std::wstring CaloName;
	std::wstring CaloPrefix;
	std::wstring DefaultPath;

	bool hasSonicNozzle;
	bool SafetyOn;											// Used for activating the automatic security
	float VolumeRef;										// Used for user information (no calculations)
	float VolumeP6;											// Used for user information (no calculations)

	int InjectionAttemptNumber;
	float InjectionMargin;
	float InjectionMultiplier;

	int TimeBetweenMeasurement;								// milliseconds
	int TimeBetweenRecording;								// milliseconds
	int TimeBetweenAutomation;								// milliseconds
	int TimeWaitValves;										// seconds
	int TimeWaitValvesShort;								// seconds
	int TimeWaitPump;										// seconds
	int TimeVacuumEmergency;								// seconds
	int TimeVacuumBottle;									// seconds
	int TimeVacuumEndDefault;								// seconds

	float PressurePumpVacuum;
	float PressureLimitVacuum;								// Used for determining the pressure considered "good vacuum"

	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available
};