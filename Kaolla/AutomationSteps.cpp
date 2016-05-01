#include "stdafx.h"
#include "Automation.h"

void Automation::StageEquilibration()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_INPROGRESS:
		// Should display time here

		// Start threads and read the data
		ThreadMeasurement();

		// Do the security checks
		SecuriteTemperatures();
		SecuriteHautePression();

		// Save the time at which the measurement took place
		experimentLocalData.experimentTime = timerExperiment.TempsActuel();

		// Send the data to be saved outside of the function
		messageHandler.ExchangeData();

		// Save the data to the file
		EnregistrementFichierMesures();

		// Increment the measurement number
		experimentLocalData.experimentMeasurement++;
		
		// Check if the time has been reached
		if (timerMeasurement.TempsActuel() > experimentLocalData.timeToEquilibrate)
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		
		// Wait
		g_flagAskShutdown = PAUSE;
		break;

	case STEP_STATUS_START:
		timerMeasurement.TopChrono();											// Start the timer to record time of the baseline
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;		// Set next step
		messageHandler.DisplayMessage(message);									// Let GUI know the step change
		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;			// Set next step
		SetNextStage();															// Set next stage
		messageHandler.DisplayMessage(message);									// Let GUI know the step change
		break;
	}
}

void Automation::StageAdsorption()
{
	switch (experimentLocalData.experimentStepStatus)
	{
	case STEP_STATUS_INPROGRESS:

		// Start the timer to record time of the measurement
		timerMeasurement.TopChrono();

		// Start threads and read the data
		ThreadMeasurement();

		// Do the security checks
		SecuriteTemperatures();
		SecuriteHautePression();

		// Save the time at which the measurement took place
		experimentLocalData.experimentTime = timerExperiment.TempsActuel();

		// Send the data to be saved outside of the function
		messageHandler.ExchangeData();

		// Save the data to the file
		EnregistrementFichierMesures();

		// Increment the measurement number
		experimentLocalData.experimentMeasurement++;

		// Go through the adsorption substeps
		SubstepsAdsorption();

		// Check if the pressure for this adsorption stage has been reached
		if (experimentLocalData.pressureFinal > experimentLocalData.dataAdsorption[experimentLocalData.experimentDose].pression_finale)
			experimentLocalData.experimentStepStatus = STEP_STATUS_END;
		
		// Wait
		g_flagAskShutdown = PAUSE;
		break;

	case STEP_STATUS_START:
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;												// Set next step
		messageHandler.DisplayMessage(_T("Start of adsorption stage %d \r\n"), experimentLocalData.experimentStage);	// Let GUI know the step change

		ControlMechanismsCloseAll();																					// Close all valves
		ThreadMeasurement();																							// Start threads and read the data
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;											// Set the initial pressure
		break;

	case STEP_STATUS_END:
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;													// Set next step
		SetNextStage();																									// Set next stage
		messageHandler.DisplayStep(Etape);																				// Let GUI know the substep change
		messageHandler.DisplayMessage(message);																			// Let GUI know the step change
		break;
	}
}

void Automation::SubstepsAdsorption()
{
	switch (experimentLocalData.experimentSubstepStage)
	{
	case SUBSTEP_STATUS_START:
		experimentLocalData.injectionAttemptCounter = 0;																// Reset counter
		messageHandler.DisplayMessage(_T("Début de la petite dose %d\r\n"), dose);										// Tell GUI about current dose
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION;											// Move to injection
		break;

	case SUBSTEP_STATUS_INJECTION:
		messageHandler.DisplayMessage(_T("Début Injection dans le volume de référence %d\r\n"), injection);				// Tell GUI about current injection
		messageHandler.DisplayStep(_T("Petites Doses %d : Injection dans le volume de référence %d"), dose, injection);		// Display step
		
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
		messageHandler.DisplayMessage(_T("Pression Initiale = %f\r\n"), experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(_T("Pression Finale = %f\r\n"), experimentLocalData.pressureFinal);
		messageHandler.DisplayMessage(_T("Pression Finale - Pression Initiale = %f\r\n"), experimentLocalData.pressureFinal - experimentLocalData.pressureInitial);
		messageHandler.DisplayMessage(_T("delta pression = %f\r\n"), (experimentLocalData.dataAdsorption[experimentLocalData.experimentDose].delta_pression));
		messageHandler.DisplayMessage(_T("Fin injection %d\r\n"), experimentLocalData.injectionAttemptCounter);

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
			messageHandler.DisplayMessage(_T("Problème d'injection - demande de pause\r\n"));
			CString message;
			message = _T("ATTENTION !!! problème de gaz !!!\t\n");
			message += _T("(gaz moteur ou gaz d'analyse)\t\n");
			message += _T("Expérience en pause !\r\n");
			message += _T("N'oubliez pas de 'reprendre' l'expérience !\t\n");
			AfxMessageBox(message, MB_ICONERROR | MB_OK);

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
			if (experimentLocalData.pressureFinal - experimentLocalData.pressureInitial > marge_multiplicateur * (experimentLocalData.dataAdsorption[experimentLocalData.experimentDose].delta_pression))
			{
				experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_INJECTION_ABORT;
			}
			// If completeted successfully go to equilibration
			else
			{
				experimentLocalData.timeToEquilibrate = experimentLocalData.dataAdsorption[experimentLocalData.experimentDose].temps_volume;		// Set the time to wait
				experimentLocalData.experimentStage = STAGE_EQUILIBRATION;																			// Go to waiting
			}
		}
		break;
	
	case SUBSTEP_STATUS_INJECTION_ABORT:

		break;

	case SUBSTEP_STATUS_ADSORPTION:
		messageHandler.DisplayMessage(_T("Adsorption Basse Pression\r\n"));
		messageHandler.DisplayMessage(_T("Demande d'ouverture de la vanne 5 pour l'adsorption\r\n"));
		nom_etape.Format(_T("Petites Doses %d : Adsorption Basse Pression"), dose);

		// Open valve
		Ouverture_Vanne(5);

		// Wait for adsorption
		experimentLocalData.timeToEquilibrate = experimentLocalData.dataAdsorption[experimentLocalData.experimentDose].temps_adsorption;		// Set the time to wait
		experimentLocalData.experimentStage = STAGE_EQUILIBRATION;																				// Go to waiting
		break;

	case SUBSTEP_STATUS_END:

		messageHandler.DisplayMessage(_T("Fin Adsorption Basse Pression\r\n"));
		// Set the initial pressure for the next dose
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;

		messageHandler.DisplayMessage(_T("Isolation de l'échantillon\r\n"));
		Fermeture_Vanne(5); // ?

		messageHandler.DisplayMessage(_T("Fin de la petite dose %d\r\n"), dose);

		// Reset things
		experimentLocalData.injectionCounter = 0;
		experimentLocalData.pressureHighOld = -1;
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

