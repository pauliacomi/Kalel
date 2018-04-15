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
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:
		LOG(logINFO) << MESSAGE_VERIFICATIONS;
		storage.experimentStatus.stepStatus = STEP_VERIFICATIONS_SECURITY;
		break;

	case STEP_VERIFICATIONS_SECURITY:
		if (VerificationSecurity()) {
			storage.experimentStatus.stepStatus = STEP_VERIFICATIONS_VALVES;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_VALVES:
		if (VerificationValves()){
			storage.experimentStatus.stepStatus = STEP_VERIFICATIONS_PRESSURE;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_PRESSURE:
		if (VerificationResidualPressure()){
			storage.experimentStatus.stepStatus = STEP_VERIFICATIONS_TEMPERATURE;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
		}
		break;

	case STEP_VERIFICATIONS_TEMPERATURE:
		if (VerificationTemperature()) {
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
		}
		break;

	case STEP_STATUS_END:

		// Close valves
		controls.valveControls.ValveClose(ID_VALVE_1, false);
		controls.valveControls.ValveClose(ID_VALVE_2, false);
		controls.valveControls.ValveClose(ID_VALVE_3, false);
		controls.valveControls.ValveClose(ID_VALVE_4, false);
		controls.valveControls.ValveClose(ID_VALVE_5, false);
		controls.valveControls.ValveClose(ID_VALVE_7, false);
		controls.valveControls.ValveClose(ID_VALVE_8, false);
		controls.valveControls.CloseEVsAndPump(false);

		LOG(logINFO) << MESSAGE_VERIFICATIONS_COMPLETE;
		storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;
		++storage.experimentStatus.mainStage;
		break;
	}
}


bool Automation::VerificationSecurity()
{
	if (!storage.machineSettings.SafetyOn)
	{
		switch (storage.experimentStatus.substepStatus)
		{
		case SUBSTEP_STATUS_START:
			// Ask user if they want to continue
			LOG_EVENT(qYESNO) << MESSAGE_NOSECURITY;

			eventUserInput = true;
			storage.automationControl.notify_all();
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
			break;

		case SUBSTEP_STATUS_INPROGRESS:
			switch (userChoice)
			{
			case CHOICE_YES:									// Signal that it's good
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
				break;

			case CHOICE_NO:										// Stop experiment
				shutdownReason = Stop::Cancel;
				eventShutdown = true;
				storage.automationControl.notify_all();
				break;

			case CHOICE_NONE:									// Do nothing yet
			default:
				break;
			}
			break;

		case SUBSTEP_STATUS_END:
			return true;
			break;

		default:
			break;
		}
		return false;
	}
	return true;
}


bool Automation::VerificationValves()
{
	switch (storage.experimentStatus.substepStatus)
	{
	case SUBSTEP_STATUS_START:
		// Ask user to check the valves
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_STATE;
		LOG_EVENT(qOK) << MESSAGE_CHECK_VALVES_OPEN;

		// Set wait
		eventUserInput = true;
		storage.automationControl.notify_all();
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
		break;

	case SUBSTEP_STATUS_INPROGRESS:
		switch (userChoice)
		{
		case CHOICE_YES:									// Signal that it's good
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
			break;

		case CHOICE_NONE:									// Do nothing yet
		default:
			break;
		}
		break;

	case SUBSTEP_STATUS_END:
		return true;
		break;

	default:
		break;
	}
	return false;
}


