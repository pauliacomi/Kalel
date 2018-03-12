#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

Automation::Automation(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
	// Time
	controls.timerExperiment.Start();				// Start global experiment timer	
	storage.experimentStatus->timeStart = time(0);
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
	// Infinite loop, it is broken from the inside
	while (running)
	{
		/*
		*
		*		1. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
		*
		*/

		// Go through any functionality
		if (storage.experimentStatus->experimentCommandsRequested) {
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
			case EXPERIMENT_TYPE_CONTINUOUS:					// in case it is continuous
				//ExecutionContinuous();							// run the continuous loop
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
		if (storage.experimentStatus->experimentWaiting &&															// If the wait functionality is requested																					
			controls.timerWaiting.TimeSeconds() > storage.experimentStatus->timeToEquilibrate) {					// and the time has been completed

			// Stop the timer
			controls.timerWaiting.Pause();

			// Reset the flag
			storage.experimentStatus->experimentWaiting = false;
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
			return (eventShutdown || eventPause || eventResume || eventReset || eventUserInput);
		});

		if (notified)
		{
			if (eventShutdown)							// Complete stop of thread
			{
				shutdownReason = STOP_COMPLETE;
				Shutdown();
				eventShutdown = false;
				continue;
			}

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

			if (eventReset)								// Reset thread
			{
				Shutdown();
				eventReset = false;
				continue;
			}

			if (eventUserInput)							// Wait for user input
			{
			}
		}
	}
}






void Automation::ExecutionManual()
{
	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_UNDEF) {

		// Send start message
		LOG(logINFO) << MESSAGE_EXPSTART;
		
		ResetAutomation();

		// Record start
		storage.experimentStatus->experimentInProgress = true;
		storage.experimentStatus->experimentRecording = true;

		// Create open and write the columns in the file
		bool err = false;
		err = controls.fileWriter.EnteteCreate(*storage.experimentSettings, *storage.machineSettings);				// Entete TXT
		err = controls.fileWriter.EnteteCSVCreate(*storage.experimentSettings, *storage.machineSettings);			// Entete CSV
		err = controls.fileWriter.FileMeasurementCreate(storage.experimentSettings->dataGeneral);					// Measurement file
		if (err){
			LOG(logERROR) << ERROR_PATHUNDEF;
		}

		// Continue experiment
		storage.experimentStatus->experimentStage = STAGE_MANUAL;
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_INPROGRESS;
	}
}



void Automation::ExecutionAuto()
{
	// First time running command
	if (storage.experimentStatus->experimentStepStatus == STEP_STATUS_UNDEF){

		// Send start message
		LOG(logINFO) << MESSAGE_EXPSTART;

		ResetAutomation();

		// Write variables to starting position
		storage.experimentStatus->experimentInProgress = true;
		storage.experimentStatus->experimentStage = STAGE_VERIFICATIONS;
		storage.experimentStatus->experimentStepStatus = STEP_STATUS_START;
		storage.experimentStatus->experimentSubstepStage = SUBSTEP_STATUS_START;
		storage.experimentStatus->verificationStep = STEP_VERIFICATIONS_SECURITY;
	}

	// Stages of automatic experiment
	switch (storage.experimentStatus->experimentStage)
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
		shutdownReason = STOP_NORMAL;	// set a normal shutdown

		eventReset = true;				// end then set the event
		// TODO: check if this notification works in advance
		storage.automationControl.notify_all();

		break;

	default:
		break;
	}
}



void Automation::ResetAutomation()
{
	// Reset all data from the experiment
	storage.experimentStatus->ResetData();

	// If the shutdown event is called externally, it will default to a cancel
	// Otherwise the flag will be changed from inside the code
	shutdownReason = STOP_CANCEL;

	// Delete all current measurements
	storage.dataCollection.del();

	// Time
	storage.experimentStatus->timeStart = time(0);
	controls.timerExperiment.Start();	// Start global experiment timer	
	controls.timerMeasurement.Start();	// Start the timer to record time between measurements
}