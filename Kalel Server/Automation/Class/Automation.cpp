#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

Automation::Automation(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
	// Time
}


Automation::~Automation()
{
	// Close valves/pump
	controls.valveControls.CloseAll(false);
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
//		1. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
//		2. IF WAITING
//				check whether the wait is complete and reset the wait
//		3. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
//	}
//
//
//
////////////////////////////////////////////////////////
void Automation::Execution()
{
	LOG(logINFO) << MESSAGE_THREAD_START_AUTO;

	// Infinite loop, it is broken from the inside
	while (running)
	{
		/*
		*
		*		1. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
		*
		*/

		// Go through any functionality
		if (storage.experimentStatus.experimentCommandsRequested) {
			switch (storage.experimentSettings.experimentType)		// We look at the type of experiment
			{
			case EXPERIMENT_TYPE_MANUAL:							// in case it is manual
				ExecutionManual();									// run the manual loop
				break;
			case EXPERIMENT_TYPE_AUTO:								// in case it is automatic
				ExecutionAuto();									// run the automatic loop
				break;
			case EXPERIMENT_TYPE_SAMPLE_VACUUM:						// in case we want to vacuum up to the sample
				SampleVacuum();										// run the functionality
				break;
			case EXPERIMENT_TYPE_BOTTLE_VACUUM:						// in case we want to vacuum up to the bottle
				BottleVacuum();										// run the functionality
				break;
			case EXPERIMENT_TYPE_CONTINUOUS:						// in case it is continuous
				ExecutionContinuous();								// run the continuous loop
				break;
			case EXPERIMENT_TYPE_UNDEF:								// in case no experiment has been set yet
				break;												// just continue
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
		if (storage.experimentStatus.experimentWaiting &&															// If the wait functionality is requested																					
			storage.timerWaiting.TimeSeconds() > storage.experimentStatus.timeToEquilibrate) {					// and the time has been completed

			// Stop the timer
			storage.timerWaiting.Pause();

			// Reset the flag
			storage.experimentStatus.experimentWaiting = false;
		}

		/*
		*
		*		3. IF RECORDING save the data to the file, and restart timer between records
		*
		*/

		// Write data
		if (storage.experimentStatus.experimentRecording)														// If we started recording
		{
			if (storage.timerMeasurement.TimeMilliseconds() > storage.machineSettings.TimeBetweenRecording)	// If enough time between measurements
			{
				// Save the data to the file
				bool err = controls.fileWriter.RecordMeasurement(
					storage.experimentSettings.dataGeneral,
					storage.currentData,
					storage.experimentStatus,
					controls.valveControls.ValveIsOpen(ID_VALVE_6));
				if (err) {
					LOG(logERROR) << ERROR_FILE_WRITE;
				}

				// Restart the timer to record time between measurements
				storage.timerMeasurement.Start();
			}
		}
		/*
		*
		*		3. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
		*		
		*/

		// Aquire the lock
		std::unique_lock<std::mutex> lock(storage.automationMutex);

		// Wait until called or until timeout
		bool notified = storage.automationControl.wait_for(lock, std::chrono::milliseconds(storage.machineSettings.TimeBetweenAutomation), [&] () 
		{
			return (eventShutdown || eventPause || eventResume || eventChangeExpSett || eventUserInput);
		});

		// Now run through the possible events
		if (notified)									// If there was a notification, check what kind
		{

			if (eventPause)								// Pause thread
			{
				Pause();
				eventPause = false;
				continue;
			}

			if (eventResume)							// Resume thread
			{
				Resume();
				eventResume = false;
				continue;
			}

			if (eventShutdown)							// Complete stop of thread
			{
				Shutdown();
				eventShutdown = false;
				continue;
			}

			if (eventChangeExpSett)						// Change experiment settings
			{
				if (storage.tExperimentSettings) {
					storage.experimentSettings = *storage.tExperimentSettings;				// timestamp included
					storage.tExperimentSettings.reset();
				}
				eventChangeExpSett = false;
				continue;
			}

			if (eventUserInput)							// Wait for user input
			{
				userChoice = 0;							// make sure no choice selected
				Pause();								// pause everything
				eventUserInput = false;					// reset event
				continue;
			}
		}
	}
}



void Automation::ExecutionManual()
{
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_UNDEF) {

		// Send start message
		LOG(logINFO) << MESSAGE_EXPSTART;
		
		ResetAutomation();

		// Create open and write the columns in the file
		bool err = controls.fileWriter.CreateFiles(storage.experimentSettings, storage.machineSettings);
		if (err) {								// No point in starting experiment then																										
			shutdownReason = Stop::Error;
			eventShutdown = true;
			storage.automationControl.notify_all();
			LOG(logERROR) << ERROR_PATHUNDEF;
		}

		// Record start
		storage.experimentStatus.experimentInProgress = true;
		storage.experimentStatus.experimentRecording = true;

		// Write Time of start
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		// Continue experiment
		storage.experimentStatus.experimentStage = STAGE_MANUAL;
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_INPROGRESS;
	}
}



void Automation::ExecutionAuto()
{
	// First time running command
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_UNDEF){

		// Send start message
		LOG(logINFO) << MESSAGE_EXPSTART;

		ResetAutomation();

		// Create, open and write the columns in the file
		bool err = controls.fileWriter.CreateFiles(storage.experimentSettings, storage.machineSettings);
		if (err) {							// No point in starting experiment then																							
			shutdownReason = Stop::Error;
			eventShutdown = true;
			storage.automationControl.notify_all();
			LOG(logERROR) << ERROR_PATHUNDEF;
		}

		// Write variables to starting position
		storage.experimentStatus.experimentInProgress = true;
		storage.experimentStatus.experimentStage = STAGE_VERIFICATIONS;
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_START;
		storage.experimentStatus.verificationStep = STEP_VERIFICATIONS_SECURITY;
	}

	// Stages of automatic experiment
	switch (storage.experimentStatus.experimentStage)
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
		StageVacuum();
		break;
	case STAGE_END_AUTOMATIC:

		// If the experiment has finished
		shutdownReason = Stop::Normal;				// set a normal shutdown
		eventShutdown = true;						// end then set the event
		storage.automationControl.notify_all();		// and notify

		break;

	default:
		break;
	}
}

