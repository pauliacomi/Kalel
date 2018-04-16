#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"
#include "../../../Kalel Shared/stringHelpers.h"

/*
*
*
*			ADSORPTION
*
*
*/


void Automation::StageDiscreteAdsorption()
{
	switch (storage.experimentStatus.stepStatus)
	{

	case STEP_STATUS_UNDEF:

		LOG(logINFO) << MESSAGE_ADSORPTION_START;																					// Log the step change
		
		// Close valves
		controls.valveControls.ValveClose(ID_VALVE_2, false);
		controls.valveControls.ValveClose(ID_VALVE_3, false);
		controls.valveControls.ValveClose(ID_VALVE_4, false);
		controls.valveControls.ValveClose(ID_VALVE_5, false);
		controls.valveControls.ValveClose(ID_VALVE_7, false);
		controls.valveControls.ValveClose(ID_VALVE_8, false);

		storage.experimentStatus.pressureInitial = storage.currentData.pressureHigh.load();											// Set the initial pressure
		storage.experimentStatus.stepStatus = STEP_STATUS_START;																	// Set next step
		break;
		
	case STEP_STATUS_START:
	{
		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_STAGE_START, storage.experimentStatus.stepCounter.load());		// Log the step change
		storage.experimentStatus.injectionDose = 0;

		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;																// Set next step
		break;
	}
	case STEP_STATUS_INPROGRESS:

		// Check if the pressure for this adsorption step has been reached
		if (storage.experimentStatus.pressureFinal > storage.experimentSettings.dataAdsorption[storage.experimentStatus.stepCounter].pression_finale) {
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
		}
		// If not, do another injection
		else
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS + 1;

		break;

	case STEP_STATUS_INPROGRESS + 1:

		// Go through the adsorption substeps
		if (SubstepsDiscreteAdsorption()) {
			storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;
		}
		break;

	case STEP_STATUS_END:

		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_STAGE_END, storage.experimentStatus.stepCounter.load());		// Log the step change
		
		// Make sure we don't skip recording
		storage.experimentStatus.isRecording = true;

		// Increment counter
		++storage.experimentStatus.stepCounter;

		// Close valves
		controls.valveControls.ValveClose(ID_VALVE_2, false);
		controls.valveControls.ValveClose(ID_VALVE_3, false);
		controls.valveControls.ValveClose(ID_VALVE_4, false);
		controls.valveControls.ValveClose(ID_VALVE_5, false);
		controls.valveControls.ValveClose(ID_VALVE_7, false);
		controls.valveControls.ValveClose(ID_VALVE_8, false);
		controls.valveControls.CloseEVsAndPump(false);
		
		if (storage.experimentStatus.stepCounter < storage.experimentSettings.dataAdsorption.size())
		{
			storage.experimentStatus.stepStatus = STEP_STATUS_START;																	// Reset substep
		}
		else
		{
			LOG(logINFO) << MESSAGE_ADSORPTION_END;																						// Log the step change
			++storage.experimentStatus.mainStage;
			storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;																	// Reset substep
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
			storage.experimentStatus.stepCounter = 0; // Reset counter
		}
		break;
	}
}


