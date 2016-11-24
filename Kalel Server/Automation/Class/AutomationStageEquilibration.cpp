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
		experimentLocalData.experimentRecording = true;
		messageHandler.GraphReset();

		// Create, open and write the columns in the:
		EnteteCreate();				// Entete TXT
		EnteteCSVCreate();			// Entete CSV
		FileMeasurementOpen();		// Measurement file

		experimentLocalData.timeStart = time(0);		// Record experiment start time
		timerExperiment.TopChrono();							// Start global experiment timer	
		timerMeasurement.TopChrono();							// Start the timer to record time between measurements

		// Set the time to wait
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
