#include "StdAfx.h"
#include "Automation.h"

/*
*
*
*			INITIAL CHECKS
*
*
*/


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
	if (!securityActivated)
	{
		// Ask user if they want to continue
		messageHandler.DisplayMessageBox(MESSAGE_NOSECURITY, MB_ICONWARNING | MB_OKCANCEL, true);
		::SetEvent(h_eventPause);
	}
	experimentLocalData.verificationStep = STEP_VERIFICATIONS_VALVES;
	return 0;
}


int Automation::VerificationValves()
{
	// Ask user to check the valves
	messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_STATE);
	messageHandler.DisplayMessageBox(MESSAGE_CHECK_VALVES_OPEN, MB_ICONQUESTION | MB_OKCANCEL, true);
	::SetEvent(h_eventPause);

	experimentLocalData.verificationStep = STEP_VERIFICATIONS_PRESSURE;

	return 0;
}


int Automation::VerificationResidualPressure()
{
	switch (experimentLocalData.experimentStepStatus)
	{

	case STEP_STATUS_START:
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
			WaitSeconds(TimeInterval);
		}
		// Continue to next step
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;
		break;

	case STEP_STATUS_INPROGRESS:
		if (experimentLocalData.experimentWaiting == false) {				// If waiting is done
			// Open valve 5
			ValveOpen(5);

			// Tell GUI we are waiting
			messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, TimeInterval);

			// Set the time to wait
			WaitSeconds(TimeInterval);

			// Continue to next step
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}		
		break;

	case STEP_STATUS_END:
		if (experimentLocalData.experimentWaiting == false) {				// If waiting is done
			// Check residual pressure
			if (experimentLocalData.pressureHigh >= GetPressionLimiteVide())
			{
				messageHandler.DisplayMessageBox(MESSAGE_WARNING_INITIAL_PRESSURE, MB_ICONQUESTION | MB_OKCANCEL, true, experimentLocalData.pressureHigh, GetPressionLimiteVide());
				::SetEvent(h_eventPause);
			}
			experimentLocalData.experimentStepStatus = STEP_STATUS_START;
			experimentLocalData.verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
		}
		break;

	default:
		ASSERT(0);
		break;
	}

	return 0;
}


int Automation::VerificationTemperature()
{
	switch (experimentLocalData.experimentStepStatus)
	{

	case STEP_STATUS_START:
		// Vérification des températures
		messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_TEMPERATURE);

		if ((experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial) || (experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + security_temperature_initial) ||
			(experimentLocalData.temperatureCage < experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial) || (experimentLocalData.temperatureCage > experimentLocalSettings.dataGeneral.temperature_experience + security_temperature_initial))
		{
			// Tell GUI we are waiting
			messageHandler.DisplayMessage(MESSAGE_WAIT_TEMP_EQUILIBRATION);
			messageHandler.DisplayMessageBox(MESSAGE_CHECK_TEMPERATURE_DIFF, MB_ICONQUESTION | MB_YESNOCANCEL, true, experimentLocalData.temperatureCalo, experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial);
			
			::SetEvent(h_eventPause);
			experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;
		}
		else
		{
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_INPROGRESS:
		if (experimentLocalSettings.continueAnyway)
		{
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}
		// Loop until the temperature is stable
		if ((experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial) || (experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience + security_temperature_initial) ||
			(experimentLocalData.temperatureCage > experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial) || (experimentLocalData.temperatureCage < experimentLocalSettings.dataGeneral.temperature_experience + security_temperature_initial))
		{
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
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
	// Go to the next step
	experimentLocalData.experimentStage = STAGE_EQUILIBRATION;
	experimentLocalData.experimentStepStatus = STEP_STATUS_START;

	return 0;
}