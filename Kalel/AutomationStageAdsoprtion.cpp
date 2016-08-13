#include "stdafx.h"
#include "Automation.h"

/*
*
*
*			ADSORPTION
*
*
*/


void Automation::StageAdsorption()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_STAGE_START);													// Let GUI know the step change

		ControlMechanismsCloseAll();																					// Close all valves
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the adsorption substeps
		SubstepsAdsorption();

		// Check if the pressure for this adsorption stage has been reached
		if (experimentLocalData.pressureFinal > experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].pression_finale) {
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		}

		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Reset substep no matter what
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_STAGE_END, experimentLocalData.adsorptionCounter);				// Let GUI know the step change

		if (experimentLocalData.adsorptionCounter < experimentLocalSettings.dataAdsorption.size())
		{
			experimentLocalData.adsorptionCounter++;
		}
		else
		{
			experimentLocalData.experimentStage = STAGE_DESORPTION;														// Set desorption if all adsorption stages have been finished
		}
		break;
	}
}


void Automation::SubstepsAdsorption()
{

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_START &&
		experimentLocalData.experimentWaiting == false)
	{
		experimentLocalData.injectionAttemptCounter = 0;																							// Reset adsorption attempt counter
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;																		// Set the initial pressure
		experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;																		// Save the injection pressure for later
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_START, experimentLocalData.adsorptionCounter, experimentLocalData.experimentDose);	// Tell GUI about current dose
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;																		// Move to injection
	}
	

	// Injection
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION)
	{
		messageHandler.DisplayMessage(MESSAGE_INJECTION_ATTEMPT, experimentLocalData.injectionAttemptCounter);			// Tell GUI about current injection
		ValveOpen(2);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);																			
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 1;										
	}
	
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 1 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(2);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 2;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 2 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveOpen(3);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 3;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 3 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(3);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 4;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 4 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveOpen(4);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION + 5;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION + 5 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(4);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_CHECK;								// Move to injection check
	}



	// Injection check
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_CHECK &&
		experimentLocalData.experimentWaiting == false)
	{
		// Set the final pressure after injection
		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;

		// Display
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_PI, experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_PF, experimentLocalData.pressureFinal);
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_DP, experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_PRESSURE_D_DPREQ, (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression));
		messageHandler.DisplayMessage(MESSAGE_INJECTION_END, experimentLocalData.injectionAttemptCounter);

		// Checks for injection succeess, else increment the injection counter and try again
		if ((experimentLocalData.pressureHighOld - marge_injection < experimentLocalData.pressureHigh) &&
			(experimentLocalData.pressureHigh < experimentLocalData.pressureHighOld + marge_injection))
		{
			// If too many injections have been tried and failed
			if (experimentLocalData.injectionAttemptCounter >= nb_injection)
			{
				// Put the thread on stand-by
				::SetEvent(h_eventPause);

				// Tell GUI
				messageHandler.DisplayMessage(MESSAGE_INJECTION_PROBLEM);
				messageHandler.DisplayMessageBox(MESSAGE_INJECTION_PROBLEM_BOX, MB_ICONERROR | MB_OK, true);

				// Reset counter
				experimentLocalData.injectionAttemptCounter = 0;
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
			}

			// If not, increment the counter and try again
			experimentLocalData.injectionAttemptCounter++;
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
		}
		// If the injection succeeded
		else
		{
			// Check if injection has overshot
			if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT;					// Remove gas
			}
			// If completeted successfully go to equilibration
			else
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ADSORPTION;													// Go to adsorption
				WaitSeconds(experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_volume);				// Set the time to wait for equilibration in the reference volume
			}
		}
	}



	// Aborting
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ABORT &&
		experimentLocalData.experimentWaiting == false)
	{
		// Turn on pump
		if (!PompeEstActive()) {
			EVActivate(1);
			EVActivate(2);
			ActivatePump();
			WaitSeconds(TIME_WAIT_PUMP);
		}
		messageHandler.DisplayMessage(MESSAGE_OUTGAS_ATTEMPT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 1;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 1 &&
		experimentLocalData.experimentWaiting == false)
	{
		if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
		{
			ValveOpen(8);
			WaitSeconds(TIME_WAIT_VALVES_SHORT);
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 2;
		}
		else
		{
			// Deactivate pump
			if (PompeEstActive()) {
				EVDeactivate(1);
				EVDeactivate(2);
				DeactivatePump();
			}

			messageHandler.DisplayMessage(MESSAGE_OUTGAS_END);
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;				// Go back to injection
		}
	}


	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 2 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(8);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 3;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 3 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveOpen(7);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 4;
	}

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ABORT + 4 &&
		experimentLocalData.experimentWaiting == false)
	{
		ValveClose(7);
		WaitSeconds(TIME_WAIT_VALVES_SHORT);

		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;					// Save pressure after open/close
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ABORT + 1;					// Go back to the start
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_ADSORPTION &&
		experimentLocalData.experimentWaiting == false)
	{
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_OPENV);

		// Open valve
		ValveOpen(5);

		// Wait for adsorption
		WaitSeconds(experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_adsorption);		// Set the time to wait
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_END;													// Go to next step
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_END &&
		experimentLocalData.experimentWaiting == false)
	{
		// Display sample isolation message
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_CLOSEV);
		
		// Close valve
		ValveClose(5);

		// Display message to show end of adsorption
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_END, experimentLocalData.experimentDose);

		// Reset things
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}



