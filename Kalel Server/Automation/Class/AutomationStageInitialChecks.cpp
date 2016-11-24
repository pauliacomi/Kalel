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
		if (VerificationSecurity()) {
			experimentLocalData.verificationStep = STEP_VERIFICATIONS_VALVES;
		}
		break;

	case STEP_VERIFICATIONS_VALVES:
		if (VerificationValves()){
			experimentLocalData.verificationStep = STEP_VERIFICATIONS_PRESSURE;
		}
		break;

	case STEP_VERIFICATIONS_PRESSURE:
		if (VerificationResidualPressure()){
			experimentLocalData.verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
		}
		break;

	case STEP_VERIFICATIONS_TEMPERATURE:
		if (VerificationTemperature()) {
			experimentLocalData.verificationStep = STEP_VERIFICATIONS_COMPLETE;
		}
		break;

	case STEP_VERIFICATIONS_COMPLETE:
		if (VerificationComplete()) {
			experimentLocalData.verificationStep = STEP_VERIFICATIONS_SECURITY;
		}
		break;
	}
}


bool Automation::VerificationSecurity()
{
	if (!securityActivated)
	{
		// Ask user if they want to continue
		messageHandler.DisplayMessageBox(MESSAGE_NOSECURITY, MB_ICONWARNING | MB_OKCANCEL, true);
		::SetEvent(h_eventPause);
	}

	return true;
}


bool Automation::VerificationValves()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_START)
	{
		// Ask user to check the valves
		messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_STATE);
		messageHandler.DisplayMessageBox(MESSAGE_CHECK_VALVES_OPEN, MB_ICONQUESTION | MB_OKCANCEL, true);

		// Pause
		::SetEvent(h_eventPause);

		// Continue to next step
		experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		return false;
	}

	if (experimentLocalData.experimentStepStatus == STEP_STATUS_END)
	{
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}


bool Automation::VerificationResidualPressure()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		messageHandler.DisplayMessage(MESSAGE_CHECK_INITIAL_PRESSURE);

		if (experimentLocalData.pressureHigh < GetPressionSecuriteBassePression() && GetMesureBassePression() && GetMesureHautePression())
		{
			// Tell GUI we are opening valve 6
			messageHandler.DisplayMessage(MESSAGE_CHECK_OPENV6_POSSIB, experimentLocalData.pressureHigh);

			// Open valve 6
			ValveOpen(6);

			// Tell GUI we are waiting
			messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, TIME_WAIT_VALVES);

			// Set the time to wait
			WaitSeconds(TIME_WAIT_VALVES);
		}
		// Continue to next step
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;
	}

	if (experimentLocalData.experimentStepStatus == STEP_STATUS_INPROGRESS
		&& experimentLocalData.experimentWaiting == false)							// If waiting is done
	{
		// Open valve 5
		ValveOpen(5);

		// Tell GUI we are waiting
		messageHandler.DisplayMessage(MESSAGE_WAIT_TIME, TIME_WAIT_VALVES);

		// Set the time to wait
		WaitSeconds(TIME_WAIT_VALVES);

		// Continue to next step
		experimentLocalData.experimentStepStatus = STEP_STATUS_END;
	}

	if (experimentLocalData.experimentStepStatus == STEP_STATUS_END
		&& experimentLocalData.experimentWaiting == false)							// If waiting is done
	{
		// Check residual pressure
		if (experimentLocalData.pressureHigh >= GetPressionLimiteVide())
		{
			messageHandler.DisplayMessageBox(MESSAGE_WARNING_INITIAL_PRESSURE, MB_ICONQUESTION | MB_OKCANCEL, true, experimentLocalData.pressureHigh, GetPressionLimiteVide());
			::SetEvent(h_eventPause);
		}
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}


bool Automation::VerificationTemperature()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
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
	}

	if (experimentLocalData.experimentStepStatus == STEP_STATUS_INPROGRESS)
	{
		if (sb_userContinue)
		{
			sb_userContinue = false;
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}
		else
		{
			// Loop until the temperature is stable
			if (!(experimentLocalData.temperatureCalo < experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial) && !(experimentLocalData.temperatureCalo > experimentLocalSettings.dataGeneral.temperature_experience + security_temperature_initial) &&
				!(experimentLocalData.temperatureCage < experimentLocalSettings.dataGeneral.temperature_experience - security_temperature_initial) && !(experimentLocalData.temperatureCage > experimentLocalSettings.dataGeneral.temperature_experience + security_temperature_initial))
			{
				experimentLocalData.experimentStepStatus = STEP_STATUS_END;
			}
		}
	}

	if (experimentLocalData.experimentStepStatus == STEP_STATUS_END)
	{
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}

bool Automation::VerificationComplete()
{
	// Go to the next step
	experimentLocalData.experimentStage = STAGE_EQUILIBRATION;
	experimentLocalData.experimentStepStatus = STEP_STATUS_START;

	return true;
}