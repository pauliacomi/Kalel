#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			ADSORPTION
*
*
*/


void Automation::StageAdsorption()
{
	switch (storage.currentData->experimentStepStatus)
	{
	case STEP_STATUS_START:
		storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step

		LOG(logINFO) << MESSAGE_ADSORPTION_STAGE_START;																	// Log the step change

		controls.valveControls->CloseAll(true);																			// Close all valves
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the adsorption substeps
		SubstepsAdsorption();

		// Check if the pressure for this adsorption stage has been reached
		if (storage.currentData->pressureFinal > storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].pression_finale) {
			storage.currentData->experimentStepStatus = STEP_STATUS_END;
		}

		break;

	case STEP_STATUS_END:
		storage.currentData->experimentStepStatus = STEP_STATUS_START;													// Reset substep no matter what

		LOG(logINFO) << MESSAGE_ADSORPTION_STAGE_START << storage.currentData->adsorptionCounter;						// Log the step change

		if (storage.currentData->adsorptionCounter < storage.experimentSettings->dataAdsorption.size())
		{
			storage.currentData->adsorptionCounter++;
		}
		else
		{
			storage.currentData->experimentStage = STAGE_DESORPTION;														// Set desorption if all adsorption stages have been finished
		}
		break;
	}
}


void Automation::SubstepsAdsorption()
{

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_START &&
		storage.currentData->experimentWaiting == false)
	{
		storage.currentData->injectionAttemptCounter = 0;																							// Reset adsorption attempt counter
		storage.currentData->SetpressureInitial( storage.currentData->GetpressureHigh());															// Set the initial pressure
		storage.currentData->SetpressureHighOld( storage.currentData->GetpressureHigh());															// Save the injection pressure for later
		LOG(logINFO) << MESSAGE_ADSORPTION_DOSE_START << storage.currentData->adsorptionCounter << storage.currentData->experimentDose;				// Log current dose
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION;																		// Move to injection
	}
	

	// Injection
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_INJECTION)
	{
		LOG(logINFO) << MESSAGE_INJECTION_ATTEMPT << storage.currentData->injectionAttemptCounter;													// Log current injection
		controls.valveControls->ValveOpen(2, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);																			
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 1;										
	}
	
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 1 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(2, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 2;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 2 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(3, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 3;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 3 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(3, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 4;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 4 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(4, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 5;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 5 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(4, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_CHECK;								// Move to injection check
	}



	// Injection check
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_CHECK &&
		storage.currentData->experimentWaiting == false)
	{
		// Set the final pressure after injection
		storage.currentData->SetpressureFinal( storage.currentData->GetpressureHigh());

		// Display
		LOG(logINFO) << MESSAGE_PRESSURE_D_PI << storage.currentData->pressureInitial;
		LOG(logINFO) << MESSAGE_PRESSURE_D_PF << storage.currentData->pressureFinal;
		LOG(logINFO) << MESSAGE_PRESSURE_D_DP << storage.currentData->pressureFinal - storage.currentData->pressureInitial;
		LOG(logINFO) << MESSAGE_PRESSURE_D_DPREQ << storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].delta_pression;
		LOG(logINFO) << MESSAGE_INJECTION_END << storage.currentData->injectionAttemptCounter;

		// Checks for injection succeess, else increment the injection counter and try again
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
				LOG(logINFO) << MESSAGE_INJECTION_PROBLEM;
				LOG(logERROR) << MESSAGE_INJECTION_PROBLEM_BOX;

				// Reset counter
				storage.currentData->injectionAttemptCounter = 0;
				storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
			}

			// If not, increment the counter and try again
			storage.currentData->injectionAttemptCounter++;
			storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
		}
		// If the injection succeeded
		else
		{
			// Check if injection has overshot
			if (storage.currentData->pressureFinal - storage.currentData->pressureInitial > marge_multiplicateur * (storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].delta_pression))
			{
				storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT;					// Remove gas
			}
			// If completeted successfully go to equilibration
			else
			{
				storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ADSORPTION;													// Go to adsorption
				WaitSeconds(storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].temps_volume);				// Set the time to wait for equilibration in the reference volume
			}
		}
	}



	// Aborting
	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT &&
		storage.currentData->experimentWaiting == false)
	{
		// Turn on pump
		if (!controls.valveControls->PumpIsActive()) {
			controls.valveControls->EVActivate(1, true);
			controls.valveControls->EVActivate(2, true);
			controls.valveControls->PumpActivate(true);
			WaitSeconds(TIME_WAIT_PUMP);
		}
		LOG(logINFO) << MESSAGE_OUTGAS_ATTEMPT;
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 1;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 1 &&
		storage.currentData->experimentWaiting == false)
	{
		if (storage.currentData->pressureFinal - storage.currentData->pressureInitial > marge_multiplicateur * (storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].delta_pression))
		{
			controls.valveControls->ValveOpen(8, true);
			WaitSeconds(TIME_WAIT_VALVES_SHORT);
			storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 2;
		}
		else
		{
			// Deactivate pump
			if (controls.valveControls->PumpIsActive()) {
				controls.valveControls->EVDeactivate(1, true);
				controls.valveControls->EVDeactivate(2, true);
				controls.valveControls->PumpDeactivate(true);
			}

			LOG(logINFO) << MESSAGE_OUTGAS_END;
			storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_INJECTION;				// Go back to injection
		}
	}


	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 2 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(8, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 3;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 3 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveOpen(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 4;
	}

	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ABORT + 4 &&
		storage.currentData->experimentWaiting == false)
	{
		controls.valveControls->ValveClose(7, true);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);

		storage.currentData->SetpressureFinal( storage.currentData->GetpressureHigh());					// Save pressure after open/close
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_ABORT + 1;					// Go back to the start
	}



	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_ADSORPTION &&
		storage.currentData->experimentWaiting == false)
	{
		LOG(logINFO) << MESSAGE_ADSORPTION_OPENV;

		// Open valve
		controls.valveControls->ValveOpen(5, true);

		// Wait for adsorption
		WaitSeconds(storage.experimentSettings->dataAdsorption[storage.currentData->adsorptionCounter].temps_adsorption);		// Set the time to wait
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_END;													// Go to next step
	}



	if (storage.currentData->experimentSubstepStage == SUBSTEP_STATUS_END &&
		storage.currentData->experimentWaiting == false)
	{
		// Display sample isolation message
		LOG(logINFO) << MESSAGE_ADSORPTION_CLOSEV;
		
		// Close valve
		controls.valveControls->ValveClose(5, true);

		// Display message to show end of adsorption
		LOG(logINFO) << MESSAGE_ADSORPTION_DOSE_END << storage.currentData->experimentDose;

		// Reset things
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}



