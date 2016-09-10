// This function contains checks for high pressure and other security related checks

#include "Automation.h"


void Automation::InitialisationSecurity()
{
	securityActivated = GetActivationSecurite();

	security_PressureHigh_HighRange = GetPressionSecuriteHautePression();
	security_PressureHigh_LowRange = GetPressionSecuriteBassePression();

	security_PressureHigh_flag = false;
	security_TemperatureHigh_flag = false;
	security_TemperatureLow_flag = false;
}

void Automation::SecuriteHautePression()
{
	if (experimentLocalSettings.experimentType == EXPERIMENT_TYPE_MANUAL)
		SecuriteHautePressionManuelle();
	if (experimentLocalSettings.experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteHautePressionAuto();
}

void Automation::SecuriteTemperatures()
{
	if (experimentLocalSettings.experimentType == EXPERIMENT_TYPE_MANUAL)
		SecuriteTemperaturesManuelle();
	if (experimentLocalSettings.experimentType == EXPERIMENT_TYPE_AUTO)
		SecuriteTemperaturesAuto();
}


void Automation::SecuriteHautePressionManuelle()
{
	// Check for the pressure being higher than low pressure limit
	if (experimentLocalData.pressureLow > GetPressionSecuriteBassePression())
	{
		// If valve 6 is open and pressure is higher than specified, close valve 6
		if (EstOuvert_Vanne(6))
		{
			messageHandler.DisplayMessage(MESSAGE_WARNING_PHIGH_V6, experimentLocalData.pressureHigh, GetPressionSecuriteBassePression());
			ValveClose(6);
		}
	}
	else
	{
		if (!EstOuvert_Vanne(6))
		{
			ValveOpen(6);
		}
	}

	// Check the result
	if (experimentLocalData.pressureHigh >= security_PressureHigh_HighRange)
	{
		if (!security_PressureHigh_flag) {
			// Set the flag
			security_PressureHigh_flag = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_PHIGH_BOX, MB_ICONERROR, false, security_PressureHigh_HighRange);
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



void Automation::SecuriteTemperaturesManuelle()
{
	// Check calorimeter temperature high
	if (experimentLocalData.temperatureCalo >= experimentLocalSettings.dataGeneral.temperature_experience + securite_temperature)
	{
		if (!security_TemperatureHigh_flag) {
			// Set the flag
			security_TemperatureHigh_flag = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_THIGH_BOX, MB_ICONERROR, false, experimentLocalSettings.dataGeneral.temperature_experience + securite_temperature);
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
	if (experimentLocalData.temperatureCalo <= experimentLocalSettings.dataGeneral.temperature_experience - securite_temperature)
	{
		if (!security_TemperatureLow_flag) {
			// Set the flag
			security_TemperatureLow_flag = true;

			// Alert user
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_TLOW_BOX, MB_ICONERROR, false, experimentLocalSettings.dataGeneral.temperature_experience - securite_temperature);
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


void Automation::SecuriteHautePressionAuto()
{
	if (securityActivated)
	{
		if (experimentLocalData.experimentStage != STAGE_VACUUM_SAMPLE)
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
			else
			{
				if (!EstOuvert_Vanne(6))
				{
					ValveOpen(6);
				}
			}

			// Check for the pressure being higher than high pressure limit
			if (experimentLocalData.pressureHigh >= GetPressionSecuriteHautePression()) {
				//g_flagState = ARRET_URGENCE_HP;
			}
		}
	}
}


void Automation::SecuriteTemperaturesAuto()
{
	if (securityActivated)
	{
		if (experimentLocalData.experimentStage != STAGE_VACUUM_SAMPLE)
		{
			if (experimentLocalData.temperatureCalo >= experimentLocalSettings.dataGeneral.temperature_experience + securite_temperature)
			{
				messageHandler.DisplayMessage(MESSAGE_WARNING_THIGH_STOP, experimentLocalSettings.dataGeneral.temperature_experience + securite_temperature);
				//g_flagState = ARRET_URGENCE_TCH;
			}
			if (experimentLocalData.temperatureCalo <= experimentLocalSettings.dataGeneral.temperature_experience - securite_temperature)
			{
				messageHandler.DisplayMessage(MESSAGE_WARNING_TLOW_STOP, experimentLocalSettings.dataGeneral.temperature_experience - securite_temperature);
				//g_flagState = ARRET_URGENCE_TCB;
			}
		}
	}
}