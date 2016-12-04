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

Security::Security(bool activated, float PressureHigh_HighRange, float PressureHigh_LowRange)
{
	securityActivated = activated;

	security_PressureHigh_HighRange = PressureHigh_HighRange;
	security_PressureHigh_LowRange = PressureHigh_LowRange;

	security_PressureHigh_flag = false;
	security_TemperatureHigh_flag = false;
	security_TemperatureLow_flag = false;
}

Security::~Security()
{
}

void Security::SecuriteHautePression()
{
	if (experimentLocalSettings->experimentType == EXPERIMENT_TYPE_MANUAL)
		SecuriteHautePressionManuelle();
	if (experimentLocalSettings->experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteHautePressionAuto();
}

void Security::SecuriteTemperatures()
{
	if (experimentLocalSettings->experimentType == EXPERIMENT_TYPE_MANUAL)
		SecuriteTemperaturesManuelle();
	if (experimentLocalSettings->experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto();
}


void Security::SecuriteHautePressionManuelle()
{
	// Check for the pressure being higher than low pressure limit
	if (experimentLocalData->pressureLow > storageVectors->machineSettings->PressionSecuriteBassePression)
	{
		// If valve 6 is open and pressure is higher than specified, close valve 6
		if (controls.valveControls->VanneEstOuvert(6))
		{
			controls.valveControlmessageHandler->DisplayMessage(MESSAGE_WARNING_PHIGH_V6, experimentLocalData->pressureHigh, storageVectors->machineSettings->PressionSecuriteBassePression);
			ValveClose(6);
		}
	}
	else
	{
		if (!controls.valveControls->VanneEstOuvert(6))
		{
			ValveOpen(6);
		}
	}

	// Check the result
	if (experimentLocalData->pressureHigh >= security_PressureHigh_HighRange)
	{
		if (!security_PressureHigh_flag) {
			// Set the flag
			security_PressureHigh_flag = true;

			// Alert user
			controls.messageHandler->DisplayMessageBox(MESSAGE_WARNING_PHIGH_BOX, MB_ICONERROR, false, security_PressureHigh_HighRange);
			controls.messageHandler->DisplayMessage(MESSAGE_WARNING_PHIGH);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_PressureHigh_flag)
		{
			controls.messageHandler->DisplayMessage(MESSAGE_WARNING_PHNORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_PressureHigh_flag = FALSE;
		}
}



void Security::SecuriteTemperaturesManuelle()
{
	// Check calorimeter temperature high
	if (experimentLocalData->temperatureCalo >= experimentLocalSettings->dataGeneral.temperature_experience + securite_temperature)
	{
		if (!security_TemperatureHigh_flag) {
			// Set the flag
			security_TemperatureHigh_flag = true;

			// Alert user
			controls.messageHandler->DisplayMessageBox(MESSAGE_WARNING_THIGH_BOX, MB_ICONERROR, false, experimentLocalSettings->dataGeneral.temperature_experience + securite_temperature);
			controls.messageHandler->DisplayMessage(MESSAGE_WARNING_CALOT_HIGH);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_TemperatureHigh_flag)
		{
			controls.messageHandler->DisplayMessage(MESSAGE_WARNING_CALOT_NORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_TemperatureHigh_flag = FALSE;
		}

	// Check calorimeter temperature low
	if (experimentLocalData->temperatureCalo <= experimentLocalSettings->dataGeneral.temperature_experience - securite_temperature)
	{
		if (!security_TemperatureLow_flag) {
			// Set the flag
			security_TemperatureLow_flag = true;

			// Alert user
			controls.messageHandler->DisplayMessageBox(MESSAGE_WARNING_TLOW_BOX, MB_ICONERROR, false, experimentLocalSettings->dataGeneral.temperature_experience - securite_temperature);
			controls.messageHandler->DisplayMessage(MESSAGE_WARNING_CALOT_LOW);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_TemperatureLow_flag)
		{
			controls.messageHandler->DisplayMessage(MESSAGE_WARNING_CALOT_NORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_TemperatureLow_flag = FALSE;
		}
}


void Security::SecuriteHautePressionAuto()
{
	if (securityActivated)
	{
		if (experimentLocalData->experimentStage != STAGE_VACUUM_SAMPLE)
		{
			// Check for the pressure being higher than low pressure limit
			if (experimentLocalData->pressureHigh > storageVectors->machineSettings->PressionSecuriteBassePression)
			{
				// If valve 6 is open and pressure is higher than specified, close valve 6
				if (controls.valveControls->VanneEstOuvert(6))
				{
					controls.messageHandler->DisplayMessage(MESSAGE_WARNING_PHIGH_V6, experimentLocalData->pressureHigh, storageVectors->machineSettings->PressionSecuriteBassePression);
					ValveClose(6);
				}
			}
			else
			{
				if (!controls.valveControls->VanneEstOuvert(6))
				{
					ValveOpen(6);
				}
			}

			// Check for the pressure being higher than high pressure limit
			if (experimentLocalData->pressureHigh >= storageVectors->machineSettings->PressionSecuriteHautePression) {
				//g_flagState = ARRET_URGENCE_HP;
			}
		}
	}
}


void Security::SecuriteTemperaturesAuto()
{
	if (securityActivated)
	{
		if (experimentLocalData->experimentStage != STAGE_VACUUM_SAMPLE)
		{
			if (experimentLocalData->temperatureCalo >= experimentLocalSettings->dataGeneral.temperature_experience + securite_temperature)
			{
				controls.messageHandler->DisplayMessage(MESSAGE_WARNING_THIGH_STOP, experimentLocalSettings->dataGeneral.temperature_experience + securite_temperature);
				//g_flagState = ARRET_URGENCE_TCH;
			}
			if (experimentLocalData->temperatureCalo <= experimentLocalSettings->dataGeneral.temperature_experience - securite_temperature)
			{
				controls.messageHandler->DisplayMessage(MESSAGE_WARNING_TLOW_STOP, experimentLocalSettings->dataGeneral.temperature_experience - securite_temperature);
				//g_flagState = ARRET_URGENCE_TCB;
			}
		}
	}
}