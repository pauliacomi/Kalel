// This function contains checks for high pressure and other security related checks
#include "Security.h"

// Resources
#include "../../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineStages.h"					// All stage, experiment type definitions are here
#include "../../../Kalel Shared/Resources/DefineAutomationSettings.h"		// All settings for automation are stored here


// Synchronization classes
#include "../../../Kalel Shared/log.h"										// Logging
#include "../../../Kalel Shared/soundHelpers.h"								// Sound handling for beeps

// Measurement and manipulation classes
#include "../../Backend/Wrapper Classes/ValveController.h"					// Controlling valves
#include "../../../Kalel Shared/Com Classes/ExperimentData.h"


Security::Security(bool activated, ValveController & valveControl)
	: securityActivated{ activated }
	, valveController{ valveControl }
{
}

Security::~Security()
{
}

void Security::SecurityOverPressure(int experimentType, double maxPlow, double maxPhigh, const ExperimentData &expData)
{
	if (experimentType == EXPERIMENT_TYPE_MANUAL)
		SecurityHighPressureManual(maxPlow,  maxPhigh, expData);
	if (experimentType == EXPERIMENT_TYPE_AUTO)
		SecurityHighPressureAuto(maxPlow, maxPhigh, expData);
}

void Security::SecurityTemperatures(int experimentType, double maxPlow, double maxPhigh, const ExperimentData &expData)
{
	if (experimentType == EXPERIMENT_TYPE_MANUAL)
		SecurityTemperaturesManual(maxPlow, maxPhigh, expData);
	if (experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto(maxPlow, maxPhigh, expData);
}


void Security::SecurityHighPressureManual(double maxPlow, double maxPhigh, const ExperimentData &expData)
{
	// Check for the pressure being higher than low pressure limit
	if (expData.pressureLow > maxPlow)
	{
		// If valve 6 is open and pressure is higher than specified, close valve 6
		if (valveController.ValveIsOpen(6))
		{
			LOG(logINFO) << MESSAGE_WARNING_PHIGH_V6 << expData.pressureHigh << maxPlow;
			valveController.ValveClose(6, false);
			LOG(logINFO) << MESSAGE_VALVE_CLOSED << 6;
		}
	}
	else
	{
		if (!valveController.ValveIsOpen(6))
		{
			valveController.ValveOpen(6, false);
			LOG(logINFO) << MESSAGE_VALVE_OPENED << 6;
		}
	}

	// Check the result
	if (expData.pressureHigh >= maxPhigh)
	{
		if (!security_PressureHigh_flag) {
			// Set the flag
			security_PressureHigh_flag = true;

			// Alert user
			LOG(logWARNING) << MESSAGE_WARNING_PHIGH << maxPhigh;

			// Play a sound
			soundh::beep::error();
		}
	}
	else
		if (security_PressureHigh_flag)
		{
			LOG(logINFO) << MESSAGE_WARNING_PHNORMAL;
			soundh::beep::allgood();
			security_PressureHigh_flag = FALSE;
		}
}


void Security::SecurityHighPressureAuto(double maxPlow, double maxPhigh, const ExperimentData &expData)
{
	if (securityActivated)
	{
		if (expData.experimentStage != STAGE_VACUUM_SAMPLE)
		{
			// Check for the pressure being higher than low pressure limit
			if (expData.pressureHigh > maxPlow)
			{
				// If valve 6 is open and pressure is higher than specified, close valve 6
				if (valveController.ValveIsOpen(6))
				{
					LOG(logINFO) << MESSAGE_WARNING_PHIGH_V6 << expData.pressureHigh << maxPlow;
					valveController.ValveClose(6, false);
					LOG(logINFO) << MESSAGE_VALVE_CLOSED << 6;
				}
			}
			else
			{
				if (!valveController.ValveIsOpen(6))
				{
					valveController.ValveOpen(6, false);
					LOG(logINFO) << MESSAGE_VALVE_OPENED << 6;
				}
			}

			// Check for the pressure being higher than high pressure limit
			if (expData.pressureHigh >= maxPhigh) {
				//g_flagState = ARRET_URGENCE_HP;
			}
		}
	}
}


void Security::SecurityTemperaturesManual(double maximumT, double minimumT, const ExperimentData &expData)
{
	// Check calorimeter temperature high
	if (expData.temperatureCalo >= maximumT)
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
			security_TemperatureHigh_flag = FALSE;
		}

	// Check calorimeter temperature low
	if (expData.temperatureCalo <= minimumT)
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




void Security::SecuriteTemperaturesAuto(double maximumT, double minimumT, const ExperimentData &expData)
{
	if (securityActivated)
	{
		if (expData.experimentStage != STAGE_VACUUM_SAMPLE)
		{
			if (expData.temperatureCalo >= maximumT)
			{
				LOG(logINFO) << MESSAGE_WARNING_THIGH_STOP << maximumT;
				//g_flagState = ARRET_URGENCE_TCH;
			}
			if (expData.temperatureCalo <= minimumT)
			{
				LOG(logINFO) << MESSAGE_WARNING_TLOW_STOP << minimumT;
				//g_flagState = ARRET_URGENCE_TCB;
			}
		}
	}
}