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
	if (storage.currentData->experimentStepStatus == STEP_STATUS_START) {

		storage.currentData->experimentStepStatus = STEP_STATUS_END;											// Set next step
		controls.messageHandler->DisplayMessage(MESSAGE_EQUILIBRATION_STARTED);										// Let GUI know the step change

		// This is where we start recording
		storage.currentData->experimentRecording = true;
		controls.messageHandler->GraphReset();

		// Create, open and write the columns in the:
		bool err = false;
		err = controls.fileWriter->EnteteCreate(*storage.experimentSettings, *storage.machineSettings);				// Entete TXT
		err = controls.fileWriter->EnteteCSVCreate(*storage.experimentSettings, *storage.machineSettings);			// Entete CSV
		if (err) {
			controls.messageHandler->DisplayMessageBox(ERROR_PATHUNDEF, MB_ICONERROR | MB_OK, false);
		}
		controls.fileWriter->FileMeasurementCreate(storage.experimentSettings->dataGeneral);						// Measurement file


		storage.currentData->timeStart = time(0);		// Record experiment start time
		controls.timerExperiment.TopChrono();							// Start global experiment timer	
		controls.timerMeasurement.TopChrono();							// Start the timer to record time between measurements

		// Set the time to wait
		WaitMinutes(storage.currentData->timeToEquilibrate = storage.experimentSettings->dataDivers.temps_ligne_base);
	}

	if (storage.currentData->experimentStepStatus == STEP_STATUS_END) {

		if (storage.currentData->experimentWaiting == false) {
			storage.currentData->experimentStage = STAGE_ADSORPTION;												// Set next stage
			storage.currentData->experimentStepStatus = STEP_STATUS_START;										// Reset next step
			controls.messageHandler->DisplayMessage(MESSAGE_EQUILIBRATION_COMPLETE);										// Let GUI know the step change
		}
	}
}
