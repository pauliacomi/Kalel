#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

/*
*
*
*			EQUILIBRATION
*
*
*/


void Automation::StageEquilibration()
{
	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_START) {

		LOG(logINFO) << MESSAGE_EQUILIBRATION_STARTED;																// Log the step change

		// This is where we start recording
		storage.experimentStatus->experimentRecording = true;

		// Create, open and write the columns in the file
		bool err = false;
		err = controls.fileWriter->EnteteCreate(*storage.experimentSettings, *storage.machineSettings);				// Entete TXT
		err = controls.fileWriter->EnteteCSVCreate(*storage.experimentSettings, *storage.machineSettings);			// Entete CSV
		controls.fileWriter->FileMeasurementCreate(storage.experimentSettings->dataGeneral);						// Measurement file
		if (err) {
			LOG(logERROR) << ERROR_PATHUNDEF;
		}

		storage.experimentStatus->timeStart = time(0);																	// Record experiment start time
		controls.timerExperiment.Start();																			// Start global experiment timer	
		controls.timerMeasurement.Start();																			// Start the timer to record time between measurements

		// Set the time to wait
		WaitMinutes(storage.experimentSettings->dataDivers.temps_ligne_base);

		storage.experimentStatus->experimentStepStatus = STEP_STATUS_END;												// Set next step
	}

	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_END) {

		if (storage.experimentStatus->experimentWaiting == false) {
			LOG(logINFO) << MESSAGE_EQUILIBRATION_COMPLETE;															// Log the step change
			storage.experimentStatus->experimentStage = STAGE_ADSORPTION;												// Set next stage
			storage.experimentStatus->experimentStepStatus = STEP_STATUS_START;										// Reset next step
		}
	}
}
