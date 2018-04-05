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


void Automation::StageAdsorption()
{
	switch (storage.experimentStatus.stepStatus)
	{
	case STEP_STATUS_START:
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;														// Set next step

		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_STAGE_START, storage.experimentStatus.adsorptionCounter.load());	// Log the step change

		controls.valveControls.CloseAll(true);																						// Close all valves
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the adsorption substeps
		SubstepsAdsorption();

		// Check if the pressure for this adsorption stage has been reached
		if (storage.experimentStatus.pressureFinal > storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].pression_finale) {
			storage.experimentStatus.stepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		storage.experimentStatus.stepStatus = STEP_STATUS_START;															// Reset substep

		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_STAGE_END, storage.experimentStatus.adsorptionCounter.load());	// Log the step change

		if (storage.experimentStatus.adsorptionCounter < storage.experimentSettings.dataAdsorption.size())
		{
			++storage.experimentStatus.adsorptionCounter;
		}
		else
		{
			storage.experimentStatus.mainStage = STAGE_DESORPTION;															// Set desorption if all adsorption stages have been finished
		}
		break;
	}
}


void Automation::SubstepsAdsorption()
{

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_START &&
		storage.experimentStatus.isWaiting == false)
	{
		storage.experimentStatus.injectionAttemptCounter = 0;																								// Reset adsorption attempt counter
		storage.experimentStatus.pressureInitial = storage.currentData.pressureHigh.load();																	// Set the initial pressure
		storage.experimentStatus.pressureHighOld =  storage.currentData.pressureHigh.load();																// Save the injection pressure for later
		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_DOSE_START, 
			storage.experimentStatus.adsorptionCounter.load(), storage.experimentStatus.injectionDose.load());												// Log current dose
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;																			// Move to injection
	}
	

	// Injection
	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_INJECTION)
	{
		LOG(logINFO) << stringh::string_format(MESSAGE_INJECTION_ATTEMPT, storage.experimentStatus.injectionAttemptCounter.load());							// Log current injection
		controls.valveControls.ValveOpen(ID_VALVE_4, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 1;										
	}
	
	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_INJECTION + 1 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveOpen(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 2;
	}

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_INJECTION + 2 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 3;
	}

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_INJECTION + 3 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveOpen(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION + 4;
	}

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_INJECTION + 4 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_CHECK;								// Move to injection check
	}



	// Injection check
	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_CHECK &&
		storage.experimentStatus.isWaiting == false)
	{
		// Set the final pressure after injection
		storage.experimentStatus.pressureFinal = storage.currentData.pressureHigh.load();

		// Display
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_PI, storage.experimentStatus.pressureInitial.load());
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_PF, storage.experimentStatus.pressureFinal.load());
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_DP, storage.experimentStatus.pressureFinal - storage.experimentStatus.pressureInitial);
		LOG(logINFO) << stringh::string_format(MESSAGE_PRESSURE_D_DPREQ, storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].delta_pression);
		LOG(logINFO) << stringh::string_format(MESSAGE_INJECTION_END, storage.experimentStatus.injectionAttemptCounter.load());

		// Checks for injection succeess, else increment the injection counter and try again
		if ((storage.experimentStatus.pressureHighOld - storage.machineSettings.InjectionMargin < storage.currentData.pressureHigh) &&
			(storage.currentData.pressureHigh < storage.experimentStatus.pressureHighOld + storage.machineSettings.InjectionMargin))
		{
			// If too many injections have been tried and failed
			if (storage.experimentStatus.injectionAttemptCounter >= storage.machineSettings.InjectionAttemptNumber)
			{
				// Put the thread on stand-by
				eventPause = true;
				storage.automationControl.notify_all();

				// Tell GUI
				LOG(logINFO) << MESSAGE_INJECTION_PROBLEM;
				LOG_EVENT(qOK) << MESSAGE_INJECTION_PROBLEM_BOX;

				// Reset counter
				storage.experimentStatus.injectionAttemptCounter = 0;
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;
			}

			// If not, increment the counter and try again
			++storage.experimentStatus.injectionAttemptCounter;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;
		}
		// If the injection succeeded
		else
		{
			// Check if injection has overshot
			if (storage.experimentStatus.pressureFinal - storage.experimentStatus.pressureInitial > 
				storage.machineSettings.InjectionMultiplier * (storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].delta_pression))
			{
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ABORT;					// Remove gas
			}
			// If completeted successfully go to equilibration
			else
			{
				controls.valveControls.ValveClose(ID_VALVE_4, true);																			// Close valve 4
				WaitSeconds(storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].temps_volume);				// Set the time to wait for equilibration in the reference volume
				storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ADSORPTION;													// Go to adsorption
			}
		}
	}



	// Aborting
	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_ABORT &&
		storage.experimentStatus.isWaiting == false)
	{
		// Turn on pump
		if (!controls.valveControls.PumpIsActive()) {
			controls.valveControls.EVActivate(ID_EV_1, true);
			controls.valveControls.EVActivate(ID_EV_2, true);
			controls.valveControls.PumpActivate(true);
			WaitSeconds(storage.machineSettings.TimeWaitPump);
		}
		LOG(logINFO) << MESSAGE_OUTGAS_ATTEMPT;
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ABORT + 1;
	}

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_ABORT + 1 &&
		storage.experimentStatus.isWaiting == false)
	{
		if (storage.experimentStatus.pressureFinal - storage.experimentStatus.pressureInitial > 
			storage.machineSettings.InjectionMultiplier * (storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].delta_pression))
		{
			controls.valveControls.ValveOpen(ID_VALVE_8, true);
			WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ABORT + 2;
		}
		else
		{
			// Deactivate pump
			if (controls.valveControls.PumpIsActive()) {
				controls.valveControls.EVDeactivate(ID_EV_1, true);
				controls.valveControls.EVDeactivate(ID_EV_2, true);
				controls.valveControls.PumpDeactivate(true);
			}

			LOG(logINFO) << MESSAGE_OUTGAS_END;
			storage.experimentStatus.substepStatus = SUBSTEP_STATUS_INJECTION;				// Go back to injection
		}
	}


	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_ABORT + 2 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ABORT + 3;
	}

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_ABORT + 3 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveOpen(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ABORT + 4;
	}

	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_ABORT + 4 &&
		storage.experimentStatus.isWaiting == false)
	{
		controls.valveControls.ValveClose(ID_VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);

		storage.experimentStatus.pressureFinal = storage.currentData.pressureHigh.load();			// Save pressure after open/close
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_ABORT + 1;					// Go back to the start
	}



	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_ADSORPTION &&
		storage.experimentStatus.isWaiting == false)
	{
		LOG(logINFO) << MESSAGE_ADSORPTION_OPENV;

		// Open valve
		controls.valveControls.ValveOpen(ID_VALVE_5, true);

		// Wait for adsorption
		WaitSeconds(storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter].temps_adsorption);		// Set the time to wait
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_END;													// Go to next step
	}



	if (storage.experimentStatus.substepStatus == SUBSTEP_STATUS_END &&
		storage.experimentStatus.isWaiting == false)
	{
		// Display sample isolation message
		LOG(logINFO) << MESSAGE_ADSORPTION_CLOSEV;
		
		// Close valve
		controls.valveControls.ValveClose(ID_VALVE_5, true);

		// Display message to show end of adsorption
		LOG(logINFO) << stringh::string_format(MESSAGE_ADSORPTION_DOSE_END, storage.experimentStatus.injectionDose.load());
		// Increment dose
		++storage.experimentStatus.injectionDose;

		// Reset things
		storage.experimentStatus.substepStatus = SUBSTEP_STATUS_START;
	}
}



