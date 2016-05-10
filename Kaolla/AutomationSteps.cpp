#include "stdafx.h"
#include "Automation.h"

void Automation::StageEquilibration()
{
	switch (experimentLocalData.experimentStepStatus)
	{

	case STEP_STATUS_START:
		experimentLocalData.experimentEquilibrationStatus = STEP_STATUS_END;								// Set next step
		messageHandler.DisplayMessage(MESSAGE_EQUILIBRATION_STARTED);										// Let GUI know the step change
		
		// Set the time to wait
		experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataDivers.temps_ligne_base * 60;
		experimentLocalData.experimentWaiting = true;
		experimentLocalData.experimentCommandsRequested = false;
		timerWaiting.TopChrono();

		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStage = STAGE_ADSORPTION;												// Set next stage
		experimentLocalData.experimentEquilibrationStatus = STEP_STATUS_START;								// Reset next step
		messageHandler.DisplayMessage(MESSAGE_EQUILIBRATION_COMPLETE);										// Let GUI know the step change
		break;
	}
}

void Automation::StageAdsorption()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_STAGE_START, experimentLocalData.experimentStage);				// Let GUI know the step change

		ControlMechanismsCloseAll();																					// Close all valves
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;											// Set the initial pressure
		break;

	case STEP_STATUS_INPROGRESS:

		// Check if the pressure for this adsorption stage has been reached
		if (experimentLocalData.pressureFinal > experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].pression_finale)
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;

		// Go through the adsorption substeps
		SubstepsAdsorption();
		
		// Set the time to wait
		experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_adsorption;
		experimentLocalData.experimentWaiting = true;
		experimentLocalData.experimentCommandsRequested = false;
		timerWaiting.TopChrono();

		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Set next substep
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_STAGE_END, experimentLocalData.adsorptionCounter);				// Let GUI know the step change

		if (experimentLocalData.adsorptionCounter < experimentLocalSettings.dataAdsorption.size())
		{
			experimentLocalData.adsorptionCounter++;
		}
		else
		{
			experimentLocalData.experimentStage = STAGE_DESORPTION;														// Set next stage
		}
		break;
	}
}

void Automation::SubstepsAdsorption()
{
	switch (experimentLocalData.experimentSubstepStage)
	{
	case SUBSTEP_STATUS_START:
		experimentLocalData.injectionAttemptCounter = 0;																// Reset counter
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_START, experimentLocalData.experimentDose);				// Tell GUI about current dose
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;											// Move to injection
		break;

	case SUBSTEP_STATUS_INJECTION:
		messageHandler.DisplayMessage(MESSAGE_INJECTION_ATTEMPT, experimentLocalData.injectionAttemptCounter);				// Tell GUI about current injection
		
		//Injection
		ValveOpenClose(2);
		ValveOpenClose(3);
		ValveOpenClose(4);

		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_CHK;										// Move to injection check
		break;

	case SUBSTEP_STATUS_INJECTION_CHK:
		// Set the final pressure
		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;
	
		// Display
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PI, experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PF, experimentLocalData.pressureFinal);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DP, experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DPREQ, (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression));
		messageHandler.DisplayMessage(MESSAGE_INJECTION_END, experimentLocalData.injectionAttemptCounter);

		// Checks for injection succeess, else increment the injection counter
		if ((experimentLocalData.pressureHighOld - marge_injection < experimentLocalData.pressureHigh) &&
			(experimentLocalData.pressureHigh < experimentLocalData.pressureHighOld + marge_injection))
		{
			experimentLocalData.injectionAttemptCounter ++;
		}

		// If too many injections have been tried and failed
		if (experimentLocalData.injectionAttemptCounter >= nb_injection)
		{
			// Put the thread on stand-by
			experimentLocalData.experimentStage = INACTIVE;

			// Tell GUI
			messageHandler.DisplayMessage(MESSAGE_INJECTION_PROBLEM);
			messageHandler.DisplayMessageBox(MESSAGE_INJECTION_PROBLEM_BOX, MB_ICONERROR | MB_OK , true);

			// Reset counter
			experimentLocalData.injectionAttemptCounter = 0;

			// Try injection again
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
		}
		// If the injection succeeded
		else
		{
			experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;

			// Check if injection has overshot
			if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_ABORT;
			}
			// If completeted successfully go to equilibration
			else
			{
				experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_volume;		// Set the time to wait
				experimentLocalData.experimentStage = STAGE_EQUILIBRATION;																			// Go to equilibration stage
				experimentLocalData.experimentEquilibrationStatus = STEP_STATUS_START;																// Also reset substage
			}
		}
		break;
	
	case SUBSTEP_STATUS_INJECTION_ABORT:
		if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression))
		{
			if (!PompeEstActive()) {
				EVActivate(1);
				EVActivate(2);
				ActivatePump();
			}

			messageHandler.DisplayMessage(MESSAGE_OUTGAS_START, experimentLocalData.injectionAttemptCounter);

			// Display
			messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PI, experimentLocalData.pressureInitial);
			messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_PF, experimentLocalData.pressureFinal);
			messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DP, experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
			messageHandler.DisplayMessage(MESSAGE_ADSORPTION_D_DPREQ, (experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].delta_pression));
			messageHandler.DisplayMessage(MESSAGE_INJECTION_END, experimentLocalData.injectionAttemptCounter);

			ValveOpenClose(8);
			ValveOpenClose(7);

		}
		else
		{
			if (PompeEstActive()) {
				EVDeactivate(1);
				EVDeactivate(2);
				DeactivatePump();
			}

			messageHandler.DisplayMessage(MESSAGE_OUTGAS_END, experimentLocalData.injectionAttemptCounter);
			experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;
		}
		break;

	case SUBSTEP_STATUS_ADSORPTION:
		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_OPENV);

		// Open valve
		ValveOpen(5);

		// Wait for adsorption
		experimentLocalData.timeToEquilibrate = experimentLocalSettings.dataAdsorption[experimentLocalData.adsorptionCounter].temps_adsorption;		// Set the time to wait
		experimentLocalData.experimentStage = STAGE_EQUILIBRATION;																				// Go to equilibration stage
		experimentLocalData.experimentEquilibrationStatus = STEP_STATUS_START;																	// Also reset substage
		break;

	case SUBSTEP_STATUS_END:

		// Set the initial pressure for the next dose
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;

		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_CLOSEV);
		
		// Close valve
		ValveClose(5);

		messageHandler.DisplayMessage(MESSAGE_ADSORPTION_DOSE_END, experimentLocalData.experimentDose);

		// Reset things
		experimentLocalData.injectionAttemptCounter = 0;
		experimentLocalData.pressureHighOld = -1;
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
		break;

	default:
		ASSERT(0);
		break;
	}
}

void Automation::StageDesorption()
{
}

void Automation::SubstepsDesorption()
{
}

