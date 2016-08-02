#include "stdafx.h"
#include "Automation.h"


/*
*
*
*			EQUILIBRATION
*
*
*/

void Automation::StageEquilibration()
{
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_START) {

		experimentLocalData.experimentStepStatus = STEP_STATUS_END;											// Set next step
		messageHandler.DisplayMessage(MESSAGE_EQUILIBRATION_STARTED);										// Let GUI know the step change

		// This is where we start recording

		// Record start
		experimentLocalData.experimentInProgress = true;
		experimentLocalData.experimentRecording = true;

		// Create, open and write the columns in the:
		EnteteCreate();				// Entete TXT
		EnteteCSVCreate();			// Entete CSV
		FileMeasurementOpen();		// Measurement file

		timerExperiment.TopChrono();	// Start global experiment timer	
		timerMeasurement.TopChrono();	// Start the timer to record time between measurements


		// Set the time to wait
		//WaitSeconds(experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataDivers.temps_ligne_base * 60);
		WaitMinutes(experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataDivers.temps_ligne_base);
	}

	if (experimentLocalData.experimentStepStatus == STEP_STATUS_END) {

		if (experimentLocalData.experimentWaiting == false) {
			experimentLocalData.experimentStage = STAGE_ADSORPTION;												// Set next stage
			experimentLocalData.experimentStepStatus = STEP_STATUS_START;										// Reset next step
			messageHandler.DisplayMessage(MESSAGE_EQUILIBRATION_COMPLETE);										// Let GUI know the step change
		}
	}
}



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
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;											// Set the initial pressure
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

	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_START)
	{
		experimentLocalData.injectionAttemptCounter = 0;																							// Reset adsorption attempt counter
		experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;																		// Save the injection pressure for later
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_START, experimentLocalData.adsorptionCounter, experimentLocalData.experimentDose);	// Tell GUI about current dose
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;																		// Move to injection
	}
	


	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION)
	{
		messageHandler.DisplayMessage(MESSAGE_INJECTION_ATTEMPT, experimentLocalData.injectionAttemptCounter);			// Tell GUI about current injection
		ValveOpenClose(2);																								// Injection
		ValveOpenClose(3);
		ValveOpenClose(4);
		WaitSeconds(30);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_CHK;										// Move to injection check
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION_CHK &&
		experimentLocalData.experimentWaiting == false)
	{
		// Set the final pressure after injection
		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;

		// Display
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PI, experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PF, experimentLocalData.pressureFinal);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DP, experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DPREQ, (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression));
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
			}

			// If not, increment the counter and try again
			experimentLocalData.injectionAttemptCounter++;
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
		}
		// If the injection succeeded
		else
		{
			experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;					// Save the injection pressure for next injection
			experimentLocalData.injectionAttemptCounter = 0;										// Reset counter

			// Check if injection has overshot
			if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_ABORT;		// Remove gas
			}
			// If completeted successfully go to equilibration
			else
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ADSORPTION;													// Go to adsorption
				WaitSeconds(experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_volume);				// Set the time to wait for equilibration in the reference volume
			}
		}
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION_ABORT &&
		experimentLocalData.experimentWaiting == false)
	{
		if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
		{
			// Turn on pump
			if (!PompeEstActive()) {
				EVActivate(1);
				EVActivate(2);
				ActivatePump();
				messageHandler.DisplayMessage(MESSAGE_OUTGAS_START);
			}

			ValveOpenClose(8);
			ValveOpenClose(7);

			experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;		// Save pressure after open/close
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
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;		// Go back to injection
		}
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
		// Set the initial pressure for the next dose
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;

		// Close valve
		ValveClose(5);

		// Display message
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_END, experimentLocalData.experimentDose);

		// Reset things
		experimentLocalData.pressureHighOld = -1;
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}



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
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;											// Set the initial pressure
		break;

	case STEP_STATUS_INPROGRESS:

		// Go through the Desorption substeps
		SubstepsDesorption();

		// Check if the pressure for this adsorption stage has been reached
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
			experimentLocalData.experimentStage = STAGE_UNDER_VACUUM;														// Set desorption if all adsorption stages have been finished
		}
		break;
	}
}



void Automation::SubstepsDesorption()
{
	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_START)
	{
		experimentLocalData.injectionAttemptCounter = 0;																							// Reset adsorption attempt counter
		experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;																		// Save the injection pressure for later
		messageHandler.DisplayMessage(MESSAGE_DESORPTION_DOSE_START, experimentLocalData.adsorptionCounter, experimentLocalData.experimentDose);	// Tell GUI about current dose
		ActivatePump();
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;																		// Move to injection
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION)
	{
		messageHandler.DisplayMessage(MESSAGE_INJECTION_ATTEMPT, experimentLocalData.injectionAttemptCounter);			// Tell GUI about current injection
		ValveOpenClose(8);																								// Injection, should think about waiting within the valves without sleep
		ValveOpenClose(7);
		WaitSeconds(2);
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_CHK;										// Move to injection check
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION_CHK &&
		experimentLocalData.experimentWaiting == false)
	{
		// Set the final pressure after injection
		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;

		// Display
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PI, experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PF, experimentLocalData.pressureFinal);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DP, experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DPREQ, (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression));
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
			}

			// If not, increment the counter and try again
			experimentLocalData.injectionAttemptCounter++;
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
		}
		// If the injection succeeded
		else
		{
			experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;					// Save the injection pressure for next injection
			experimentLocalData.injectionAttemptCounter = 0;										// Reset counter

																									// Check if injection has overshot
			if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_ABORT;		// Remove gas
			}
			// If completeted successfully go to equilibration
			else
			{
				DeactivatePump();
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_ADSORPTION;													// Go to adsorption
				WaitSeconds(experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_volume);				// Set the time to wait for equilibration in the reference volume
			}
		}
	}



	if (experimentLocalData.experimentSubstepStage == SUBSTEP_STATUS_INJECTION_ABORT &&
		experimentLocalData.experimentWaiting == false)
	{
		if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
		{
			// Turn on pump
			if (!PompeEstActive()) {
				EVActivate(1);
				EVActivate(2);
				ActivatePump();
				messageHandler.DisplayMessage(MESSAGE_OUTGAS_START);
			}

			ValveOpenClose(8);
			ValveOpenClose(7);

			experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;		// Save pressure after open/close
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
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;		// Go back to injection
		}
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
		// Set the initial pressure for the next dose
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;

		// Close valve
		ValveClose(5);

		// Display message
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_END, experimentLocalData.experimentDose);

		// Reset things
		experimentLocalData.pressureHighOld = -1;
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
	}
}

