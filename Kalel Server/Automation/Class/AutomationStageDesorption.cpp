#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			DESORPTION
*
*
*/


void Automation::StageDesorption()
{
	switch (storage.experimentStatus.experimentStepStatus.get())
	{
	case STEP_STATUS_START:
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS;										// Set next step

		LOG(logINFO) << MESSAGE_DESORPTION_STAGE_START;																	// Let GUI know the step change

		controls.valveControls.CloseAll(true);																			// Close all valves
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the Desorption substeps
		SubstepsDesorption();

		// Check if the pressure for this desorption stage has been reached
		if (storage.experimentStatus.pressureFinal.get() < storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter.get()].pression_finale) {
			storage.experimentStatus.experimentStepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;														// Reset substep
		
		LOG(logINFO) << MESSAGE_DESORPTION_STAGE_END << storage.experimentStatus.desorptionCounter.get();						// Log the step change

		if (storage.experimentStatus.desorptionCounter.get() < storage.experimentSettings.dataDesorption.size())
		{
			++storage.experimentStatus.desorptionCounter;
		}
		else
		{
			storage.experimentStatus.experimentStage = STAGE_VACUUM_SAMPLE;													// Set sample vacuum if all desorption stages have been finished
		}
		break;
	}
}



void Automation::SubstepsDesorption()
{
	// Start desorption
	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_START &&
		storage.experimentStatus.experimentWaiting == false)
	{
		storage.experimentStatus.injectionAttemptCounter = 0;																							// Reset desorption attempt counter
		storage.experimentStatus.pressureInitial.set(storage.currentData.pressureHigh);																	// Set the initial pressure
		storage.experimentStatus.pressureHighOld.set(storage.currentData.pressureHigh);																	// Save the injection pressure for later
		
		LOG(logINFO) << MESSAGE_DESORPTION_DOSE_START << storage.experimentStatus.desorptionCounter.get() << storage.experimentStatus.experimentDose.get();			// Log about current dose
		
		// Turn on pump
		if (!controls.valveControls.PumpIsActive()) {
			controls.valveControls.EVActivate(EV_1, true);
			controls.valveControls.EVActivate(EV_2, true);
			controls.valveControls.PumpActivate(true);
			WaitSeconds(storage.machineSettings.TimeWaitPump);
		}
		WaitSeconds(storage.machineSettings.TimeWaitPump);
		
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;																		// Move to removal
	}


	// Removal of gas
	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_REMOVAL &&
		storage.experimentStatus.experimentWaiting == false)
	{
		LOG(logINFO) << MESSAGE_OUTGAS_ATTEMPT << storage.experimentStatus.injectionAttemptCounter.get();												// Tell GUI about current injection
		
		controls.valveControls.ValveOpen(VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL + 1;		
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_REMOVAL + 1 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_8, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL + 2;
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_REMOVAL + 2 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveOpen(VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL + 3;
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_REMOVAL + 3 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_7, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_CHECK;					// Move to removal check
	}


	// Check for removal
	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_CHECK &&
		storage.experimentStatus.experimentWaiting == false)
	{
		// Set the final pressure after gas removal
		storage.experimentStatus.pressureFinal.set(storage.currentData.pressureHigh);

		// Display
		LOG(logINFO) << MESSAGE_PRESSURE_D_PI << storage.experimentStatus.pressureInitial.get();
		LOG(logINFO) << MESSAGE_PRESSURE_D_PF << storage.experimentStatus.pressureFinal.get();
		LOG(logINFO) << MESSAGE_PRESSURE_D_DP << storage.experimentStatus.pressureFinal.get() - storage.experimentStatus.pressureInitial.get();
		LOG(logINFO) << MESSAGE_PRESSURE_D_DPREQ << storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter.get()].delta_pression;
		LOG(logINFO) << MESSAGE_OUTGAS_END << storage.experimentStatus.injectionAttemptCounter.get();

		// Checks for removal succeess, else increment the counter and try again
		if ((storage.experimentStatus.pressureHighOld.get() - storage.machineSettings.InjectionMargin < storage.currentData.pressureHigh) &&
			(storage.currentData.pressureHigh < storage.experimentStatus.pressureHighOld.get() + storage.machineSettings.InjectionMargin))
		{
			// If too many injections have been tried and failed
			if (storage.experimentStatus.injectionAttemptCounter.get() >= storage.machineSettings.InjectionAttemptNumber)
			{
				// Put the thread on stand-by
				eventPause = true;
				storage.automationControl.notify_all();

				// Tell GUI
				LOG(logINFO) << MESSAGE_OUTGAS_PROBLEM;
				LOG(logERROR) << MESSAGE_OUTGAS_PROBLEM_BOX;

				// Reset counter
				storage.experimentStatus.injectionAttemptCounter = 0;
				storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;
			}

			// If not, increment the counter and try again
			++storage.experimentStatus.injectionAttemptCounter;
			storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;
		}
		// If the removal succeeded
		else
		{	// Check if removal has overshot
			if (storage.experimentStatus.pressureInitial.get() - storage.experimentStatus.pressureFinal.get() > 
				storage.machineSettings.InjectionMultiplier * (storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter.get()].delta_pression))
			{
				storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT;						// Add gas
				LOG(logINFO) << (MESSAGE_INJECTION_ATTEMPT);
			}
			// If completeted successfully go to equilibration
			else
			{
				// Deactivate pump
				if (controls.valveControls.PumpIsActive()) {
					controls.valveControls.EVDeactivate(EV_1, true);
					controls.valveControls.EVDeactivate(EV_2, true);
					controls.valveControls.PumpDeactivate(true);
				}
				storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_DESORPTION;														// Go to desorption
				WaitSeconds(storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter.get()].temps_volume);					// Set the time to wait for equilibration in the reference volume
			}
		}
	}


	// Aborting the desorption step
	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_ABORT &&
		storage.experimentStatus.experimentWaiting == false)
	{
		if (storage.experimentStatus.pressureInitial.get() - storage.experimentStatus.pressureFinal.get() > 
			storage.machineSettings.InjectionMultiplier * (storage.experimentSettings.dataDesorption[storage.experimentStatus.desorptionCounter.get()].delta_pression))
		{
			// Add some gas
			controls.valveControls.ValveOpen(VALVE_2, true);
			WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
			storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 1;
		}
		else
		{
			LOG(logINFO) << (MESSAGE_INJECTION_END);
			storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;		// Go back to removal
		}
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 1 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_2, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 2;
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 2 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveOpen(VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 3;
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 3 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_3, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 4;
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 4 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveOpen(VALVE_4, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 5;
	}

	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 5 &&
		storage.experimentStatus.experimentWaiting == false)
	{
		controls.valveControls.ValveClose(VALVE_4, true);
		WaitSeconds(storage.machineSettings.TimeWaitValvesShort);

		storage.experimentStatus.pressureFinal.set(storage.currentData.pressureHigh);						// Save pressure after open/close
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_ABORT;								// Move back to the start
	}


	// Performing the desorption step
	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_DESORPTION &&
		storage.experimentStatus.experimentWaiting == false)
	{
		LOG(logINFO) << (MESSAGE_DESORPTION_OPENV);

		// Open valve
		controls.valveControls.ValveOpen(VALVE_5, true);

		// Wait for desorption
		WaitSeconds(storage.experimentSettings.dataAdsorption[storage.experimentStatus.adsorptionCounter.get()].temps_adsorption);		// Set the time to wait
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_END;													// Go to next step
	}


	// End desorption
	if (storage.experimentStatus.experimentSubstepStage == SUBSTEP_STATUS_END &&
		storage.experimentStatus.experimentWaiting == false)
	{
		// Display sample isolation message
		LOG(logINFO) << MESSAGE_DESORPTION_CLOSEV;

		// Close valve
		controls.valveControls.ValveClose(VALVE_5, true);

		// Display message
		LOG(logINFO) << MESSAGE_DESORPTION_DOSE_END << storage.experimentStatus.experimentDose.get();

		// Reset things
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}

