#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"
#include "../../../Kalel Shared/stringHelpers.h"

/*
*
*
*			INITIAL CHECKS
*
*
*/


void Automation::Verifications()
{
	switch (storage.experimentStatus.checksStage)
	{
	case STEP_VERIFICATIONS_SECURITY:
		if (VerificationSecurity()) {
			storage.experimentStatus.checksStage = STEP_VERIFICATIONS_VALVES;
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_VALVES:
		if (VerificationValves()){
			storage.experimentStatus.checksStage = STEP_VERIFICATIONS_PRESSURE;
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_PRESSURE:
		if (VerificationResidualPressure()){
			storage.experimentStatus.checksStage = STEP_VERIFICATIONS_TEMPERATURE;
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_TEMPERATURE:
		if (VerificationTemperature()) {
			storage.experimentStatus.checksStage = STEP_VERIFICATIONS_COMPLETE;
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_COMPLETE:
		if (VerificationComplete()) {
			storage.experimentStatus.mainStage = STAGE_EQUILIBRATION;
			storage.experimentStatus.stepStatus = STEP_STATUS_START;
		}
		break;
	}
}


bool Automation::VerificationSecurity()
{
	if (!storage.machineSettings.SafetyOn)
	{
		if (!storage.experimentStatus.isWaitingUser) {
			// Ask user if they want to continue
			LOG_EVENT(qYESNO) << MESSAGE_NOSECURITY;

			storage.experimentStatus.isWaitingUser = true;
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
				storage.experimentStatus.isWaitingUser = false;
				return true;
				break;
			case CHOICE_NO:										// Stop experiment

				shutdownReason = Stop::Cancel;
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
	if (!storage.experimentStatus.isWaitingUser) {
		// Ask user to check the valves
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_STATE;
		LOG_EVENT(qOK) << MESSAGE_CHECK_VALVES_OPEN;

		// Set wait
		storage.experimentStatus.isWaitingUser = true;
		eventUserInput = true;
		storage.automationControl.notify_all();

		return false;
	}
	else {
		switch (userChoice)
		{
		case CHOICE_NONE:									// Do nothing yet
			break;
		case CHOICE_YES:									// Signal that it's good
			storage.experimentStatus.isWaitingUser = false;
			return true;
			break;
		default:
			break;
		}
		return false;
	}
}


bool Automation::VerificationResidualPressure()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_START:
		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_PRESSURE;

		// Open valve 4
		controls.valveControls.ValveOpen(ID_VALVE_4, true);

		// Set the time to wait
		WaitSeconds(storage.machineSettings.TimeWaitValves, true);

		// Continue to next step
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;
		break;

	case STEP_STATUS_INPROGRESS:
		if (storage.experimentStatus.isWaiting) break;

		if (storage.currentData.pressureHigh < storage.machineSettings.readers.find(PRESSURE_LP)->second.safe_max)
		{
			// Tell GUI we are opening valve 6
			LOG(logINFO) << stringh::string_format(MESSAGE_CHECK_OPENV6_POSSIB, storage.currentData.pressureHigh.load());

			// Open valve 6
			controls.valveControls.ValveOpen(ID_VALVE_6, true);

			// Set the time to wait
			WaitSeconds(storage.machineSettings.TimeWaitValves, true);
		}
		// Continue to next step
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 1;
		break;

	case STEP_STATUS_INPROGRESS + 1:
		if (storage.experimentStatus.isWaiting) break;

		if (storage.currentData.pressureHigh >= storage.machineSettings.PressureLimitVacuum)
		{
			if (!storage.experimentStatus.isWaitingUser) {

				// Ask user if they want to continue
				LOG_EVENT(qYESNO) << stringh::string_format(MESSAGE_WARNING_INITIAL_PRESSURE,
					storage.currentData.pressureHigh.load(), storage.machineSettings.PressureLimitVacuum);

				storage.experimentStatus.isWaitingUser = true;
				eventUserInput = true;
				storage.automationControl.notify_all();
			}
			else
			{
				switch (userChoice)
				{
				case CHOICE_NONE:									// Do nothing yet
					break;
				case CHOICE_YES:									// Signal that it's good
					storage.experimentStatus.isWaitingUser = false;
					return true;
					break;
				case CHOICE_NO:										// Stop experiment
					shutdownReason = Stop::Cancel;
					eventShutdown = true;
					storage.automationControl.notify_all();
					break;
				default:
					break;
				}
			}
		}
		else
		{
			// Continue to next step
			storage.experimentStatus.isWaitingUser = false;
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 2;
		}

		break;

	case STEP_STATUS_INPROGRESS + 2:
		if (storage.experimentStatus.isWaiting) break;

		// Open valve 5
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		// Set the time to wait
		WaitSeconds(storage.machineSettings.TimeWaitValves, true);

		// Continue to next step
		storage.experimentStatus.stepStatus = STEP_STATUS_END;
		break;

	case STEP_STATUS_END:
		if (storage.experimentStatus.isWaiting) break;

		if (storage.currentData.pressureHigh >= storage.machineSettings.PressureLimitVacuum)
		{
			if (!storage.experimentStatus.isWaitingUser) {
				// Ask user if they want to continue
				LOG_EVENT(qYESNO) << stringh::string_format(MESSAGE_WARNING_INITIAL_PRESSURE, 
					storage.currentData.pressureHigh.load(), storage.machineSettings.PressureLimitVacuum);

				storage.experimentStatus.isWaitingUser = true;
				eventUserInput = true;
				storage.automationControl.notify_all();
			}
			else
			{
				switch (userChoice)
				{
				case CHOICE_NONE:									// Do nothing yet
					break;
				case CHOICE_YES:									// Signal that it's good
					controls.valveControls.ValveClose(ID_VALVE_4, true);
					controls.valveControls.ValveClose(ID_VALVE_5, true);
					controls.valveControls.CloseAll(true);			// Close valves
					return true;
					break;
				case CHOICE_NO:										// Stop experiment
					shutdownReason = Stop::Cancel;
					eventShutdown = true;
					storage.automationControl.notify_all();
					break;
				default:
					break;
				}
			}
		}
		else
		{
			controls.valveControls.ValveClose(ID_VALVE_4, true);
			controls.valveControls.ValveClose(ID_VALVE_5, true);
			storage.experimentStatus.isWaitingUser = false;			// If somehow it got here with this true
			return true;
		}
		break;

	default:
		break;
	}
	return false;
}


bool Automation::VerificationTemperature()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_START:

		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_TEMPERATURE;

		if ((storage.currentData.temperatureCalo < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCalo > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check))
		{
			if (!storage.experimentStatus.isWaitingUser) {

				LOG_EVENT(qYESTRYCANCEL) << stringh::string_format(MESSAGE_CHECK_TEMPERATURE_DIFF, storage.currentData.temperatureCalo.load(),
					storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check);

				storage.experimentStatus.isWaitingUser = true;
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
					storage.experimentStatus.isWaitingUser = false;
					return true;
					break;
				case CHOICE_WAIT:									// Go to wait step
					LOG(logINFO) << MESSAGE_WAIT_TEMP_EQUILIBRATION;
					storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;
					return false;
					break;
				case CHOICE_NO:										// Stop experiment

					shutdownReason = Stop::Cancel;
					eventShutdown = true;
					storage.automationControl.notify_all();
					return false;
					break;
				default:
					break;
				}
			}
		}
		else {
			storage.experimentStatus.isWaitingUser = false;			// If somehow it got here with this true
			return true;
		}
		break;

	case STEP_STATUS_INPROGRESS:
		// Loop until the temperature is stable
		if ((storage.currentData.temperatureCalo < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCalo > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check))
		{
			return false;
		}
		else return true;
		break;
	default:
		break;
	}

	return false;
}

bool Automation::VerificationComplete()
{
	// Log complete
	LOG(logINFO) << MESSAGE_VERIFICATIONS_COMPLETE;

	return true;
}