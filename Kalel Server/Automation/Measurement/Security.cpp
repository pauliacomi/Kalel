// This function contains checks for high pressure and other security related checks
#include "Security.h"

// Resources
#include "../../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineStages.h"					// All stage, experiment type definitions are here
#include "../../../Kalel Shared/Resources/DefineInstruments.h"				// All Instrument definitions


// Synchronization classes
#include "../../../Kalel Shared/log.h"										// Logging
#include "../../../Kalel Shared/soundHelpers.h"								// Sound handling for beeps

// Measurement and manipulation classes
#include "../../Backend/Wrapper Classes/ValveController.h"					// Controlling valves
#include "../../Automation/CommonStorage.h"


Security::Security(ValveController & valveControl)
	: valveController{ valveControl }
{
}

Security::~Security()
{
}

void Security::SecurityOverPressure(const Storage & storage)
{
	if (storage.experimentSettings.experimentType == EXPERIMENT_TYPE_MANUAL)
		SecurityHighPressureManual(storage);
	if (storage.experimentSettings.experimentType == EXPERIMENT_TYPE_AUTO)
		SecurityHighPressureAuto(storage);
}

void Security::SecurityTemperatures(const Storage & storage)
{
	if (storage.experimentSettings.experimentType == EXPERIMENT_TYPE_MANUAL)
		SecurityTemperaturesManual(storage);
	if (storage.experimentSettings.experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto(storage);
}


void Security::SecurityHighPressureManual(const Storage & storage)
{
	auto maxPlow = storage.machineSettings.readers.find(PRESSURE_LP)->second.safe_max;
	auto maxPhigh = storage.machineSettings.readers.find(PRESSURE_HP)->second.safe_max;

	// Check for the pressure being higher than low pressure limit
	if (storage.currentData.pressureLow > maxPlow)
	{
		// If valve 6 is open and pressure is higher than specified, close valve 6
		if (valveController.ValveIsOpen(VALVE_6))
		{
			// Set the flag
			security_PressureHigh_flag = true;

			LOG(logINFO) << MESSAGE_WARNING_PHIGH_V6 << storage.currentData.pressureLow << maxPlow;
			valveController.ValveClose(VALVE_6, true);

			// Play a sound
			soundh::beep::error();
		}
	}
	else
	{
		if (security_PressureHigh_flag)
		{
			if (!valveController.ValveIsOpen(VALVE_6))
			{
				valveController.ValveOpen(VALVE_6, true);
				LOG(logINFO) << MESSAGE_WARNING_PHNORMAL_V6;
				soundh::beep::allgood();
				security_PressureHigh_flag = false;
			}
		}
	}

	// Check the result
	if (storage.currentData.pressureHigh > maxPhigh)
	{
		if (!security_PressureHigh_flag) {

			// Set the flag
			security_PressureHigh_flag = true;

			// Alert user
			LOG(logWARNING) << MESSAGE_WARNING_PHIGH << storage.currentData.pressureHigh << maxPhigh;

			// Play a sound
			soundh::beep::error();
		}
	}
	else
		if (security_PressureHigh_flag)
		{
			LOG(logINFO) << MESSAGE_WARNING_PHNORMAL;
			soundh::beep::allgood();
			security_PressureHigh_flag = false;
		}
}


void Security::SecurityHighPressureAuto(const Storage & storage)
{
	if (storage.machineSettings.SafetyOn)
	{
		auto maxPlow = storage.machineSettings.readers.find(PRESSURE_LP)->second.safe_max;
		auto maxPhigh = storage.machineSettings.readers.find(PRESSURE_HP)->second.safe_max;

		// Check for the pressure being higher than low pressure limit
		if (storage.currentData.pressureLow > maxPlow)
		{
			// If valve 6 is open and pressure is higher than specified, close valve 6
			if (valveController.ValveIsOpen(VALVE_6))
			{
				// Set the flag
				security_PressureHigh_flag = true;

				LOG(logINFO) << MESSAGE_WARNING_PHIGH_V6 << storage.currentData.pressureLow << maxPlow;
				valveController.ValveClose(VALVE_6, true);
			}
		}
		else
		{
			if (security_PressureHigh_flag)
			{
				if (!valveController.ValveIsOpen(VALVE_6))
				{
					valveController.ValveOpen(VALVE_6, true);
				}
			}
		}

		// Check for the pressure being higher than high pressure limit
		if (storage.currentData.pressureHigh >= maxPhigh) {
			//g_flagState = ARRET_URGENCE_HP;
		}
	}
}


void Security::SecurityTemperaturesManual(const Storage & storage)
{
	auto minimumT = storage.machineSettings.readers.find(TEMPERATURE_CALO)->second.safe_min;
	auto maximumT = storage.machineSettings.readers.find(TEMPERATURE_CALO)->second.safe_max;

	// Check calorimeter temperature high
	if (storage.currentData.temperatureCalo >= maximumT)
	{
		if (!security_TemperatureHigh_flag) {
			// Set the flag
			security_TemperatureHigh_flag = true;

			// Alert user
			LOG(logWARNING) << MESSAGE_WARNING_CALOT_HIGH << maximumT;

			// Play a sound
			soundh::beep::error();
		}
	}
	else
		if (security_TemperatureHigh_flag)
		{
			LOG(logINFO) << MESSAGE_WARNING_CALOT_NORMAL;
			soundh::beep::allgood();
			security_TemperatureHigh_flag = false;
		}

	// Check calorimeter temperature low
	if (storage.currentData.temperatureCalo <= minimumT)
	{
		if (!security_TemperatureLow_flag) {
			// Set the flag
			security_TemperatureLow_flag = true;

			// Alert user
			LOG(logWARNING) << MESSAGE_WARNING_CALOT_LOW << minimumT;

			// Play a sound
			soundh::beep::error();
		}
	}
	else
		if (security_TemperatureLow_flag)
		{
			LOG(logINFO) << MESSAGE_WARNING_CALOT_NORMAL;
			soundh::beep::allgood();
			security_TemperatureLow_flag = FALSE;
		}
}




void Security::SecuriteTemperaturesAuto(const Storage & storage)
{
	if (storage.machineSettings.SafetyOn)
	{
		auto minimumT = storage.machineSettings.readers.find(TEMPERATURE_CALO)->second.safe_min;
		auto maximumT = storage.machineSettings.readers.find(TEMPERATURE_CALO)->second.safe_max;

		if (storage.currentData.temperatureCalo >= maximumT)
		{
			LOG(logINFO) << MESSAGE_WARNING_THIGH_STOP << maximumT;
			//g_flagState = ARRET_URGENCE_TCH;
		}
		if (storage.currentData.temperatureCalo <= minimumT)
		{
			LOG(logINFO) << MESSAGE_WARNING_TLOW_STOP << minimumT;
			//g_flagState = ARRET_URGENCE_TCB;
		}
	}
}