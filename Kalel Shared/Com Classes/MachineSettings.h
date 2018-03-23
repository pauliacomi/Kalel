#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>		// for timepoint
#include <mutex>		// for mutex

#include "ExperimentHelperClasses.h"

class MachineSettings
{
public:
	MachineSettings();
	MachineSettings(const MachineSettings&);
	~MachineSettings();

	MachineSettings & operator=(const MachineSettings & p);

public:
	///*******************
	///		Settings
	///*******************

	std::wstring CaloName = L"UnnamedCalo";
	std::wstring CaloPrefix = L"Prefix";
	std::wstring DefaultPath = L"C:";

	bool hasSonicNozzle = false;							// Used for user information (no calculations)
	bool SafetyOn = true;									// Used for activating the automatic security
	float VolumeRef = 1;									// Used for user information (no calculations)
	float VolumeP6 = 1;										// Used for user information (no calculations)

	int InjectionAttemptNumber = 5;
	float InjectionMargin = 0.01f;
	float InjectionMultiplier = 1.2f;

	int TimeBetweenMeasurement = 4000;						// milliseconds
	int TimeBetweenRecording = 500;							// milliseconds
	int TimeBetweenAutomation = 10;							// milliseconds
	int TimeWaitValves = 5;									// seconds
	int TimeWaitValvesShort = 2;							// seconds
	int TimeWaitPump = 10;									// seconds
	int TimeVacuumEmergency = 6000;							// seconds
	int TimeVacuumBottle = 300;								// seconds
	int TimeVacuumEndDefault = 12000;						// seconds

	float PressurePumpVacuum = 0.5f;
	float PressureLimitVacuum = 0.01f;						// Used for determining the pressure considered "good vacuum"

	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available

	// Functions to easily create instruments or readers
	void AddInstrument(const Instrument &i, unsigned int position = 0);
	void AddReader(const Reader &r, unsigned int position = 0);
	void AddController(const Controller &r, unsigned int position = 0);
};