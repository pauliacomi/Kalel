#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"

Automation::Automation(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
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

		if (storage.experimentStatus.isRunningAutomation) {
			if (!storage.experimentStatus.isWaiting)
			{
				// Go through any functionality
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

			else																								// If the wait functionality is requested
				if (storage.timerWaiting.TimeSeconds() > storage.experimentStatus.timeToWait) {					// and the time has been completed

				// Reset the flag
					storage.experimentStatus.isWaiting = false;
				}

		/*
		*
		*		3. IF RECORDING save the data to the file, and restart timer between records
		*
		*/

			// Write data
			if (storage.experimentStatus.isRecording)														// If we started recording
			{
				if (storage.timerRecording.TimeMilliseconds() > storage.machineSettings.TimeBetweenRecording)		// If enough time between measurements
				{
					// Save the data to the file
					bool err = controls.fileWriter.RecordMeasurement(
						storage.experimentSettings.dataGeneral,
						storage.currentData,
						storage.experimentStatus,
						controls.valveControls.ValveIsOpen(ID_VALVE_6));
					if (err) {
						LOG_EVENT(qOK) << ERROR_FILE_WRITE;
						eventUserInput = true;
						storage.automationControl.notify_all();
					}

					// Restart the timer to record time between measurements
					storage.timerRecording.Start();
				}
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
					storage.experimentSettings = *storage.tExperimentSettings;				// timestamp change included
					storage.tExperimentSettings.reset();
				}
				eventChangeExpSett = false;
				continue;
			}

			if (eventUserInput)							// Wait for user input
			{
				storage.experimentStatus.isWaitingUser = true;
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
	switch (storage.experimentStatus.mainStage)
	{
	case STAGE_UNDEF:
	{
		// Send start message
		LOG(logINFO) << MESSAGE_EXPSTART;

		ResetAutomation();

		// Create open and write the columns in the file
		bool err = controls.fileWriter.CreateFiles(storage.experimentSettings, storage.machineSettings);
		if (err) {								// No point in starting experiment then																										
			shutdownReason = Stop::Error;
			eventShutdown = true;
			storage.automationControl.notify_all();
			LOG(logERROR) << ERROR_FILE_WRITE;
		}

		// Record start
		storage.experimentStatus.inProgress = true;
		storage.experimentStatus.isRecording = true;

		// Start the timer to record time between recording of measurements
		storage.timerRecording.Start();

		// Write Time of start
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		// Continue experiment
		++storage.experimentStatus.mainStage;
		storage.experimentStatus.stepStatus = STEP_STATUS_INPROGRESS;
		break;
	}
	default:
		break;
	}
}


void Automation::ExecutionAuto()
{
	// Stages of automatic experiment
	switch (storage.experimentStatus.mainStage)
	{
	case STAGE_UNDEF:
	{
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

		// Record start
		storage.experimentStatus.inProgress = true;

		// Advance
		storage.experimentStatus.mainStage = STAGE_AUTO_VERIFICATIONS;
		break;
	}
	case STAGE_AUTO_VERIFICATIONS:
		Verifications();
		break;
	case STAGE_AUTO_EQUILIBRATION:
		StageEquilibration(storage.experimentSettings.dataDivers.time_baseline);
		break;
	case STAGE_AUTO_RECORD:

		// This is where we start recording
		storage.experimentStatus.isRecording = true;

		// Start the timer to record time between recording of measurements
		storage.timerRecording.Start();

		// Record experiment start time
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		++storage.experimentStatus.mainStage;
		break;

	case STAGE_AUTO_ADSORPTION:
		StageDiscreteAdsorption();
		break;
	case STAGE_AUTO_DESORPTION:
		StageDiscreteDesorption();
		break;
	case STAGE_AUTO_VACUUM_SAMPLE:
		StageVacuum();
		break;
	case STAGE_AUTO_END:

		// If the experiment has finished
		eventShutdown = true;						// set the event
		storage.automationControl.notify_all();		// and notify

		break;

	default:
		break;
	}
}

void Automation::ExecutionContinuous()
{
	// Stages of automatic continuous experiment
	switch (storage.experimentStatus.mainStage)
	{
	case STAGE_UNDEF:
	{
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

		// Record start
		storage.experimentStatus.inProgress = true;
		storage.experimentStatus.isRecording = true;
		
		// Start the timer to record time between recording of measurements
		storage.timerRecording.Start();

		// Record experiment start time
		storage.experimentStatus.timeStart = timeh::TimePointToMs(timeh::NowTime());

		// Advance
		++storage.experimentStatus.mainStage;
		break;
	}
	case STAGE_CONT_VERIFICATIONS:
		Verifications();
		break;
	case STAGE_CONT_FLOWRATE:
		StageContinuousFlowrate();
		break;
	case STAGE_CONT_DEADVOL:
		StageContinuousDeadvolume();
		break;
	case STAGE_CONT_VACUUM_BEFOREEX:
		StageContVacuum();
	case STAGE_CONT_EQUILIBRATION:
		StageEquilibration(storage.experimentSettings.dataContinuous.temps_equilibre_continue);
		break;
	case STAGE_CONT_ADSORPTION:
		StageContinuousAdsorption();
		break;
	case STAGE_CONT_END_EQUILIBRATION:
		StageEquilibration(storage.experimentSettings.dataContinuous.temps_final_equilibre);
		break;
	case STAGE_CONT_END:

		// If the experiment has finished
		eventShutdown = true;						// set the event
		storage.automationControl.notify_all();		// and notify

		break;

	default:
		break;
	}
}

void Automation::ResetAutomation()
{
	// Reset status from the experiment
	storage.experimentStatus.ResetData();
	
	// Delete all current measurements
	storage.dataCollection.del();

	// If the shutdown event not called externally, default to normal stop
	shutdownReason = Stop::Normal;
}