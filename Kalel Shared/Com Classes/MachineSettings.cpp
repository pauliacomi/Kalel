#include "../Forcelib.h"

#include "MachineSettings.h"

#include "../timeHelpers.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	ResetData();							// Do initialisation of all variables
}

MachineSettings::MachineSettings(const MachineSettings & rhs)
{
	Replace(rhs);							// Copy values from reference
}

MachineSettings & MachineSettings::operator=(const MachineSettings & p)
{
	if (this != &p) {  // make sure not same object

		Replace(p);

		tp = timeh::NowTime();
	}
	return *this;    // Return ref for multiple assignment
}

void MachineSettings::ResetData()
{
	tp = timeh::NowTime();								// Time

	CaloName = L"UnnamedCalo";
	CaloPrefix = L"Prefix";
	DefaultPath = L"C:";

	hasSonicNozzle = false;
	SafetyOn = true;									// Used for activating the automatic security
	VolumeRef = 1;										// Used for user information (no calculations)
	VolumeP6 = 1;										// Used for user information (no calculations)

	InjectionAttemptNumber = 5;
	InjectionMargin = 0.01f;
	InjectionMultiplier = 1.2f;

	TimeBetweenMeasurement = 1000;						// milliseconds
	TimeBetweenRecording = 2000;						// milliseconds
	TimeBetweenAutomation = 10;							// milliseconds
	TimeWaitValves = 5;									// seconds
	TimeWaitValvesShort = 2;							// seconds
	TimeWaitPump = 10;									// seconds
	TimeVacuumEmergency = 6000;							// seconds
	TimeVacuumBottle = 300;								// seconds
	TimeVacuumEndDefault = 12000;						// seconds

	PressurePumpVacuum = 0.5f;
	PressureLimitVacuum = 0.01f;						// Used for determining the pressure considered "good vacuum"
}

void MachineSettings::Replace(const MachineSettings & rhs) {

	tp = rhs.tp;

	CaloName = rhs.CaloName;
	CaloPrefix = rhs.CaloPrefix;
	DefaultPath = rhs.DefaultPath;

	hasSonicNozzle = rhs.hasSonicNozzle;
	SafetyOn = rhs.SafetyOn;
	VolumeRef = rhs.VolumeRef;
	VolumeP6 = rhs.VolumeP6;

	InjectionAttemptNumber = rhs.InjectionAttemptNumber;
	InjectionMargin = rhs.InjectionMargin;
	InjectionMultiplier = rhs.InjectionMultiplier;

	TimeBetweenMeasurement = rhs.TimeBetweenMeasurement;
	TimeBetweenRecording = rhs.TimeBetweenRecording;
	TimeBetweenAutomation = rhs.TimeBetweenAutomation;
	TimeWaitValves = rhs.TimeWaitValves;
	TimeWaitValvesShort = rhs.TimeWaitValvesShort;
	TimeWaitPump = rhs.TimeWaitPump;
	TimeVacuumEmergency = rhs.TimeVacuumEmergency;
	TimeVacuumBottle = rhs.TimeVacuumBottle;
	TimeVacuumEndDefault = rhs.TimeVacuumEndDefault;

	PressurePumpVacuum = rhs.PressurePumpVacuum;
	PressureLimitVacuum = rhs.PressureLimitVacuum;

	instruments = rhs.instruments;
	readers = rhs.readers;
	controllers = rhs.controllers;
}