bool Automation::SubstepsDiscreteAdsorption()
{
	switch (storage.experimentStatus.substepStatus)
	{

	/****************
	Initial
	****************/
	case SUBSTEP_STATUS_START:
	{
		storage.experimentStatus.injectionAttemptCounter = 0;																						// Reset injection attempt counter
		// Check for safety
		auto p_lp = storage.machineSettings.readers.find(PRESSURE_LP);
		if (p_lp != storage.machineSettings.readers.end())
		{
			if (storage.experimentStatus.pressureHighOld > 0.9 * p_lp->second.safe_max)
			{
				controls.valveControls.ValveClose(ID_VALVE_6, false);
			}
		}
		storage.experimentStatus.pressureHighOld = storage.experimentStatus.pressureInitial.load();													// Save the "old" pressure

		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_DOSE_START,
			storage.experimentStatus.stepCounter.load(), storage.experimentStatus.injectionDose.load());											// Log current dose
		storage.experimentStatus.isRecording = false;
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;																			// Move to injection
		break;
	}
	/****************
	Injection
	****************/
	case SUBSTEP_STATUS_INJECTION:
		LOG(logINFO) << stringh::string_format(MESSAGE_INJECTION_ATTEMPT, storage.experimentStatus.injectionAttemptCounter.load());					// Log current injection
		controls.valveControls.ValveOpen(ID_VALVE_4, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 1;
		break;

	case SUBSTEP_STATUS_INJECTION + 1:
		controls.valveControls.ValveOpen(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 2;
		break;

	case SUBSTEP_STATUS_INJECTION + 2:
		controls.valveControls.ValveClose(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 3;
		break;

	case SUBSTEP_STATUS_INJECTION + 3:
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 4;
		break;

	case SUBSTEP_STATUS_INJECTION + 4:
		controls.valveControls.ValveClose(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;								// Move to injection check
		break;

	/****************
	Check
	****************/
	case SUBSTEP_STATUS_CHECK:

		// Display
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_PI, storage.experimentStatus.pressureInitial.load());
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_PF, storage.currentData.pressureHigh.load());
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_DP, storage.currentData.pressureHigh - storage.experimentStatus.pressureInitial);
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_DPREQ, storage.experimentSettings.dataAdsorption[storage.experimentStatus.stepCounter].delta_pression);
		LOG(logINFO) << stringh::string_format(MESSAGE_INJECTION_END, storage.experimentStatus.injectionAttemptCounter.load());

		// Checks for injection succeess, else increment the injection counter and try again
		if (storage.experimentStatus.pressureHighOld + storage.machineSettings.InjectionMargin > storage.currentData.pressureHigh)
		{
			// If too many injections have been tried and failed
			if (storage.experimentStatus.injectionAttemptCounter >= storage.machineSettings.attemptNumber)
			{
				// Put the thread on stand-by
				eventPause = true;
				storage.automationControl.notify_all();

				// Tell GUI
				LOG(logINFO) << MESSAGE_INJECTION_PROBLEM;
				LOG_EVENT(qOK) << MESSAGE_INJECTION_PROBLEM_BOX;

				// Reset counter
				storage.experimentStatus.injectionAttemptCounter = 0;
			}
			else
			{
				// If not, increment the counter and try again
				++storage.experimentStatus.injectionAttemptCounter;
			}

			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;
		}
		else
		{
			// If the injection succeeded											

			// Check if injection has undershot
			if (storage.currentData.pressureHigh - storage.experimentStatus.pressureInitial <
				storage.experimentSettings.dataAdsorption[storage.experimentStatus.stepCounter].delta_pression)
			{
				// Reset counter
				storage.experimentStatus.injectionAttemptCounter = 0;
				storage.experimentStatus.pressureHighOld = storage.currentData.pressureHigh.load();
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;																	// Inject more gas
			}
			// Check if injection has overshot
			else if (storage.currentData.pressureHigh - storage.experimentStatus.pressureInitial >
				storage.machineSettings.InjectionMultiplier * (storage.experimentSettings.dataAdsorption[storage.experimentStatus.stepCounter].delta_pression))
			{
				// Reset counter
				storage.experimentStatus.injectionAttemptCounter = 0;
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL;																	// Remove gas
			}
			// If completeted successfully go to equilibration
			else
			{
				LOG(logINFO) << MESSAGE_EQUILIBRATION_REFVOL;
				controls.valveControls.ValveClose(ID_VALVE_4, false);																			// Close valve 4
				controls.valveControls.CloseEVsAndPump(false);																					// Close pump if needed
				WaitMinutes(storage.experimentSettings.dataAdsorption[storage.experimentStatus.stepCounter].temps_volume, true);				// Set the time to wait for equilibration in the reference volume
				storage.experimentStatus.isRecording = true;
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ADSORPTION;																// Go to adsorption
			}
		}
		break;

	/****************
	Remove gas
	****************/
	case SUBSTEP_STATUS_REMOVAL:
		LOG(logINFO) << stringh::string_format(MESSAGE_OUTGAS_ATTEMPT, storage.experimentStatus.injectionAttemptCounter.load());						// Log injection
		controls.valveControls.OpenEVsAndPump(true);
		WaitSeconds(storage.machineSettings.TimeWaitPump);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 1;
		break;

	case SUBSTEP_STATUS_REMOVAL + 1:
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 2;
		break;

	case SUBSTEP_STATUS_REMOVAL + 2:
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 3;
		break;

	case SUBSTEP_STATUS_REMOVAL + 3:
		controls.valveControls.ValveOpen(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 4;
		break;

	case SUBSTEP_STATUS_REMOVAL + 4:
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;							// Go back to the start
		break;


	/****************
	Adsorption step
	****************/
	case SUBSTEP_STATUS_ADSORPTION:
		LOG(logINFO) << MESSAGE_ADSORPTION_OPENV;

		// Open valve
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		// Wait for adsorption
		WaitMinutes(storage.experimentSettings.dataAdsorption[storage.experimentStatus.stepCounter].temps_adsorption, true);		// Set the time to wait
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;																// Go to next step
		break;
	
	/****************
	Final
	****************/
	case SUBSTEP_STATUS_END:
		// Display sample isolation message
		LOG(logINFO) << MESSAGE_ADSORPTION_CLOSEV;

		storage.experimentStatus.pressureFinal = storage.currentData.pressureHigh;														// Save final pressure

		// Close valves
		controls.valveControls.ValveClose(ID_VALVE_2, true);
		controls.valveControls.ValveClose(ID_VALVE_3, true);
		controls.valveControls.ValveClose(ID_VALVE_4, true);
		controls.valveControls.ValveClose(ID_VALVE_5, true);
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		controls.valveControls.ValveClose(ID_VALVE_8, true);

		// Display message to show end of adsorption
		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_DOSE_END,
			storage.experimentStatus.stepCounter.load(), storage.experimentStatus.injectionDose.load());						// Log current dose
	
		storage.experimentStatus.pressureInitial = storage.currentData.pressureHigh;

		// Increment dose
		++storage.experimentStatus.injectionDose;

		// Reset things
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
		return true;
		break;

	default:
		break;
	}
	return false;
}



