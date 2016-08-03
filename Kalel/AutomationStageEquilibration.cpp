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