bool Automation::VerificationResidualPressure()
{
	switch (storage.experimentStatus.substepStatus)
	{
	case SUBSTEP_STATUS_START:
		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_PRESSURE;

		// Open valve 4
		controls.valveControls.ValveOpen(ID_VALVE_4, true);

		// Set the time to wait
		WaitSeconds(storage.machineSettings.TimeWaitValves, true);

		// Continue to next step
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
		break;

	case SUBSTEP_STATUS_INPROGRESS:
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
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 1;
		break;

	case SUBSTEP_STATUS_INPROGRESS + 1:

		if (storage.currentData.pressureHigh >= storage.machineSettings.PressureLimitVacuum)
		{
			// Ask user if they want to continue
			LOG_EVENT(qYESNO) << stringh::string_format(MESSAGE_WARNING_INITIAL_PRESSURE,
				storage.currentData.pressureHigh.load(), storage.machineSettings.PressureLimitVacuum);

			eventUserInput = true;
			storage.automationControl.notify_all();
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 2;
		}
		else
		{
			// Continue to next step
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 3;
		}

		break;

	case SUBSTEP_STATUS_INPROGRESS + 2:

		switch (userChoice)
		{
		case CHOICE_YES:									// Signal that it's good
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 3;
			break;

		case CHOICE_NO:										// Stop experiment
			shutdownReason = Stop::Cancel;
			eventShutdown = true;
			storage.automationControl.notify_all();
			break;

		case CHOICE_NONE:									// Do nothing yet
		default:
			break;
		}
		break;

	case SUBSTEP_STATUS_INPROGRESS + 3:

		// Open valve 5
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		// Set the time to wait
		WaitSeconds(storage.machineSettings.TimeWaitValves, true);

		// Continue to next step
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 4;
		break;


	case SUBSTEP_STATUS_INPROGRESS + 4:
		if (storage.currentData.pressureHigh >= storage.machineSettings.PressureLimitVacuum)
		{
			// Ask user if they want to continue
			LOG_EVENT(qYESNO) << stringh::string_format(MESSAGE_WARNING_INITIAL_PRESSURE,
				storage.currentData.pressureHigh.load(), storage.machineSettings.PressureLimitVacuum);

			eventUserInput = true;
			storage.automationControl.notify_all();
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 5;
		}
		else
		{
			controls.valveControls.ValveClose(ID_VALVE_4, true);
			controls.valveControls.ValveClose(ID_VALVE_5, true);
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
		}
		break;


	case SUBSTEP_STATUS_INPROGRESS + 5:

		switch (userChoice)
		{
		case CHOICE_YES:									// Signal that it's good
			controls.valveControls.ValveClose(ID_VALVE_4, true);
			controls.valveControls.ValveClose(ID_VALVE_5, true);
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
			break;

		case CHOICE_NO:										// Stop experiment
			shutdownReason = Stop::Cancel;
			eventShutdown = true;
			storage.automationControl.notify_all();
			break;

		case CHOICE_NONE:									// Do nothing yet
		default:
			break;
		}
		break;

	case SUBSTEP_STATUS_END:
		return true;
		break;

	default:
		break;
	}
	return false;
}


bool Automation::VerificationTemperature()
{
	switch (storage.experimentStatus.substepStatus)
	{
	case SUBSTEP_STATUS_START:

		// Display initial message
		LOG(logINFO) << MESSAGE_CHECK_INITIAL_TEMPERATURE;

		if ((storage.currentData.temperatureCalo < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCalo > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check))
		{
			LOG_EVENT(qYESTRYCANCEL) << stringh::string_format(MESSAGE_CHECK_TEMPERATURE_DIFF, storage.currentData.temperatureCalo.load(),
				storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check);

			eventUserInput = true;
			storage.automationControl.notify_all();
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS;
		}
		else {
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
		}
		break;

	case SUBSTEP_STATUS_INPROGRESS:
		switch (userChoice)
		{
		case CHOICE_YES:									// Signal that it's good
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;
			break;

		case CHOICE_WAIT:									// Go to wait step
			LOG(logINFO) << MESSAGE_WAIT_TEMP_EQUILIBRATION;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INPROGRESS + 1;
			break;

		case CHOICE_NO:										// Stop experiment
			shutdownReason = Stop::Cancel;
			eventShutdown = true;
			storage.automationControl.notify_all();
			break;

		case CHOICE_NONE:									// Do nothing yet
		default:
			break;
		}
		break;

	case SUBSTEP_STATUS_INPROGRESS + 1:
		// Loop until the temperature is stable
		if ((storage.currentData.temperatureCalo < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCalo > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage < storage.experimentSettings.dataGeneral.temperature_experience - storage.experimentSettings.dataGeneral.temperature_range_initial_check) ||
			(storage.currentData.temperatureCage > storage.experimentSettings.dataGeneral.temperature_experience + storage.experimentSettings.dataGeneral.temperature_range_initial_check))
		{
			break;
		}
		else
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;;
		break;

	case SUBSTEP_STATUS_END:
		return true;
		break;

	default:
		break;
	}

	return false;
}
