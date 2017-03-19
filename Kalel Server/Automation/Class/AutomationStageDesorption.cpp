#include "Automation.h"

/*
*
*
*			DESORPTION
*
*
*/


void Automation::StageDesorption()
{
	switch (storage.currentData->experimentStepStatus)
	{
	case STEP_STATUS_START:
		storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		controls.messageHandler->DisplayMessage(MESSAGE_DESORPTION_STAGE_START);										// Let GUI know the step change

		controls.valveControls->CloseAll(true);																			// Close all valves
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the Desorption substeps
		SubstepsDesorption();

		// Check if the pressure for this desorption stage has been reached
		if (storage.currentData->pressureFinal < storage.experimentSettings->dataDesorption[storage.currentData->desorptionCounter].pression_finale) {
			storage.currentData->experimentStepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		storage.currentData->experimentStepStatus = STEP_STATUS_START;													// Reset substep no matter what
		
		controls.messageHandler->DisplayMessage(MESSAGE_DESORPTION_STAGE_END, storage.currentData->desorptionCounter);				// Let GUI know the step change

		if (storage.currentData->desorptionCounter < storage.experimentSettings->dataDesorption.size())
		{
			storage.currentData->desorptionCounter++;
		}
		else
		{
			storage.currentData->experimentStage = STAGE_VACUUM_SAMPLE;													// Set sample vacuum if all desorption stages have been finished
		}
		break;
	}
}



void Automation::SubstepsDesorption()
{
	// Start desorption
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_START &&
		storage.currentData->experimentWaiting == false)
	{
		storage.currentData->injectionAttemptCounter = 0;																							// Reset desorption attempt counter
		storage.currentData->SetpressureInitial (storage.currentData->GetpressureHigh());															// Set the initial pressure
		storage.currentData->SetpressureHighOld (storage.currentData->GetpressureHigh());															// Save the injection pressure for later
		
		controls.messageHandler->DisplayMessage(MESSAGE_DESORPTION_DOSE_START, storage.currentData->desorptionCounter, storage.currentData->experimentDose);	// Tell GUI about current dose
		
		// Turn on pump
		if (controls.valveControls->PumpIsInactive()) {
			controls.valveControls->EVActivate(1, true);
			controls.valveControls->EVActivate(2, true);
			controls.valveControls->PumpActivate(true);
			WaitSeconds(TIME_WAIT_PUMP);
		}
		WaitSeconds(TIME_WAIT_PUMP);
		
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;																		// Move to removal
	}


	// Removal of gas
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_REMOVAL &&
		storage.currentData->experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_OUTGAS_ATTEMPT, storage.currentData->injectionAttemptCounter);				// Tell GUI about current injection
		
		controls.valveControls->ValveOpen(8, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL + 1;		
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_REMOVAL + 1 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(8, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL + 2;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_REMOVAL + 2 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL + 3;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_REMOVAL + 3 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_CHECK;					// Move to removal check
	}


	// Check for removal
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_CHECK &&
		storage.currentData->experimentWaiting == false)
	{
		// Set the final pressure after gas removal
		storage.currentData->SetpressureFinal( storage.currentData->GetpressureHigh());

		// Display
		controls.messageHandler->DisplayMessage(MESSAGE_PRESSURE_D_PI, storage.currentData->pressureInitial);
		controls.messageHandler->DisplayMessage(MESSAGE_PRESSURE_D_PF, storage.currentData->pressureFinal);
		controls.messageHandler->DisplayMessage(MESSAGE_PRESSURE_D_DP, storage.currentData->pressureFinal - storage.currentData->pressureInitial);
		controls.messageHandler->DisplayMessage(MESSAGE_PRESSURE_D_DPREQ, (storage.experimentSettings->dataDesorption[storage.currentData->desorptionCounter].delta_pression));
		controls.messageHandler->DisplayMessage(MESSAGE_OUTGAS_END, storage.currentData->injectionAttemptCounter);

		// Checks for removal succeess, else increment the counter and try again
		if ((storage.currentData->pressureHighOld - marge_injection < storage.currentData->pressureHigh) &&
			(storage.currentData->pressureHigh < storage.currentData->pressureHighOld + marge_injection))
		{
			// If too many injections have been tried and failed
			if (storage.currentData->injectionAttemptCounter >= nb_injection)
			{
				// Put the thread on stand-by
				eventPause = true;
				storage.automationControl.notify_all();

				// Tell GUI
				controls.messageHandler->DisplayMessage(MESSAGE_OUTGAS_PROBLEM);
				controls.messageHandler->DisplayMessageBox(MESSAGE_OUTGAS_PROBLEM_BOX, MB_ICONERROR | MB_OK, true);

				// Reset counter
				storage.currentData->injectionAttemptCounter = 0;
				storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;
			}

			// If not, increment the counter and try again
			storage.currentData->injectionAttemptCounter++;
			storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;
		}
		// If the removal succeeded
		else
		{	// Check if removal has overshot
			if (storage.currentData->pressureInitial - storage.currentData->pressureFinal > marge_multiplicateur * (storage.experimentSettings->dataDesorption[storage.currentData->desorptionCounter].delta_pression))
			{
				storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT;						// Add gas
				controls.messageHandler->DisplayMessage(MESSAGE_INJECTION_ATTEMPT);
			}
			// If completeted successfully go to equilibration
			else
			{
				// Deactivate pump
				if (controls.valveControls->PumpIsActive()) {
					controls.valveControls->EVDeactivate(1, true);
					controls.valveControls->EVDeactivate(2, true);
					controls.valveControls->PumpDeactivate(true);
				}
				storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_DESORPTION;														// Go to desorption
				WaitSeconds(storage.experimentSettings->dataDesorption[storage.currentData->desorptionCounter].temps_volume);					// Set the time to wait for equilibration in the reference volume
			}
		}
	}


	// Aborting the desorption step
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT &&
		storage.currentData->experimentWaiting == false)
	{
		if (storage.currentData->pressureInitial - storage.currentData->pressureFinal > marge_multiplicateur * (storage.experimentSettings->dataDesorption[storage.currentData->desorptionCounter].delta_pression))
		{
			// Add some gas
			controls.valveControls->ValveOpen(2, true);
			WaitSeconds(TIME_WAIT_VALVES_SHORT);
			storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 1;
		}
		else
		{
			controls.messageHandler->DisplayMessage(MESSAGE_INJECTION_END);
			storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_REMOVAL;		// Go back to removal
		}
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 1 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(2, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 2;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 2 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(3, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 3;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 3 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(3, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 4;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 4 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(4, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 5;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 5 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(4, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);

		storage.currentData->SetpressureFinal( storage.currentData->GetpressureHigh());					// Save pressure after open/close
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT;								// Move back to the start
	}


	// Performing the desorption step
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_DESORPTION &&
		storage.currentData->experimentWaiting == false)
	{
		controls.messageHandler->DisplayMessage(MESSAGE_DESORPTION_OPENV);

		// Open valve
		controls.valveControls->ValveOpen(5, true);

		// Wait for desorption
		WaitSeconds(storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].temps_adsorption);		// Set the time to wait
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_END;													// Go to next step
	}


	// End desorption
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_END &&
		storage.currentData->experimentWaiting == false)
	{
		// Display sample isolation message
		controls.messageHandler->DisplayMessage(MESSAGE_DESORPTION_CLOSEV);

		// Close valve
		controls.valveControls->ValveClose(5, true);

		// Display message
		controls.messageHandler->DisplayMessage(MESSAGE_DESORPTION_DOSE_END, storage.currentData->experimentDose);

		// Reset things
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}

