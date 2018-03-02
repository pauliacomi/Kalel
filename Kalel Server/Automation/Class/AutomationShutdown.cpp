#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"


void Automation::Shutdown()
{
	switch (shutdownReason)
	{

	case STOP_CANCEL:		// This cancels an experiment in progress, GUI must ask check for experiment running
		
		//When thread finishes, let main window know to unlock menu and reset graph
		LOG(logINFO) << MESSAGE_EXPCANCEL;
		LOG(logINFO) << MESSAGE_FILLLINE;

		// Stop all timers 
		controls.timerExperiment.Pause();
		controls.timerMeasurement.Pause();
		controls.timerWaiting.Pause();

		// Run reset funtion
		ResetAutomation();
		storage.resetExperimentSettings();

		break;

	case STOP_NORMAL:		// This option is used if the experiment finishes correctly
							// It then resets everything

		//When thread finishes, let main window know to unlock menu
		LOG(logINFO) << MESSAGE_EXPFINISH;					// Experiment has been finished normally
		LOG(logINFO) << MESSAGE_FILLLINE;

		// Stop all timers 
		controls.timerExperiment.Pause();
		controls.timerMeasurement.Pause();
		controls.timerWaiting.Pause();

		// Reset all data from the experiment
		storage.currentData->ResetData();

		// Run reset funtion
		ResetAutomation();
		storage.resetExperimentSettings();

		break;

	case STOP_COMPLETE:		// This option is used if the automation thread is to be closed

		// When thread finishes, let main window know to unlock menu
		LOG(logINFO) << MESSAGE_THREAD_SHUTDOWN;

		// Break loop
		running = false;

		break;

	default:
		throw;		// Error, should never be reached
		break;
	}
}


void Automation::Pause()
{
	if (storage.currentData->experimentInProgress)
	{
		controls.timerWaiting.Pause();
		storage.currentData->experimentRecording = false;

		// Log the pause
		LOG(logINFO) << MESSAGE_EXPPAUSE;
	}
	controls.timerExperiment.Pause();
	controls.timerMeasurement.Pause();
	storage.currentData->experimentCommandsRequested = false;
}


void Automation::Resume()
{
	if (storage.currentData->experimentInProgress)
	{
		controls.timerWaiting.Resume();
		storage.currentData->experimentRecording = true;

		// Log the resume
		LOG(logINFO) << MESSAGE_EXPRESUME;
	}
	controls.timerExperiment.Resume();
	controls.timerMeasurement.Resume();
	storage.currentData->experimentCommandsRequested = true;
}