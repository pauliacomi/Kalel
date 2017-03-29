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
	switch (storage.currentData->verificationStep)
	{
	case STEP_VERIFICATIONS_SECURITY:
		if (VerificationSecurity()) {
			storage.currentData->verificationStep = STEP_VERIFICATIONS_VALVES;
		}
		break;

	case STEP_VERIFICATIONS_VALVES:
		if (VerificationValves()){
			storage.currentData->verificationStep = STEP_VERIFICATIONS_PRESSURE;
		}
		break;

	case STEP_VERIFICATIONS_PRESSURE:
		if (VerificationResidualPressure()){
			storage.currentData->verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
		}
		break;

	case STEP_VERIFICATIONS_TEMPERATURE:
		if (VerificationTemperature()) {
			storage.currentData->verificationStep = STEP_VERIFICATIONS_COMPLETE;
		}
		break;

	case STEP_VERIFICATIONS_COMPLETE:
		if (VerificationComplete()) {
			storage.currentData->verificationStep = STEP_VERIFICATIONS_SECURITY;
		}
		break;
	}
}


bool Automation::VerificationSecurity()
{
	if (!storage.machineSettings->ActivationSecurite)
	{
		// Ask user if they want to continue
		controls.messageHandler->DisplayMessageBox(MESSAGE_NOSECURITY, MB_ICONWARNING | MB_OKCANCEL, true);
		eventPause = true;
		storage.automationControl.notify_all();
	}

	return true;
}


bool Automation::VerificationValves()
{
	if (storage.currentData->experimentStepStatus == STEP_STATUS_START)
	{
		// Ask user to check the valves
		controls.messageHandler->DisplayMessage(MESSAGE_CHECK_INITIAL_STATE);
		controls.messageHandler->DisplayMessageBox(MESSAGE_CHECK_VALVES_OPEN, MB_ICONQUESTION | MB_OKCANCEL, true);

		// Pause
		eventPause = true;
		storage.automationControl.notify_all();

		// Continue to next step
		storage.currentData->experimentStepStatus = STEP_STATUS_END;
		return false;
	}

	if (storage.currentData->experimentStepStatus == STEP_STATUS_END)
	{
		storage.currentData->experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}


bool Automation::VerificationResidualPressure()
{
	if (storage.currentData->experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		controls.messageHandler->DisplayMessage(MESSAGE_CHECK_INITIAL_PRESSURE);

		if (storage.currentData->pressureHigh < storage.machineSettings->PressionSecuriteBassePression 
			&& storage.machineSettings->HighPressureToMeasure 
			&& storage.machineSettings->LowPressureToMeasure)
		{
			// Tell GUI we are opening valve 6
			controls.messageHandler->DisplayMessage(MESSAGE_CHECK_OPENV6_POSSIB, storage.currentData->pressureHigh);

			// Open valve 6
			controls.valveControls->ValveOpen(6, true);

			// Tell GUI we are waiting
			controls.messageHandler->DisplayMessage(MESSAGE_WAIT_TIME, TIME_WAIT_VALVES);

			// Set the time to wait
			WaitSeconds(TIME_WAIT_VALVES);
		}
		// Continue to next step
		storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;
	}

	if (storage.currentData->experimentStepStatus == STEP_STATUS_INPROGRESS
		&& storage.currentData->experimentWaiting == false)							// If waiting is done
	{
		// Open valve 5
		controls.valveControls->ValveOpen(5, true);

		// Tell GUI we are waiting
		controls.messageHandler->DisplayMessage(MESSAGE_WAIT_TIME, TIME_WAIT_VALVES);

		// Set the time to wait
		WaitSeconds(TIME_WAIT_VALVES);

		// Continue to next step
		storage.currentData->experimentStepStatus = STEP_STATUS_END;
	}

	if (storage.currentData->experimentStepStatus == STEP_STATUS_END
		&& storage.currentData->experimentWaiting == false)							// If waiting is done
	{
		// Check residual pressure
		if (storage.currentData->pressureHigh >= storage.machineSettings->PressionLimiteVide)
		{
			controls.messageHandler->DisplayMessageBox(MESSAGE_WARNING_INITIAL_PRESSURE, MB_ICONQUESTION | MB_OKCANCEL, true, storage.currentData->pressureHigh, storage.machineSettings->PressionLimiteVide);
			eventPause = true;
			storage.automationControl.notify_all();
		}
		storage.currentData->experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}


bool Automation::VerificationTemperature()
{
	if (storage.currentData->experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		controls.messageHandler->DisplayMessage(MESSAGE_CHECK_INITIAL_TEMPERATURE);

		if ((storage.currentData->temperatureCalo < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) || (storage.currentData->temperatureCalo > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial) ||
			(storage.currentData->temperatureCage < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) || (storage.currentData->temperatureCage > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial))
		{
			// Tell GUI we are waiting
			controls.messageHandler->DisplayMessage(MESSAGE_WAIT_TEMP_EQUILIBRATION);
			controls.messageHandler->DisplayMessageBox(MESSAGE_CHECK_TEMPERATURE_DIFF, MB_ICONQUESTION | MB_YESNOCANCEL, true, storage.currentData->temperatureCalo, storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial);

			// Pause
			eventPause = true;
			storage.automationControl.notify_all();
			storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;
		}
		else
		{
			storage.currentData->experimentStepStatus = STEP_STATUS_END;
		}
	}

	if (storage.currentData->experimentStepStatus == STEP_STATUS_INPROGRESS)
	{
		if (sb_userContinue)
		{
			sb_userContinue = false;
			storage.currentData->experimentStepStatus = STEP_STATUS_END;
		}
		else
		{
			// Loop until the temperature is stable
			if (!(storage.currentData->temperatureCalo < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) && !(storage.currentData->temperatureCalo > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial) &&
				!(storage.currentData->temperatureCage < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) && !(storage.currentData->temperatureCage > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial))
			{
				storage.currentData->experimentStepStatus = STEP_STATUS_END;
			}
		}
	}

	if (storage.currentData->experimentStepStatus == STEP_STATUS_END)
	{
		storage.currentData->experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}

bool Automation::VerificationComplete()
{
	// Go to the next step
	storage.currentData->experimentStage = STAGE_EQUILIBRATION;
	storage.currentData->experimentStepStatus = STEP_STATUS_START;

	return true;
}