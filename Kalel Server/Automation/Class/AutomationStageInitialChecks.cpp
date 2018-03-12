#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			INITIAL CHECKS
*
*
*/


void Automation::Verifications()
{
	switch (storage.experimentStatus->verificationStep)
	{
	case STEP_VERIFICATIONS_SECURITY:
		if (VerificationSecurity()) {
			storage.experimentStatus->verificationStep = STEP_VERIFICATIONS_VALVES;
		}
		break;

	case STEP_VERIFICATIONS_VALVES:
		if (VerificationValves()){
			storage.experimentStatus->verificationStep = STEP_VERIFICATIONS_PRESSURE;
		}
		break;

	case STEP_VERIFICATIONS_PRESSURE:
		if (VerificationResidualPressure()){
			storage.experimentStatus->verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
		}
		break;

	case STEP_VERIFICATIONS_TEMPERATURE:
		if (VerificationTemperature()) {
			storage.experimentStatus->verificationStep = STEP_VERIFICATIONS_COMPLETE;
		}
		break;

	case STEP_VERIFICATIONS_COMPLETE:
		if (VerificationComplete()) {
			storage.experimentStatus->verificationStep = STEP_VERIFICATIONS_SECURITY;
		}
		break;
	}
}


bool Automation::VerificationSecurity()
{
	if (!storage.machineSettings->ActivationSecurite)
	{
		// Ask user if they want to continue
		LOG(logEVENT) << MESSAGE_NOSECURITY;
		eventPause = true;
		storage.automationControl.notify_all();
	}

	return true;
}


bool Automation::VerificationValves()
{
	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_START)
	{
		// Ask user to check the valves
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_STATE;
		LOG(logEVENT) << MESSAGE_CHECK_VALVES_OPEN;

		// Pause
		eventPause = true;
		storage.automationControl.notify_all();

		// Continue to next step
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_END;
		return false;
	}

	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_END)
	{
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}


bool Automation::VerificationResidualPressure()
{
	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_PRESSURE;

		if (storage.currentData->pressureHigh < storage.machineSettings->PressionSecuriteBassePression)
		{
			// Tell GUI we are opening valve 6
			LOG(logINFO) << MESSAGE_CHECK_OPENV6_POSSIB << storage.currentData->pressureHigh;

			// Open valve 6
			controls.valveControls.ValveOpen(6, true);

			// Tell GUI we are waiting
			LOG(logINFO) << MESSAGE_WAIT_TIME << TIME_WAIT_VALVES;

			// Set the time to wait
			WaitSeconds(TIME_WAIT_VALVES);
		}
		// Continue to next step
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_INPROGRESS;
	}

	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_INPROGRESS
		&& storage.experimentStatus->experimentWaiting == false)							// If waiting is done
	{
		// Open valve 5
		controls.valveControls.ValveOpen(5, true);

		// Tell GUI we are waiting
		LOG(logINFO) << MESSAGE_WAIT_TIME, TIME_WAIT_VALVES;

		// Set the time to wait
		WaitSeconds(TIME_WAIT_VALVES);

		// Continue to next step
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_END;
	}

	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_END
		&& storage.experimentStatus->experimentWaiting == false)							// If waiting is done
	{
		// Check residual pressure
		if (storage.currentData->pressureHigh >= storage.machineSettings->PressionLimiteVide)
		{
			LOG(logWARNING) << MESSAGE_WARNING_INITIAL_PRESSURE << storage.currentData->pressureHigh << storage.machineSettings->PressionLimiteVide;
			eventPause = true;
			storage.automationControl.notify_all();
		}
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}


bool Automation::VerificationTemperature()
{
	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_TEMPERATURE;

		if ((storage.currentData->temperatureCalo < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) || (storage.currentData->temperatureCalo > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial) ||
			(storage.currentData->temperatureCage < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) || (storage.currentData->temperatureCage > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial))
		{
			// Tell GUI we are waiting
			LOG(logINFO) << MESSAGE_WAIT_TEMP_EQUILIBRATION;
			LOG(logWARNING) << MESSAGE_CHECK_TEMPERATURE_DIFF << storage.currentData->temperatureCalo << storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial;

			// Pause
			eventPause = true;
			storage.automationControl.notify_all();
			storage.experimentStatus->experimentStepStatus = STEP_STATUS_INPROGRESS;
		}
		else
		{
			storage.experimentStatus->experimentStepStatus = STEP_STATUS_END;
		}
	}

	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_INPROGRESS)
	{
		if (sb_userContinue)
		{
			sb_userContinue = false;
			storage.experimentStatus->experimentStepStatus = STEP_STATUS_END;
		}
		else
		{
			// Loop until the temperature is stable
			if (!(storage.currentData->temperatureCalo < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) && !(storage.currentData->temperatureCalo > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial) &&
				!(storage.currentData->temperatureCage < storage.experimentSettings->dataGeneral.temperature_experience - security_temperature_initial) && !(storage.currentData->temperatureCage > storage.experimentSettings->dataGeneral.temperature_experience + security_temperature_initial))
			{
				storage.experimentStatus->experimentStepStatus = STEP_STATUS_END;
			}
		}
	}

	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_END)
	{
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_START;
		return true;
	}

	return false;
}

bool Automation::VerificationComplete()
{
	// Go to the next step
	storage.experimentStatus->experimentStage = STAGE_EQUILIBRATION;
	storage.experimentStatus->experimentStepStatus = STEP_STATUS_START;

	return true;
}