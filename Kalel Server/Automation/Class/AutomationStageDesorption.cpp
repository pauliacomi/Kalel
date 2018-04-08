#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"
#include "../../../Kalel Shared/stringHelpers.h"

/*
*
*
*			DESORPTION
*
*
*/


void Automation::StageDesorption()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_UNDEF:

		LOG(logINFO) << MESSAGE_DESORPTION_START;																					// Log the step change
																																	// Close valves
		controls.valveControls.ValveClose(ID_VALVE_2, false);
		controls.valveControls.ValveClose(ID_VALVE_3, false);
		controls.valveControls.ValveClose(ID_VALVE_4, false);
		controls.valveControls.ValveClose(ID_VALVE_5, false);
		controls.valveControls.ValveClose(ID_VALVE_7, false);
		controls.valveControls.ValveClose(ID_VALVE_8, false);

		storage.experimentStatus.injectionDose = 0;																					// Set dose
		storage.experimentStatus.stepStatus = STEP_STATUS_START;																	// Set next step
		break;

	case STEP_STATUS_START:

		LOG(logINFO) << stringh::string_format(MESSAGE_DESORPTION_STAGE_START, storage.experimentStatus.desorptionCounter.load());	// Log the step change
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;																// Set next step
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the desorption substeps
		SubstepsDesorption();

		// Check if the pressure for this desorption stage has been reached
		if (storage.experimentStatus.pressureFinal < storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter].pression_finale) {
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		
		LOG(logINFO) << stringh::string_format(MESSAGE_DESORPTION_STAGE_END, storage.experimentStatus.desorptionCounter.load());		// Log the step change
		
		// Close valves
		controls.valveControls.ValveClose(ID_VALVE_2, false);
		controls.valveControls.ValveClose(ID_VALVE_3, false);
		controls.valveControls.ValveClose(ID_VALVE_4, false);
		controls.valveControls.ValveClose(ID_VALVE_5, false);
		controls.valveControls.ValveClose(ID_VALVE_7, false);
		controls.valveControls.ValveClose(ID_VALVE_8, false);

		if (storage.experimentStatus.desorptionCounter < storage.experimentSettings.dataDesorption.size())
		{
			++storage.experimentStatus.desorptionCounter;
			storage.experimentStatus.stepStatus = STEP_STATUS_START;																	// Reset substep
		}
		else
		{
			LOG(logINFO) << MESSAGE_DESORPTION_END;																						// Log the step change
			++storage.experimentStatus.mainStage;
			storage.experimentStatus.stepStatus = STEP_STATUS_UNDEF;																	// Reset substep
		}
		break;
	}
}



