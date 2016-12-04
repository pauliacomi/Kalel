// This function contains checks for high pressure and other security related checks
#include "../../stdafx.h"
#include "Security.h"

// Resources
#include "../../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineStages.h"					// All stage, experiment type definitions are here
#include "../../../Kalel Shared/Resources/DefineAutomationSettings.h"		// All settings for automation are stored here


// Synchronization classes
#include "../../MessageHandler.h"											// Handles all the messages from this class to the client

// Measurement and manipulation classes
#include "../../Backend/Wrapper Classes/Vannes.h"							// Controlling valves
#include "../../../Kalel Shared/Com Classes/ExperimentData.h"


Security::Security(bool activated, float PressureHigh_HighRange, float PressureHigh_LowRange, ValveController & valveControl, MessageHandler & messageHandler)
	: g_pVanne{ valveControl }
	, messageHandler{ messageHandler }
{
	securityActivated = activated;

	security_PressureHigh_flag = false;
	security_TemperatureHigh_flag = false;
	security_TemperatureLow_flag = false;
}

Security::~Security()
{
}

void Security::SecurityHighPressure(int experimentType, float maxPlow, float maxPhigh, const ExperimentData &expData)
{
	if (experimentType == EXPERIMENT_TYPE_MANUAL)
		SecurityHighPressureManual(maxPlow,  maxPhigh, expData);
	if (experimentType == EXPERIMENT_TYPE_AUTO)
		SecurityHighPressureAuto(maxPlow, maxPhigh, expData);
}

void Security::SecurityTemperatures(int experimentType, float maxPlow, float maxPhigh, const ExperimentData &expData)
{
	if (experimentType == EXPERIMENT_TYPE_MANUAL)
		SecurityTemperaturesManual(maxPlow, maxPhigh, expData);
	if (experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto(maxPlow, maxPhigh, expData);
}


void Security::SecurityHighPressureManual(float maxPlow, float maxPhigh, const ExperimentData &expData)
{
	// Check for the pressure being higher than low pressure limit
	if (expData.pressureLow > maxPlow)
	{
		// If valve 6 is open and pressure is higher than specified, close valve 6
		if (g_pVanne.VanneEstOuvert(6))
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_PHIGH_V6, expData.pressureHigh, maxPlow);
			g_pVanne.Fermer(6);
			messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSED, 6);
		}
	}
	else
	{
		if (!g_pVanne.VanneEstOuvert(6))
		{
			g_pVanne.Ouvrir(6);
			messageHandler.DisplayMessage(MESSAGE_VALVE_OPENED, 6);
		}
	}

	// Check the result
	if (expData.pressureHigh >= maxPhigh)
	{
		if (!security_PressureHigh_flag) {
			// Set the flag
			security_PressureHigh_flag = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_PHIGH_BOX, MB_ICONERROR, false, maxPhigh);
			messageHandler.DisplayMessage(MESSAGE_WARNING_PHIGH);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_PressureHigh_flag)
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_PHNORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_PressureHigh_flag = FALSE;
		}
}


void Security::SecurityHighPressureAuto(float maxPlow, float maxPhigh, const ExperimentData &expData)
{
	if (securityActivated)
	{
		if (expData.experimentStage != STAGE_VACUUM_SAMPLE)
		{
			// Check for the pressure being higher than low pressure limit
			if (expData.pressureHigh > maxPlow)
			{
				// If valve 6 is open and pressure is higher than specified, close valve 6
				if (g_pVanne.VanneEstOuvert(6))
				{
					messageHandler.DisplayMessage(MESSAGE_WARNING_PHIGH_V6, expData.pressureHigh, maxPlow);
					g_pVanne.Fermer(6);
					messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSED, 6);
				}
			}
			else
			{
				if (!g_pVanne.VanneEstOuvert(6))
				{
					g_pVanne.Ouvrir(6);
					messageHandler.DisplayMessage(MESSAGE_VALVE_OPENED, 6);
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
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_THIGH_BOX, MB_ICONERROR, false, maximumT);
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_HIGH);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_TemperatureHigh_flag)
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_NORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_TemperatureHigh_flag = FALSE;
		}

	// Check calorimeter temperature low
	if (expData.temperatureCalo <= minimumT)
	{
		if (!security_TemperatureLow_flag) {
			// Set the flag
			security_TemperatureLow_flag = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_TLOW_BOX, MB_ICONERROR, false, minimumT);
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_LOW);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_TemperatureLow_flag)
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_NORMAL);
			MessageBeep(MB_ICONINFORMATION);
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
				messageHandler.DisplayMessage(MESSAGE_WARNING_THIGH_STOP, maximumT);
				//g_flagState = ARRET_URGENCE_TCH;
			}
			if (expData.temperatureCalo <= minimumT)
			{
				messageHandler.DisplayMessage(MESSAGE_WARNING_TLOW_STOP, minimumT);
				//g_flagState = ARRET_URGENCE_TCB;
			}
		}
	}
}