#include "StdAfx.h"
#include "Automation.h"
#include "Define_Manip_Auto_Verifications.h"

void Automation::Verifications()
{
	switch (experimentLocalData.verificationStep)
	{
	case STEP_VERIFICATIONS_SECURITY:
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_VALVES;
		VerificationSecurity();
		experimentLocalData.experimentStage = PAUSE;
		break;
	case STEP_VERIFICATIONS_VALVES:
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_PRESSURE;
		VerificationValves();
		break;
	case STEP_VERIFICATIONS_PRESSURE:
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
		VerificationResidualPressure();
		break;
	case STEP_VERIFICATIONS_TEMPERATURE:
		experimentLocalData.experimentStage = STAGE_EQUILIBRATION;
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_COMPLETE;
		VerificationTemperature();
		break;
	}

	g_flagAskShutdown = STOP;
}



int Automation::VerificationSecurity()
{
	bool secu = GetActivationSecurite();
	if (!secu)
	{
		// should implement a pause functionality to put here
		CString MessageInBox;
		MessageInBox.Format(MESSAGE_NOSECURITY);
		return AfxMessageBox(MessageInBox, MB_ICONWARNING | MB_OKCANCEL);
	}
	return 0;
}


int Automation::VerificationValves()
{
	// Normalement, toutes les vannes sont fermées...
	messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_STATE);
	messageHandler.DisplayStep(MESSAGE_CHECK_INITIAL_STATE);

	CString MessageInBox;
	MessageInBox.Format(MESSAGE_CHECK_VALVES_OPEN);
	int verif_etat_initial = AfxMessageBox(MessageInBox, MB_ICONQUESTION | MB_OKCANCEL);

	if (verif_etat_initial == IDCANCEL)
		return IDCANCEL;

	return 0;
}


int Automation::VerificationResidualPressure()
{
	// Vérification de la pression résiduelle
	messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_PRESSURE);
	messageHandler.DisplayStep(MESSAGE_CHECK_INITIAL_PRESSURE);

	// Read the pressure
	ReadHighPressure();
	//messageHandler.DisplayHighPressure();

	if (experimentLocalData.pressureHigh < GetPressionSecuriteBassePression() && GetMesureBassePression() && GetMesureHautePression())
	{
		// Tell GUI we are opening valve 6
		messageHandler.DisplayMessage(MESSAGE_CHECK_OPENV6_POSSIB, experimentLocalData.pressureHigh);

		// Open valve 6
		ValveOpen(6);

		// Tell GUI we are waiting
		messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, Intervalle1 / 1000);

		// Wait
		AttenteSecondes(Intervalle1 / 1000);
	}

	// Open valve 5
	ValveOpen(5);

	// Tell GUI we are waiting
	messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, Intervalle1 / 1000);

	// Wait
	AttenteSecondes(Intervalle1 / 1000);

	// Read the pressure
	ThreadMeasurement();
	//messageHandler.DisplayHighPressure();

	// Check residual pressure
	if (experimentLocalData.pressureHigh >= GetPressionLimiteVide())
	{
		CString MessageInBox;
		MessageInBox.Format(MESSAGE_WARNING_INITIAL_PRESSURE, experimentLocalData.pressureHigh, GetPressionLimiteVide());
		int verif_pression = AfxMessageBox(MessageInBox, MB_ICONQUESTION | MB_OKCANCEL);
		if (verif_pression == IDCANCEL)
			return IDCANCEL;
	}

	return 0;
}



int Automation::VerificationTemperature()
{
	// Vérification des températures
	messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_TEMPERATURE);
	messageHandler.DisplayStep(MESSAGE_CHECK_INITIAL_TEMPERATURE);

	experimentLocalData.experimentStage = STAGE_TEMP;

	//LectureTemperatures();
	//messageHandler.DisplayTemperatures();

	// préciser quelle température dans la boite de dialogue ???
	if ((experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - 1) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + 1) ||
		(experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - 1) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + 1))
	{
		//messageHandler.DisplayMessageBox(MESSAGE_CHECK_TEMPERATURE_DIFF, MB_ICONQUESTION | MB_YESNOCANCEL);
		
		int verif_temperatures = IDCANCEL; //= AfxMessageBox(MessageInBox, MB_ICONQUESTION | MB_YESNOCANCEL);

		// Does the user wait or cancel?
		if (verif_temperatures == IDCANCEL)		// Cancel, give up on experiment
			return IDCANCEL;
		if (verif_temperatures == IDYES)		// Yes, wait until temperature stabilizes
		{
			// Tell GUI we are waiting
			messageHandler.DisplayMessage(MESSAGE_WAIT_TEMP_EQUILIBRATION);

			// Loop until the temperature is stable
			while ((experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - 1) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + 1) ||
				(experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - 1) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + 1))
			{
				int attente_pause = 500;//ms
				int attente_temperature = 10000;//ms
				for (int i = 0; i < attente_temperature / attente_pause; i++)
				{
					TemperatureStop();
					Sleep(attente_pause);		// use functionality?
				}
				// Read the temperature
				ThreadMeasurement();
				// Display temperatures
				// messageHandler.DisplayTemperatures();
			}
		}
	}

	return 0;
}
