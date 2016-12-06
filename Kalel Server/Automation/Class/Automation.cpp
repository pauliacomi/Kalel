#include "Automation.h"

Automation::Automation(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
	// Initialise class members
	sb_settingsModified = false;
	sb_userContinue = false;

	// Initialise events
	//   - Non signalled by default
	//   - With manual reinitiallisation
	h_eventShutdown = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventResume = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventPause = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventReset = CreateEvent(NULL, TRUE, FALSE, NULL);
	h_eventUserInput = CreateEvent(NULL, TRUE, FALSE, NULL);

	events[0] = h_eventShutdown;
	events[1] = h_eventPause;
	events[2] = h_eventResume;
	events[3] = h_eventReset;
	events[4] = h_eventUserInput;

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;

	// Time
	storage.currentData->timeStart = time(0);
	controls.timerExperiment.TopChrono();	// Start global experiment timer	
	controls.timerMeasurement.TopChrono();	// Start the timer to record time between measurements
}


Automation::~Automation()
{
	// Close valves/pump
	controls.valveControls->CloseAll(false);

	// Destroy the events
	CloseHandle(h_eventShutdown);
	CloseHandle(h_eventResume);
	CloseHandle(h_eventPause);
	CloseHandle(h_eventReset);
	CloseHandle(h_eventUserInput);
}

////////////////////////////////////////////////////////
//	Main execution function
//
//	This is an infinite loop which has to be broken form inside
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
		*		1. Get the experiment settings if they are new
		*
		*/

		if (sb_settingsModified) {
			if (storage.currentData->experimentInProgress == true) {
				ExperimentSettings tempSettings = GetSettings();																// We have the two settings coexisting to record any changes
				controls.fileWriter->RecordDataChange(false, tempSettings, *storage.experimentSettings, *storage.currentData);	// non-CSV
				controls.fileWriter->RecordDataChange(true, tempSettings, *storage.experimentSettings, *storage.currentData);	// CSV
				storage.experimentSettings = std::make_shared<ExperimentSettings>(tempSettings);								// Now save the new settings as the old ones
			}
			else
				storage.experimentSettings = std::make_shared<ExperimentSettings>(GetSettings());
		}

		/*
		*
		*		2. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
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
				ASSERT(0); // Error
				break;
			}
		}


		/*
		*
		*		3. IF WAITING check whether the wait is complete and reset the wait
		*
		*/

		// If waiting complete
		if (storage.currentData->experimentWaiting &&														// If the wait functionality is requested																					
			storage.currentData->timeToEquilibrateCurrent > storage.currentData->timeToEquilibrate) {			//and the time has been completed

			// Stop the timer
			controls.timerWaiting.ArretTemps();

			// Reset the flag
			storage.currentData->experimentWaiting = false;
		}
		
		/*
		*
		*		4. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
		*		
		*/

		// Now run through the possible events
		switch (::WaitForMultipleObjects(5, events, FALSE, T_BETWEEN_AUTOMATION)) // (ms) Poll time
		{

		case WAIT_OBJECT_0:												// Complete stop of thread
			shutdownReason = STOP_COMPLETE;
			Shutdown();
			::ResetEvent(h_eventShutdown);	// Reset the event
			break;

		case WAIT_OBJECT_0 + 1:											// Pause thread
			Pause();
			::ResetEvent(h_eventPause);		// Reset the event
			break;

		case WAIT_OBJECT_0 + 2:											// Resume thread
			Resume();
			::ResetEvent(h_eventResume);	// Reset the event
			break;

		case WAIT_OBJECT_0 + 3:											// Reset thread
			Shutdown();
			break;

		case WAIT_OBJECT_0 + 4:											// Wait for user input
			break;

		case WAIT_TIMEOUT:
			break;

		default:
			ASSERT(FALSE); // unknown error
			break;
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
		storage.experimentSettings = std::make_shared<ExperimentSettings>(GetSettings());

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
		controls.fileWriter->FileMeasurementOpen(storage.experimentSettings->dataGeneral);							// Measurement file

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
		storage.experimentSettings = std::make_shared<ExperimentSettings>(GetSettings());

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
		::SetEvent(h_eventReset);	// end then set the event

		break;

	case STAGE_CONTINUOUS_ADSORPTION:
		break;
	default:
		ASSERT(0); // Error
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
	controls.timerExperiment.TopChrono();	// Start global experiment timer	
	controls.timerMeasurement.TopChrono();	// Start the timer to record time between measurements
}



ExperimentSettings Automation::GetSettings()
{
	ExperimentSettings tempSettings;

	// Copy the data from the main thread, no need for synchronisation as we are only copying
	tempSettings = storage.experimentSettings.get();

	sb_settingsModified = false;

	return tempSettings;
}
