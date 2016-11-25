#include "Automation.h"

Automation::Automation(Storage &h)
	: running(true)
	, messageHandler{ h }
{
	// Save pointer to experiment settings
	experimentSettings = h.experimentSettings;

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

	// Initialise instruments
	g_pVanne = new CVannes();

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;

	// Time
	experimentLocalData.timeStart = time(0);
	timerExperiment.TopChrono();	// Start global experiment timer	
	timerMeasurement.TopChrono();	// Start the timer to record time between measurements
}


Automation::~Automation()
{
	// Close valves/pump
	ControlMechanismsCloseAll();

	// Delete instruments
	delete g_pVanne;

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
//		3. Measure values from instruments
//		4. Do a security and safety check on the values
//		5. Record the time the measurement was done / the time between measurments
//		6. IF RECORDING, save the data to the file, restart timer between measurements and increment measurement number
//		7. IF WAITING, check whether the wait is complete and reset the wait
//		8. Display the data to the GUI
//		9. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
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
		*		SETTINGS RETRIEVAL
		*
		*/

		if (sb_settingsModified) {
			if (experimentLocalData.experimentInProgress == true) {
				ExperimentSettings tempSettings = GetSettings();	// We have the two settings coexisting to record any changes
				RecordDataChange(tempSettings, false);				// non-CSV
				RecordDataChange(tempSettings, true);				// CSV
				experimentLocalSettings = tempSettings;				// Now save the new settings as the old ones
			}
			else
				experimentLocalSettings = GetSettings();
		}

		/*
		*
		*		AUTOMATION
		*
		*/

		// Go through any functionality
		if (experimentLocalData.experimentCommandsRequested) {
			switch (experimentLocalSettings.experimentType)		// We look at the type of experiment
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
		*		NON-BLOCKING WAITING FUNCTIONALITY
		*
		*/

		// If waiting complete
		if (experimentLocalData.experimentWaiting &&														// If the wait functionality is requested																					
			experimentLocalData.timeToEquilibrateCurrent > experimentLocalData.timeToEquilibrate) {			//and the time has been completed

			// Stop the timer
			timerWaiting.ArretTemps();

			// Reset the flag
			experimentLocalData.experimentWaiting = false;
		}
		
		/*
		*
		*		EVENT-BASED LOOP TIMING
		*		// Checks for thread states
		*/

		// Now run through the possible events
		switch (::WaitForMultipleObjects(5, events, FALSE, T_BETWEEN_MEASURE)) // (ms) Poll time
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
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_UNDEF) {

		// Send start message
		messageHandler.ExperimentStart();
		
		ResetAutomation();

		// Get data
		experimentLocalSettings = GetSettings();

		// Record start
		experimentLocalData.experimentInProgress = true;
		experimentLocalData.experimentRecording = true;

		// Create open and write the columns in the:
		bool err = false;
		err = EnteteCreate();				// Entete TXT
		err = EnteteCSVCreate();			// Entete CSV
		if (err){
			messageHandler.DisplayMessageBox(ERROR_PATHUNDEF, MB_ICONERROR | MB_OK, false);
		}
		FileMeasurementOpen();		// Measurement file

		// Continue experiment
		experimentLocalData.experimentStage = STAGE_MANUAL;
		experimentLocalData.experimentStepStatus = STEP_STATUS_INPROGRESS;
		experimentLocalData.experimentCommandsRequested = false;

		return true;
	}

	return false;
}



bool Automation::ExecutionAuto()
{
	// First time running command
	if (experimentLocalData.experimentStepStatus == STEP_STATUS_UNDEF){

		// Send start message
		messageHandler.ExperimentStart();

		ResetAutomation();

		// Get data
		experimentLocalSettings = GetSettings();

		// Write variables to starting position
		experimentLocalData.experimentInProgress = true;
		experimentLocalData.experimentStage = STAGE_VERIFICATIONS;
		experimentLocalData.experimentStepStatus = STEP_STATUS_START;
		experimentLocalData.experimentSubstepStage = SUBSTEP_STATUS_START;
		experimentLocalData.verificationStep = STEP_VERIFICATIONS_SECURITY;
	}

	// Stages of automatic experiment
	switch (experimentLocalData.experimentStage)
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
	experimentLocalData.ResetData();
	experimentLocalSettings.ResetData();

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;

	// Time
	experimentLocalData.timeStart = time(0);
	timerExperiment.TopChrono();	// Start global experiment timer	
	timerMeasurement.TopChrono();	// Start the timer to record time between measurements
}



ExperimentSettings Automation::GetSettings()
{
	ExperimentSettings tempSettings;

	// Copy the data from the main thread, no need for synchronisation as we are only copying
	tempSettings = experimentSettings.get();

	sb_settingsModified = false;

	return tempSettings;
}
