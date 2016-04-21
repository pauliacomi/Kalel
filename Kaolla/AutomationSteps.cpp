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

		// Send the data to be saved outside of the function - ?
		messageHandler.ExchangeData();

		// Save the data to the file
		EnregistrementFichierMesures();

		// Increment the measurement number
		experimentLocalData.experimentMeasurement++;
		
		// Check if the time has been reached
		if (timerMeasurement.TempsActuel > experimentLocalData.dataDivers.temps_ligne_base)
			experimentLocalData.stepStatus = STEP_STATUS_END;
		g_flagAskShutdown = PAUSE;
		break;

	case STEP_STATUS_START:
		// Start the timer to record time of the baseline
		timerMeasurement.TopChrono();
		experimentLocalData.stepStatus = STEP_STATUS_INPROGRESS;
		// Let GUI know the step change
		messageHandler.DisplayStep(Etape);
		messageHandler.DisplayMessage(message);
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

void Automation::StepAdsorption()
{
	switch (experimentLocalData.stepStatus)
	{
	case STEP_STATUS_INPROGRESS:
		// Tell GUI about current dose
		messageHandler.DisplayMessage(_T("D�but de la petite dose %d\r\n"), dose);

		// Start threads and read the data
		ThreadMeasurement();

		SubstepsAdsorption();

		// increment injection var
		// ?

		// Previous injection  = -1 
		// injection counter =  0

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

		// Check if the required pressure has been reached
		if (experimentLocalData.pressureFinal > experimentLocalData.dataAdsorption[experimentLocalData.adsorptionStage].pression_finale)
			experimentLocalData.stepStatus = STEP_STATUS_END;
		g_flagAskShutdown = PAUSE;
		break;

	case STEP_STATUS_START:
		// Start the timer to record time of the baseline
		timerMeasurement.TopChrono();
		experimentLocalData.stepStatus = STEP_STATUS_INPROGRESS;
		// Let GUI know the step change
		messageHandler.DisplayStep(Etape);
		messageHandler.DisplayMessage(message);
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
	case INJECTION:
		// Display step
		messageHandler.DisplayMessage(_T("D�but Injection dans le volume de r�f�rence %d\r\n"), injection);
		messageHandler.DisplayStep(_T("Petites Doses %d : Injection dans le volume de r�f�rence %d"), dose, injection);
		
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
			// Pause the thread
			experimentLocalData.experimentStage = PAUSE;

			// Tell GUI
			messageHandler.DisplayMessage(_T("Probl�me d'injection - demande de pause\r\n"));
			CString message;
			message = _T("ATTENTION !!! probl�me de gaz !!!\t\n");
			message += _T("(gaz moteur ou gaz d'analyse)\t\n");
			message += _T("Exp�rience en pause !\r\n");
			message += _T("N'oubliez pas de 'reprendre' l'exp�rience !\t\n");
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