void Automation::ExecutionContinuous()
{
	// First time running command
	if (storage.experimentStatus.experimentStepStatus == STEP_STATUS_UNDEF) {

		// Send start message
		LOG(logINFO) << MESSAGE_EXPSTART;

		ResetAutomation();

		// Create, open and write the columns in the file
		bool err = controls.fileWriter.CreateFiles(storage.experimentSettings, storage.machineSettings);
		if (err) {							// No point in starting experiment then																							
			shutdownReason = Stop::Error;
			eventShutdown = true;
			storage.automationControl.notify_all();
			LOG(logERROR) << ERROR_PATHUNDEF;
		}

		// Write variables to starting position
		storage.experimentStatus.experimentInProgress = true;
		storage.experimentStatus.experimentStage = STAGE_VERIFICATIONS;
		storage.experimentStatus.experimentStepStatus = STEP_STATUS_START;
		storage.experimentStatus.experimentSubstepStage = SUBSTEP_STATUS_START;
		storage.experimentStatus.verificationStep = STEP_VERIFICATIONS_SECURITY;
	}

	// Stages of automatic experiment
	switch (storage.experimentStatus.experimentStage)
	{
	case STAGE_VERIFICATIONS:
		Verifications();
		break;
	case STAGE_EQUILIBRATION:
		StageEquilibration();
		break;
	case STAGE_ADSORPTION:
		StageContinuous();
		break;
	case STAGE_VACUUM_SAMPLE:
		StageVacuum();
		break;
	case STAGE_END_AUTOMATIC:

		// If the experiment has finished
		shutdownReason = Stop::Normal;				// set a normal shutdown
		eventShutdown = true;						// end then set the event
		storage.automationControl.notify_all();		// and notify

		break;

	default:
		break;
	}
}

void Automation::ResetAutomation()
{
	// Reset all data from the experiment
	storage.experimentStatus.ResetData();
	storage.experimentSettings.ResetData();

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = Stop::Normal;

	// Delete all current measurements
	storage.dataCollection.del();
}