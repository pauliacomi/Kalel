#include "StdAfx.h"
#include "Automation.h"
#include "Define_Manip_Auto_Verifications.h"

void Automation::Verifications()
{
	switch (experimentLocalData.verificationStep)
	{
	case STEP_VERIFICATIONS_SECURITY:
		VerificationSecurity();
		break;
	case STEP_VERIFICATIONS_VALVES:
		VerificationValves();
		break;
	case STEP_VERIFICATIONS_PRESSURE:
		VerificationResidualPressure();
		break;
	case STEP_VERIFICATIONS_TEMPERATURE:
		VerificationTemperature();
		break;
	case STEP_VERIFICATIONS_COMPLETE:
		VerificationComplete();
		break;
	}
}


int Automation::VerificationSecurity()
{
	bool secu = GetActivationSecurite();
	if (!secu)
	{
		// Ask user if they want to continue
		messageHandler.DisplayMessageBox(MESSAGE_NOSECURITY, MB_ICONWARNING | MB_OKCANCEL, true);
		g_flagState = PAUSE;
	}
	experimentLocalData.verificationStep = STEP_VERIFICATIONS_VALVES;
	return 0;
}


int Automation::VerificationValves()
{
	// Ask user to check the valves
	messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_STATE);
	messageHandler.DisplayMessageBox(MESSAGE_CHECK_VALVES_OPEN, MB_ICONQUESTION | MB_OKCANCEL, true);
	g_flagState = PAUSE;

	experimentLocalData.verificationStep = STEP_VERIFICATIONS_PRESSURE;

	return 0;
}


int Automation::VerificationResidualPressure()
{
	switch (experimentLocalData.experimentSubstepStage)
	{

	case SUBSTEP_STATUS_START:
		// Vérification de la pression résiduelle
		messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_PRESSURE);

		if (experimentLocalData.pressureHigh < GetPressionSecuriteBassePression() && GetMesureBassePression() && GetMesureHautePression())
		{
			// Tell GUI we are opening valve 6
			messageHandler.DisplayMessage(MESSAGE_CHECK_OPENV6_POSSIB, experimentLocalData.pressureHigh);

			// Open valve 6
			ValveOpen(6);

			// Tell GUI we are waiting
			messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, TimeInterval);

			// Set the time to wait
			experimentLocalData.timeToEquilibrate = TimeInterval;
			experimentLocalData.experimentStage = STAGE_EQUILIBRATION;
			experimentLocalData.experimentEquilibrationStatus = STEP_STATUS_INPROGRESS;
		}
		experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS;
		break;

	case STEP_STATUS_INPROGRESS:
		// Open valve 5
		ValveOpen(5);

		// Tell GUI we are waiting
		messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, TimeInterval);
		// Set the time to wait
		experimentLocalData.timeToEquilibrate = TimeInterval;
		experimentLocalData.experimentStage = STAGE_EQUILIBRATION;
		experimentLocalData.experimentEquilibrationStatus = STEP_STATUS_INPROGRESS;

		experimentLocalData.experimentSubstepStage = STEP_STATUS_END;
		break;

	case STEP_STATUS_END:
		// Check residual pressure
		if (experimentLocalData.pressureHigh >= GetPressionLimiteVide())
		{
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_INITIAL_PRESSURE, MB_ICONQUESTION | MB_OKCANCEL, true, experimentLocalData.pressureHigh, GetPressionLimiteVide());
			g_flagState = PAUSE;
		}
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
		break;

	default:
		ASSERT(0);
		break;
	}

	return 0;
}


int Automation::VerificationTemperature()
{
	switch (experimentLocalData.experimentSubstepStage)
	{

	case SUBSTEP_STATUS_START:
		// Vérification des températures
		messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_TEMPERATURE);

		if ((experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - TEMP_SECURITY) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + TEMP_SECURITY) ||
			(experimentLocalData.temperatureCage < experimentLocalSettings.dataGeneral.temperature_experience - TEMP_SECURITY) || (experimentLocalData.temperatureCage > experimentLocalSettings.dataGeneral.temperature_experience + TEMP_SECURITY))
		{
			messageHandler.DisplayMessageBox(MESSAGE_CHECK_TEMPERATURE_DIFF, MB_ICONQUESTION | MB_YESNOCANCEL, true, experimentLocalData.temperatureCalo, experimentLocalSettings.dataGeneral.temperature_experience - TEMP_SECURITY);
			g_flagState = PAUSE;
			experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS;
		}
		else
		{
			experimentLocalData.experimentSubstepStage = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_INPROGRESS:
		// Tell GUI we are waiting
		messageHandler.DisplayMessage(MESSAGE_WAIT_TEMP_EQUILIBRATION);

		// Loop until the temperature is stable
		if ((experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - TEMP_SECURITY) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + TEMP_SECURITY) ||
			(experimentLocalData.temperatureCage < experimentLocalSettings.dataGeneral.temperature_experience - TEMP_SECURITY) || (experimentLocalData.temperatureCage > experimentLocalSettings.dataGeneral.temperature_experience + TEMP_SECURITY))
		{
			g_flagState = PAUSE;
		}
		else
		{
			experimentLocalData.experimentSubstepStage = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_COMPLETE;
		break;

	default:
		ASSERT(0);
		break;
	}

	return 0;
}

int Automation::VerificationComplete()
{
	// Start the global experiment timer
	timerExperiment.TopChrono();

	// Go to the next step
	experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataDivers.temps_ligne_base;		// Set the time to wait
	experimentLocalData.experimentSubstepStage = STEP_STATUS_START;
	experimentLocalData.experimentStage = STAGE_ADSORPTION;

	return 0;
}