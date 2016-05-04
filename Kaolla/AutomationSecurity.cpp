// This function contains checks for high pressure and other security related checks

#include "StdAfx.h"
#include "Automation.h"
#include "Define_Manip_Securite.h"

void Automation::InitialisationSecurityManual()
{
	security_PressureHigh = false;
	security_TemperatureHigh = false;
	security_TemperatureLow = false;
}

void Automation::SecuriteHautePression()
{
	if (experimentLocalData.experimentType == EXPERIMENT_TYPE_MANUAL)
		SecuriteHautePressionManuelle();
	if (experimentLocalData.experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteHautePressionAuto();
}

void Automation::SecuriteTemperatures()
{
	if (experimentLocalData.experimentType == EXPERIMENT_TYPE_MANUAL)
		SecuriteTemperaturesManuelle();
	if (experimentLocalData.experimentTime == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto();
}


void Automation::SecuriteHautePressionManuelle()
{
	// Check the result
	if (experimentLocalData.pressureHigh >= GetPressionSecuriteHautePression())
	{
		if (!security_PressureHigh) {
			// Set the flag
			security_PressureHigh = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_PHIGH_BOX, MB_ICONERROR, GetPressionSecuriteHautePression());
			messageHandler.DisplayMessage(MESSAGE_WARNING_PHIGH);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_PressureHigh)
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_PHNORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_PressureHigh = FALSE;
		}
}



void Automation::SecuriteTemperaturesManuelle()
{
	// Check calorimeter temperature high
	if (experimentLocalData.temperatureCalo >= experimentLocalData.dataGeneral.temperature_experience + securite_temperature)
	{
		if (!security_TemperatureHigh) {
			// Set the flag
			security_TemperatureHigh = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_THIGH_BOX, MB_ICONERROR, GetPressionSecuriteHautePression());
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_HIGH);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_TemperatureHigh)
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_NORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_TemperatureHigh = FALSE;
		}

	// Check calorimeter temperature low
	if (experimentLocalData.temperatureCalo <= experimentLocalData.dataGeneral.temperature_experience - securite_temperature)
	{
		if (!security_TemperatureLow) {
			// Set the flag
			security_TemperatureLow = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_TLOW_BOX, MB_ICONERROR, GetPressionSecuriteHautePression());
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_LOW);

			// Play a sound
			MessageBeep(MB_ICONERROR);
		}
	}
	else
		if (security_TemperatureLow)
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_CALOT_NORMAL);
			MessageBeep(MB_ICONINFORMATION);
			security_TemperatureLow = FALSE;
		}
}


void Automation::SecuriteHautePressionAuto()
{
	// Check if the security is acivated from the config files
	bool mesure_de_securite = GetActivationSecurite();

	if (mesure_de_securite)
	{
		if (experimentLocalData.experimentStage != STAGE_UNDER_VACUUM)
		{
			// Check for the pressure being higher than low pressure limit
			if (experimentLocalData.pressureHigh > GetPressionSecuriteBassePression())
			{
				// If valve 6 is open and pressure is higher than specified, close valve 6
				if (EstOuvert_Vanne(6))
				{
					messageHandler.DisplayMessage(MESSAGE_WARNING_PHIGH_V6, experimentLocalData.pressureHigh, GetPressionSecuriteBassePression());
					ValveClose(6);
				}
			}

			// Check for the pressure being higher than high pressure limit
			if (experimentLocalData.pressureHigh >= GetPressionSecuriteHautePression())
				g_flagAskShutdown = ARRET_URGENCE_HP;
		}
	}
}


void Automation::SecuriteTemperaturesAuto()
{
	// Check if the security is acivated from the config files
	bool mesure_de_securite = GetActivationSecurite();
	
	if (mesure_de_securite)
	{
		if (experimentLocalData.experimentStage != STAGE_UNDER_VACUUM)
		{
			if (experimentLocalData.temperatureCalo >= experimentLocalData.dataGeneral.temperature_experience + securite_temperature)
			{
				messageHandler.DisplayMessage(MESSAGE_WARNING_THIGH_STOP, experimentLocalData.dataGeneral.temperature_experience + securite_temperature);
				g_flagAskShutdown = ARRET_URGENCE_TCH;
			}
			if (experimentLocalData.temperatureCalo <= experimentLocalData.dataGeneral.temperature_experience - securite_temperature)
			{
				messageHandler.DisplayMessage(MESSAGE_WARNING_TLOW_STOP, experimentLocalData.dataGeneral.temperature_experience - securite_temperature);
				g_flagAskShutdown = ARRET_URGENCE_TCB;
			}
		}
	}
}