#include "stdafx.h"
#include "Automation.h"

void Automation::StepEquilibration()
{
	switch (experimentLocalData.stepStatus)
	{
	case STEP_STATUS_INPROGRESS:
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
		// Close all valves
		closeall
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

	// WRITE NAME OF STEP DISPLAYMESSAGE
	// CLOSE ALL VALVES
	// SET DOSE NUMBER TO 1
	// LOOP
	// DISPLAY DOSE NUMBER
	//
	// DISPLAY DOSE NUMBER
	//
	// WHILE (PRESSURE < FINAL REQUESTED PRESSURE OF THE STEP) 
	// DISPLAY END STEP
	// DISPLAY END MESSAGE
	// CHANGE EXPERIMENT STAGE
}

void Automation::StepDesorption()
{
}