void Automation::SubstepsDesorption()
{
	switch (storage.experimentStatus.substepStatus)
	{

	/****************
	Initial
	****************/
	case SUBSTEP_STATUS_START:
		storage.experimentStatus.injectionAttemptCounter = 0;																							// Reset desorption attempt counter
		storage.experimentStatus.pressureInitial = storage.currentData.pressureHigh.load();																// Set the initial pressure
		storage.experimentStatus.pressureHighOld = storage.currentData.pressureHigh.load();																// Save the injection pressure for later

		LOG(logINFO) << stringh::string_format(MESSAGE_DESORPTION_DOSE_START,
			storage.experimentStatus.desorptionCounter.load(), storage.experimentStatus.injectionDose.load());											// Log about current dose
		storage.experimentStatus.isRecording = false;
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL;																				// Move to removal
		break;

		
	/****************
	Remove gas
	****************/
	case SUBSTEP_STATUS_REMOVAL:
		LOG(logINFO) << stringh::string_format(MESSAGE_OUTGAS_ATTEMPT, storage.experimentStatus.injectionAttemptCounter.load());						// Log injection
		// Turn on pump
		if (!controls.valveControls.PumpIsActive()) {
			controls.valveControls.EVActivate(ID_EV_1, true);
			controls.valveControls.EVActivate(ID_EV_2, true);
			controls.valveControls.PumpActivate(true);
			WaitSeconds(storage.machineSettings.TimeWaitPump);
		}
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 1;
		break;

	case SUBSTEP_STATUS_REMOVAL + 1:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveOpen(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 1;
		break;

	case SUBSTEP_STATUS_REMOVAL + 2:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 2;
		break;

	case SUBSTEP_STATUS_REMOVAL + 3:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL + 3;
		break;

	case SUBSTEP_STATUS_REMOVAL + 4:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;					// Move to removal check
		break;
		
	/****************
	Check
	****************/
	case SUBSTEP_STATUS_CHECK:
		if (storage.experimentStatus.isWaiting) break;

		// Set the final pressure after gas removal
		storage.experimentStatus.pressureFinal = storage.currentData.pressureHigh.load();

		// Display
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_PI, storage.experimentStatus.pressureInitial.load());
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_PF, storage.experimentStatus.pressureFinal.load());
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_DP, storage.experimentStatus.pressureFinal - storage.experimentStatus.pressureInitial);
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_DPREQ, storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter].delta_pression);
		LOG(logINFO) << stringh::string_format(MESSAGE_OUTGAS_END, storage.experimentStatus.injectionAttemptCounter.load());

		// Checks for removal succeess, else increment the counter and try again
		if ((storage.experimentStatus.pressureHighOld - storage.machineSettings.InjectionMargin < storage.currentData.pressureHigh) &&
			(storage.currentData.pressureHigh < storage.experimentStatus.pressureHighOld + storage.machineSettings.InjectionMargin))
		{
			// If too many injections have been tried and failed
			if (storage.experimentStatus.injectionAttemptCounter >= storage.machineSettings.attemptNumber)
			{
				// Put the thread on stand-by
				eventPause = true;
				storage.automationControl.notify_all();

				// Tell GUI
				LOG(logINFO) << MESSAGE_OUTGAS_PROBLEM;
				LOG(logERROR) << MESSAGE_OUTGAS_PROBLEM_BOX;

				// Reset counter
				storage.experimentStatus.injectionAttemptCounter = 0;
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL;
			}

			// If not, increment the counter and try again
			++storage.experimentStatus.injectionAttemptCounter;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_REMOVAL;
		}
		// If the removal succeeded
		else
		{	// Check if removal has overshot
			if (storage.experimentStatus.pressureInitial - storage.experimentStatus.pressureFinal >
				storage.machineSettings.InjectionMultiplier * (storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter].delta_pression))
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;	
			}
			// If completeted successfully go to equilibration
			else
			{
				// Deactivate pump
				if (controls.valveControls.PumpIsActive()) {
					controls.valveControls.EVDeactivate(ID_EV_1, true);
					controls.valveControls.EVDeactivate(ID_EV_2, true);
					controls.valveControls.PumpDeactivate(true);
				}
				storage.experimentStatus.isRecording = true;
				WaitMinutes(storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter].temps_volume);					// Set the time to wait for equilibration in the reference volume
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_DESORPTION;														// Go to desorption
			}
		}
		break;


	/****************
	Inject more gas
	****************/
	case SUBSTEP_STATUS_INJECTION:
		if (storage.experimentStatus.isWaiting) break;
		LOG(logINFO) << stringh::string_format(MESSAGE_INJECTION_ATTEMPT, storage.experimentStatus.injectionAttemptCounter.load());							// Log current injection
		controls.valveControls.ValveOpen(ID_VALVE_4, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 1;
		break;

	case SUBSTEP_STATUS_INJECTION + 1:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveOpen(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 2;
		break;

	case SUBSTEP_STATUS_INJECTION + 2:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveClose(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 3;
		break;

	case SUBSTEP_STATUS_INJECTION + 3:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 4;
		break;

	case SUBSTEP_STATUS_INJECTION + 4:
		if (storage.experimentStatus.isWaiting) break;
		controls.valveControls.ValveClose(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;
		break;
		
	/****************
	Desorption step
	****************/
	case SUBSTEP_STATUS_DESORPTION:
		if (storage.experimentStatus.isWaiting) break;
		LOG(logINFO) << MESSAGE_DESORPTION_OPENV;

		// Open valve
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		// Wait for desorption
		WaitMinutes(storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].temps_adsorption);		// Set the time to wait
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;													// Go to next step
		break;
		
	/****************
	Final
	****************/
	case SUBSTEP_STATUS_END:
		if (storage.experimentStatus.isWaiting) break;
		// Display sample isolation message
		LOG(logINFO) << MESSAGE_DESORPTION_CLOSEV;

		// Close valves
		controls.valveControls.ValveClose(ID_VALVE_2, true);
		controls.valveControls.ValveClose(ID_VALVE_3, true);
		controls.valveControls.ValveClose(ID_VALVE_4, true);
		controls.valveControls.ValveClose(ID_VALVE_5, true);
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		controls.valveControls.ValveClose(ID_VALVE_8, true);

		// Display message
		LOG(logINFO) << stringh::string_format(MESSAGE_DESORPTION_DOSE_END,
			storage.experimentStatus.desorptionCounter.load(), storage.experimentStatus.injectionDose.load());

		// Increment dose
		++storage.experimentStatus.injectionDose;

		// Reset things
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
		break;

	default:
		break;
	}
}

