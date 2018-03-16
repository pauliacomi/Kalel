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
	~MachineSettings();

private:
	///*******************
	///		Sync tools
	///*******************

	std::mutex mtx;											// Synchronisation mutex
	std::chrono::system_clock::time_point timeChanged;		// Time when machine settings changed

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

public:
	///*******************
	///		Getters
	///*******************

	std::wstring get_CaloName();
	std::wstring get_CaloPrefix();
	std::wstring get_DefaultPath();

	bool get_hasSonicNozzle();
	bool get_SafetyOn();
	float get_VolumeRef();
	float get_VolumeP6();

	int get_InjectionAttemptNumber();
	float get_InjectionMargin();
	float get_InjectionMultiplier();

	int get_TimeBetweenMeasurement();
	int get_TimeBetweenRecording();
	int get_TimeBetweenAutomation();
	int get_TimeWaitValves();
	int get_TimeWaitValvesShort();
	int get_TimeWaitPump();
	int get_TimeVacuumEmergency();
	int get_TimeVacuumBottle();
	int get_TimeVacuumEndDefault();

	float get_PressurePumpVacuum();
	float get_PressureLimitVacuum();

	///*******************
	///		Setters
	///*******************

	void set_CaloName(const std::wstring & s);
	void set_CaloPrefix(const std::wstring & s);
	void set_DefaultPath(const std::wstring & s);

	void set_hasSonicNozzle(bool x);
	void set_SafetyOn(bool x);
	void set_VolumeRef(float x);
	void set_VolumeP6(float x);

	void set_InjectionAttemptNumber(int x);
	void set_InjectionMargin(float x);
	void set_InjectionMultiplier(float x);

	void set_TimeBetweenMeasurement(int x);
	void set_TimeBetweenRecording(int x);
	void set_TimeBetweenAutomation(int x);
	void set_TimeWaitValves(int x);
	void set_TimeWaitValvesShort(int x);
	void set_TimeWaitPump(int x);
	void set_TimeVacuumEmergency(int x);
	void set_TimeVacuumBottle(int x);
	void set_TimeVacuumEndDefault(int x);

	void set_PressurePumpVacuum(float x);
	void set_PressureLimitVacuum(float x);

	// Functions to easily create instruments or readers
	void AddInstrument(Instrument i, unsigned int position = 0);
	void AddReader(Reader r, unsigned int position = 0);
	void AddController(Controller r, unsigned int position = 0);
};