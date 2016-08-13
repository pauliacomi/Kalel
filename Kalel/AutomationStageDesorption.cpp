#include "stdafx.h"
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
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_STAGE_START);													// Let GUI know the step change

		ControlMechanismsCloseAll();																					// Close all valves
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the Desorption substeps
		SubstepsDesorption();

		// Check if the pressure for this desorption stage has been reached
		if (experimentLocalData.pressureFinal < experimentLocalSettings.dataDesorption[experimentLocalData.desorptionCounter].pression_finale) {
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}
		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Reset substep no matter what
		
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_STAGE_END, experimentLocalData.desorptionCounter);				// Let GUI know the step change

		if (experimentLocalData.desorptionCounter < experimentLocalSettings.dataDesorption.size())
		{
			experimentLocalData.desorptionCounter++;
		}
		else
		{
			experimentLocalData.experimentStage = STAGE_VACUUM_SAMPLE;													// Set sample vacuum if all desorption stages have been finished
		}
		break;
	}
}



void Automation::SubstepsDesorption()
{
	// Start desorption
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_START)
	{
		experimentLocalData.injectionAttemptCounter = 0;																							// Reset desorption attempt counter
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;																		// Set the initial pressure
		experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;																		// Save the injection pressure for later
		
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_DOSE_START, experimentLocalData.desorptionCounter, experimentLocalData.experimentDose);	// Tell GUI about current dose
		
		ActivatePump();
		WaitSeconds(TIME_WAIT_PUMP);
		
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INPROGRESS;																		// Move to injection
	}


	// Removal of gas
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INPROGRESS)
	{
		messageHandler.DisplayMessage(MESSAGE_OUTGAS_ATTEMPT, experimentLocalData.injectionAttemptCounter);				// Tell GUI about current injection
		
		ValveOpenClose(8);																								// Injection, should think about waiting within the valves without sleep
		ValveOpenClose(7);
		WaitSeconds(TIME_WAIT_VALVES);
		
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INPROGRESS + 1;										// Move to injection check
	}


	// Check for removal
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INPROGRESS + 1 &&
		experimentLocalData.experimentWaiting == false)
	{
		// Set the final pressure after gas removal
		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;

		// Display
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_PI, experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_PF, experimentLocalData.pressureFinal);
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_DP, experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_DPREQ, (experimentLocalSettings.dataDesorption[experimentLocalData.desorptionCounter].delta_pression));
		messageHandler.DisplayMessage(MESSAGE_OUTGAS_END, experimentLocalData.injectionAttemptCounter);

		// Checks for removal succeess, else increment the counter and try again
		if ((experimentLocalData.pressureHighOld - marge_injection < experimentLocalData.pressureHigh) &&
			(experimentLocalData.pressureHigh < experimentLocalData.pressureHighOld + marge_injection))
		{
			// If too many injections have been tried and failed
			if (experimentLocalData.injectionAttemptCounter >= nb_injection)
			{
				// Put the thread on stand-by
				::SetEvent(h_eventPause);

				// Tell GUI
				messageHandler.DisplayMessage(MESSAGE_OUTGAS_PROBLEM);
				messageHandler.DisplayMessageBox(MESSAGE_OUTGAS_PROBLEM_BOX, MB_ICONERROR | MB_OK, true);

				// Reset counter
				experimentLocalData.injectionAttemptCounter = 0;
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INPROGRESS;
			}

			// If not, increment the counter and try again
			experimentLocalData.injectionAttemptCounter++;
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INPROGRESS;
		}
		// If the removal succeeded
		else
		{	// Check if removal has overshot
			if (experimentLocalData.pressureInitial - experimentLocalData.pressureFinal > marge_multiplicateur * (experimentLocalSettings.dataDesorption[experimentLocalData.desorptionCounter].delta_pression))
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT;						// Add gas
				messageHandler.DisplayMessage(MESSAGE_INJECTION_ATTEMPT);
			}
			// If completeted successfully go to equilibration
			else
			{
				DeactivatePump();
				experimentLocalData.experimentSubstepStage = STEP_STATUS_INPROGRESS + 2;													// Go to desorption
				WaitSeconds(experimentLocalSettings.dataDesorption[experimentLocalData.desorptionCounter].temps_volume);					// Set the time to wait for equilibration in the reference volume
			}
		}
	}


	// Aborting the desorption step
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ABORT &&
		experimentLocalData.experimentWaiting == false)
	{
		if (experimentLocalData.pressureInitial - experimentLocalData.pressureFinal > marge_multiplicateur * (experimentLocalSettings.dataDesorption[experimentLocalData.desorptionCounter].delta_pression))
		{
			// Add some gas
			ValveOpenClose(3);
			ValveOpenClose(2);
			ValveOpenClose(4);
			WaitSeconds(TIME_WAIT_VALVES);

			experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;		// Save pressure after open/close
		}
		else
		{
			messageHandler.DisplayMessage(MESSAGE_INJECTION_END);
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INPROGRESS;		// Go back to injection
		}
	}


	// Performing the desorption step
	if (experimentLocalData.experimentSubstepStage == STEP_STATUS_INPROGRESS + 2 &&
		experimentLocalData.experimentWaiting == false)
	{
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_OPENV);

		// Open valve
		ValveOpen(5);

		// Wait for desorption
		WaitSeconds(experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_adsorption);		// Set the time to wait
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_END;													// Go to next step
	}


	// End desorption
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_END &&
		experimentLocalData.experimentWaiting == false)
	{
		// Display sample isolation message
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_CLOSEV);

		// Close valve
		ValveClose(5);

		// Display message
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_DOSE_END, experimentLocalData.experimentDose);

		// Reset things
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}

