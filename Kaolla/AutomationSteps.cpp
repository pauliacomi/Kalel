#include "stdafx.h"
#include "Automation.h"

void Automation::StepEquilibration()
{
	switch (experimentLocalData.stepStatus)
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
		if (timerMeasurement.TempsActuel() > experimentLocalData.dataDivers.temps_ligne_base)
			experimentLocalData.stepStatus = STEP_STATUS_END;
		
		// Wait
		g_flagAskShutdown = PAUSE;
		break;

	case STEP_STATUS_START:
		// Start the timer to record time of the baseline
		timerMeasurement.TopChrono();
		experimentLocalData.stepStatus = STEP_STATUS_INPROGRESS;
		// Let GUI know the step change
		messageHandler.DisplayMessage(message);
		break;

	case STEP_STATUS_END:
		// Change to the next step
		experimentLocalData.stepStatus = STEP_STATUS_START;
		// Let GUI know the step change
		messageHandler.DisplayMessage(message);
		SetNextStage();
		break;
	}
}

void Automation::StepAdsorption()
{
	switch (experimentLocalData.stepStatus)
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

		// Send the data to be saved outside of the function - ?
		messageHandler.ExchangeData();

		// Save the data to the file
		EnregistrementFichierMesures();

		// Increment the measurement number
		experimentLocalData.experimentMeasurement++;

		// Go through the adsorption substeps
		SubstepsAdsorption();

		// Check if the required pressure has been reached
		if (experimentLocalData.pressureFinal > experimentLocalData.dataAdsorption[experimentLocalData.adsorptionStage].pression_finale)
			experimentLocalData.stepStatus = STEP_STATUS_END;
		
		// Wait
		g_flagAskShutdown = PAUSE;
		break;

	case STEP_STATUS_START:

		experimentLocalData.stepStatus = STEP_STATUS_INPROGRESS;
		
		// Let GUI know the step change
		messageHandler.DisplayMessage(_T("Début de la petite dose %d\r\n"), dose);
		// Close all valves
		ControlMechanismsCloseAll();
		// Start threads and read the data
		ThreadMeasurement();
		// Set the initial pressure
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;

		break;

	case STEP_STATUS_END:
		// Change to the next step
		experimentLocalData.stepStatus = STEP_STATUS_START;
		// Let GUI know the step change
		messageHandler.DisplayStep(Etape);
		messageHandler.DisplayMessage(message);
		SetNextStage();
		break;
	}
}

void Automation::SubstepsAdsorption()
{
	switch (experimentLocalData.experimentSubstepStage)
	{
	case START:
		// Tell GUI about current dose
		messageHandler.DisplayMessage(_T("Début de la petite dose %d\r\n"), dose);

		break;
	case INJECTION:
		// Display step
		messageHandler.DisplayMessage(_T("Début Injection dans le volume de référence %d\r\n"), injection);
		messageHandler.DisplayStep(_T("Petites Doses %d : Injection dans le volume de référence %d"), dose, injection);
		
		//Injection
		ValveOpenClose(2);
		ValveOpenClose(3);
		ValveOpenClose(4);

		// Set the Final pressure
		experimentLocalData.pressureFinal = experimentLocalData.pressureHigh;
		
		// Display
		messageHandler.DisplayMessage(_T("Pression Initiale = %f\r\n"), PressionInitiale);
		messageHandler.DisplayMessage(_T("Pression Finale = %f\r\n"), PressionFinale);
		messageHandler.DisplayMessage(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
		messageHandler.DisplayMessage(_T("delta pression = %f\r\n"), (petites_doses.delta_pression));

		messageHandler.DisplayMessage(_T("Fin injection %d\r\n"), injection);

		// Checks for injection succeess
		if ((experimentLocalData.pressureHighOld - marge_injection < experimentLocalData.pressureHigh) &&
			(experimentLocalData.pressureHigh < experimentLocalData.pressureHighOld + marge_injection))
		{
			experimentLocalData.injectionCounter++;
		}
		if (experimentLocalData.injectionCounter >= nb_injection)
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
			experimentLocalData.injectionCounter = 0;
		}
		experimentLocalData.pressureHighOld = experimentLocalData.pressureHigh;

		// Check if injection has completed successfully
		if (PressionFinale - PressionInitiale > multiplicateur * (petites_doses.delta_pression))
		{
			experimentLocalData.experimentSubstepStage = INJECTION_ABORTED;
		}
		// If completeted successfully go to equilibration
		else 
		{
			experimentLocalData.experimentStage = STEP_EQUILIBRATION;
		}
		break;
	
	case INJECTION_ABORTED:

		break;

	case ADSORPTION:
		messageHandler.DisplayMessage(_T("Adsorption Basse Pression\r\n"));
		messageHandler.DisplayMessage(_T("Demande d'ouverture de la vanne 5 pour l'adsorption\r\n"));
		Ouverture_Vanne(5);

		nom_etape.Format(_T("Petites Doses %d : Adsorption Basse Pression"), dose);
		LireEcrireAfficher(petites_doses.temps_adsorption, nom_etape, pParam);
		messageHandler.DisplayMessage(_T("Fin Adsorption Basse Pression\r\n"));
		break;

	case FINISHED:

		// Set the initial pressure for the next dose
		experimentLocalData.pressureInitial = experimentLocalData.pressureHigh;

		messageHandler.DisplayMessage(_T("Isolation de l'échantillon\r\n"));
		Fermeture_Vanne(5); // ?

		messageHandler.DisplayMessage(_T("Fin de la petite dose %d\r\n"), dose);

		// Reset counter
		experimentLocalData.injectionCounter = 0;
		experimentLocalData.pressureHighOld = -1;

		break;

	default:
		break;
	}
}

void Automation::StepDesorption()
{
}

void Automation::SubstepsDesorption()
{
}

