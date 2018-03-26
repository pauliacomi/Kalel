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
	switch (storage.experimentStatus.verificationStep)
	{
	case STEP_VERIFICATIONS_SECURITY:
		if (VerificationSecurity()) {
			storage.experimentStatus.verificationStep = STEP_VERIFICATIONS_VALVES;
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_VALVES:
		if (VerificationValves()){
			storage.experimentStatus.verificationStep = STEP_VERIFICATIONS_PRESSURE;
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_PRESSURE:
		if (VerificationResidualPressure()){
			storage.experimentStatus.verificationStep = STEP_VERIFICATIONS_TEMPERATURE;
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_TEMPERATURE:
		if (VerificationTemperature()) {
			storage.experimentStatus.verificationStep = STEP_VERIFICATIONS_COMPLETE;
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_COMPLETE:
		if (VerificationComplete()) {
			storage.experimentStatus.experimentStage = STAGE_EQUILIBRATION;
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		}
		break;
	}
}


bool Automation::VerificationSecurity()
{
	if (!storage.machineSettings.SafetyOn)
	{
		if (!waitingUser) {
			// Ask user if they want to continue
			LOG_EVENT(qYESNO) << MESSAGE_NOSECURITY;

			waitingUser = true;
			eventUserInput = true;
			storage.automationControl.notify_all();

			return false;
		}
		else
		{
			switch (userChoice)
			{
			case CHOICE_NONE:									// Do nothing yet
				return false;
				break;
			case CHOICE_YES:									// Signal that it's good
				waitingUser = false;
				return true;
				break;
			case CHOICE_NO:										// Stop experiment

				shutdownReason = STOP_CANCEL;
				eventShutdown = true;
				storage.automationControl.notify_all();
				return false;
				break;
			default:
				return false;
				break;
			}
		}
	}

	return true;
}


bool Automation::VerificationValves()
{
	if (!waitingUser) {
		// Ask user to check the valves
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_STATE;
		LOG_EVENT(qOK) << MESSAGE_CHECK_VALVES_OPEN;

		// Set wait
		waitingUser = true;
		eventUserInput = true;
		storage.automationControl.notify_all();

		return false;
	}
	else {
		switch (userChoice)
		{
		case CHOICE_NONE:									// Do nothing yet
			return false;
			break;
		case CHOICE_YES:									// Signal that it's good
			waitingUser = false;
			return true;
			break;
		default:
			return false;
			break;
		}
	}
}


bool Automation::VerificationResidualPressure()
{
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_PRESSURE;

		if (storage.currentData.pressureHigh < storage.machineSettings.readers.find(PRESSURE_LP)->second.safe_max)
		{
			// Tell GUI we are opening valve 6
			LOG(logINFO) << MESSAGE_CHECK_OPENV6_POSSIB << storage.currentData.pressureHigh;

			// Open valve 6
			controls.valveControls.ValveOpen(ID_VALVE_6, true);

			// Set the time to wait
			WaitSeconds(storage.machineSettings.TimeWaitValves, true);
		}
		// Continue to next step
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS;
		return false;
	}

	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_INPROGRESS
		&& storage.experimentStatus.experimentWaiting == false)							// If waiting is done
	{
		// Open valve 5
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		// Set the time to wait
		WaitSeconds(storage.machineSettings.TimeWaitValves, true);

		// Continue to next step
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_END;
		return false;
	}

	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_END
		&& storage.experimentStatus.experimentWaiting == false)							// If waiting is done
	{
		if (storage.currentData.pressureHigh >= storage.machineSettings.PressureLimitVacuum)
		{
			if (!waitingUser) {
				// Ask user if they want to continue
				LOG_EVENT(qYESNO) << MESSAGE_WARNING_INITIAL_PRESSURE << storage.currentData.pressureHigh << storage.machineSettings.PressureLimitVacuum;

				waitingUser = true;
				eventUserInput = true;
				storage.automationControl.notify_all();

				return false;
			}
			else
			{
				switch (userChoice)
				{
				case CHOICE_NONE:									// Do nothing yet
					return false;
					break;
				case CHOICE_YES:									// Signal that it's good
					waitingUser = false;
					return true;
					break;
				case CHOICE_NO:										// Stop experiment

					shutdownReason = STOP_CANCEL;
					eventShutdown = true;
					storage.automationControl.notify_all();
					return false;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			return true;
		}
	}

	return false;
}


bool Automation::VerificationTemperature()
{
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_START)
	{
		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_TEMPERATURE;

		if ((storage.currentData.temperatureCalo < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCalo > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) || 
			(storage.currentData.temperatureCage > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check))
		{
			if (!waitingUser) {

				LOG_EVENT(qYESTRYCANCEL) << MESSAGE_CHECK_TEMPERATURE_DIFF << storage.currentData.temperatureCalo << storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check;

				waitingUser = true;
				eventUserInput = true;
				storage.automationControl.notify_all();

				return false;
			}
			else {
				switch (userChoice)
				{
				case CHOICE_NONE:									// Do nothing yet
					return false;
					break;
				case CHOICE_YES:									// Signal that it's good
					waitingUser = false;
					return true;
					break;
				case CHOICE_WAIT:									// Go to wait step
					LOG(logINFO) << MESSAGE_WAIT_TEMP_EQUILIBRATION;
					storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS;
					return false;
					break;
				case CHOICE_NO:										// Stop experiment

					shutdownReason = STOP_CANCEL;
					eventShutdown = true;
					storage.automationControl.notify_all();
					return false;
					break;
				default:
					break;
				}
			}
		}
		else return true;
	
	}

	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_INPROGRESS)
	{
		// Loop until the temperature is stable
		if ((storage.currentData.temperatureCalo < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCalo > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check))
		{
			return false;
		}
		else return true;
	}

	return false;
}

bool Automation::VerificationComplete()
{
	// Log complete
	LOG(logINFO) << MESSAGE_VERIFICATIONS_COMPLETE;

	return true;
}