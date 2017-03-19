#include "Automation.h"

Automation::Automation(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
	// Initialise class members
	sb_settingsModified = false;

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;

	// Time
	storage.currentData->timeStart = time(0);
	controls.timerExperiment.Start();				// Start global experiment timer	
	controls.timerMeasurement.Start();				// Start the timer to record time between measurements
}


Automation::~Automation()
{
	// Close valves/pump
	controls.valveControls->CloseAll(false);
}

////////////////////////////////////////////////////////
//	Main execution function
//
//	This is an infinite loop which has to be broken from inside
//	The general flowchart goes as follows:
//	
//	initialize everything
//	loop
//	{
//		1. Get the experiment settings if they are new
//		2. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
//		3. IF WAITING
//				check whether the wait is complete and reset the wait
//		4. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
//	}
//
//
//
////////////////////////////////////////////////////////
void Automation::Execution()
{
	// Infinite loop, it is broken from the inside
	while (running)
	{
		/*
		*
		*		1. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
		*
		*/

		// Go through any functionality
		if (storage.currentData->experimentCommandsRequested) {
			switch (storage.experimentSettings->experimentType)		// We look at the type of experiment
			{
			case EXPERIMENT_TYPE_MANUAL:						// in case it is manual
				ExecutionManual();								// run the manual loop
				break;
			case EXPERIMENT_TYPE_AUTO:							// in case it is automatic
				ExecutionAuto();								// run the automatic loop
				break;
			case EXPERIMENT_TYPE_SAMPLE_VACUUM:					// in case we want to vacuum up to the sample
				SampleVacuum();									// run the functionality
				break;
			case EXPERIMENT_TYPE_BOTTLE_VACUUM:					// in case we want to vacuum up to the bottle
				BottleVacuum();									// run the functionality
				break;
			case EXPERIMENT_TYPE_UNDEF:							// in case no experiment has been set yet
				break;											// just continue
			default:
				break;
			}
		}

		/*
		*
		*		2. IF WAITING check whether the wait is complete and reset it
		*
		*/

		// If waiting complete
		if (storage.currentData->experimentWaiting &&														// If the wait functionality is requested																					
			storage.currentData->timeToEquilibrateCurrent > storage.currentData->timeToEquilibrate) {			//and the time has been completed

			// Stop the timer
			controls.timerWaiting.Pause();

			// Reset the flag
			storage.currentData->experimentWaiting = false;
		}
		
		/*
		*
		*		3. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
		*		
		*/

		// Now run through the possible events
		// Wait until called
		std::unique_lock<std::mutex> lock(storage.automationMutex);
		auto notified = storage.automationControl.wait_for(lock, std::chrono::milliseconds(T_BETWEEN_AUTOMATION), [&] () 
		{
			return (h_eventShutdown || h_eventPause || h_eventResume || h_eventReset || h_eventUserInput);
		});

		if (notified)
		{
			if (sb_settingsModified)						// Get the expermient settings if they are modified
			{
				sb_settingsModified = false;

				if (storage.currentData->experimentInProgress == true) {
					controls.fileWriter->RecordDataChange(false, *storage.newExperimentSettings, *storage.experimentSettings, *storage.currentData);	// non-CSV
					controls.fileWriter->RecordDataChange(true, *storage.newExperimentSettings, *storage.experimentSettings, *storage.currentData);		// CSV
				}

				storage.experimentSettings = std::make_shared<ExperimentSettings>(*storage.newExperimentSettings);
			}


			if (h_eventShutdown)							// Complete stop of thread
			{
				shutdownReason = STOP_COMPLETE;
				Shutdown();
				h_eventShutdown = false;
				continue;
			}

			if (h_eventPause)								// Pause thread
			{
				Pause();
				h_eventPause = false;
				continue;
			}
			
			if (h_eventResume)								// Resume thread
			{
				Resume();
				h_eventResume = false;
				continue;
			}												

			if (h_eventReset)								// Reset thread
			{
				Shutdown();
				continue;
			}

			if (h_eventUserInput)							// Wait for user input
			{
			}
		}
	}
}






bool Automation::ExecutionManual()
{
	if (storage.currentData->experimentStepStatus == STEP_STATUS_UNDEF) {

		// Send start message
		controls.messageHandler->ExperimentStart();
		
		ResetAutomation();

		// Get data
		storage.experimentSettings = std::make_shared<ExperimentSettings>(*storage.newExperimentSettings);

		// Record start
		storage.currentData->experimentInProgress = true;
		storage.currentData->experimentRecording = true;

		// Create open and write the columns in the:
		bool err = false;
		err = controls.fileWriter->EnteteCreate(*storage.experimentSettings, *storage.machineSettings);				// Entete TXT
		err = controls.fileWriter->EnteteCSVCreate(*storage.experimentSettings, *storage.machineSettings);			// Entete CSV
		if (err){
			controls.messageHandler->DisplayMessageBox(ERROR_PATHUNDEF, MB_ICONERROR | MB_OK, false);
		}
		controls.fileWriter->FileMeasurementCreate(storage.experimentSettings->dataGeneral);							// Measurement file

		// Continue experiment
		storage.currentData->experimentStage = STAGE_MANUAL;
		storage.currentData->experimentStepStatus = STEP_STATUS_INPROGRESS;
		storage.currentData->experimentCommandsRequested = false;

		return true;
	}

	return false;
}



bool Automation::ExecutionAuto()
{
	// First time running command
	if (storage.currentData->experimentStepStatus == STEP_STATUS_UNDEF){

		// Send start message
		controls.messageHandler->ExperimentStart();

		ResetAutomation();

		// Get data
		storage.experimentSettings = std::make_shared<ExperimentSettings>(*storage.newExperimentSettings);

		// Write variables to starting position
		storage.currentData->experimentInProgress = true;
		storage.currentData->experimentStage = STAGE_VERIFICATIONS;
		storage.currentData->experimentStepStatus = STEP_STATUS_START;
		storage.currentData->experimentSubstepStage = SUBSTEP_STATUS_START;
		storage.currentData->verificationStep = STEP_VERIFICATIONS_SECURITY;
	}

	// Stages of automatic experiment
	switch (storage.currentData->experimentStage)
	{
	case STAGE_VERIFICATIONS:
		Verifications();
		break;
	case STAGE_EQUILIBRATION:
		StageEquilibration();
		break;
	case STAGE_ADSORPTION:
		StageAdsorption();
		break;
	case STAGE_DESORPTION:
		StageDesorption();
		break;
	case STAGE_VACUUM_SAMPLE:
		StageDesorption();
		break;
	case STAGE_END_AUTOMATIC:

		// If the experiment has finished
		shutdownReason = STOP_NORMAL;	// set a normal shutdown

		h_eventReset = true;			// end then set the event
		storage.automationControl.notify_all();

		break;

	case STAGE_CONTINUOUS_ADSORPTION:
		break;
	default:
		break;
	}

	return true;
}



void Automation::ResetAutomation()
{
	// Reset all data from the experiment
	storage.currentData->ResetData();
	storage.experimentSettings->ResetData();

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;

	// Time
	storage.currentData->timeStart = time(0);
	controls.timerExperiment.Start();	// Start global experiment timer	
	controls.timerMeasurement.Start();	// Start the timer to record time between measurements
}