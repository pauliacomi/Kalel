#include "Automation.h"

// Utilities
#include "../../../Kalel Shared/log.h"


void Automation::Shutdown()
{
	switch (shutdownReason)
	{

	case Stop::Cancel:			// This cancels an experiment in progress
		
		//When thread finishes, let main window know to unlock menu and reset graph
		LOG(logINFO) << MESSAGE_EXPCANCEL;

		// Reset all data from the experiment
		storage.experimentSettings.ResetData();

		// Run reset funtion
		ResetAutomation();

		break;

	case Stop::Normal:			// This option is used if the experiment finishes correctly
								// It then resets everything

		LOG(logINFO) << MESSAGE_EXPFINISH;

		// Reset all data from the experiment
		storage.experimentSettings.ResetData();

		// Run reset funtion
		ResetAutomation();

		break;

	case Stop::Complete:		// This option is used if the automation thread is to be closed

		// When thread finishes, let main window know to unlock menu
		LOG(logINFO) << MESSAGE_THREAD_SHUTDOWN_AUTO;

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
	if (storage.experimentStatus.experimentInProgress)
	{
		storage.timerWaiting.Pause();
		storage.experimentStatus.experimentRecording = false;

		// Log the pause
		LOG(logINFO) << MESSAGE_EXPPAUSE;
	}
	storage.timerRecording.Pause();
	storage.experimentStatus.experimentCommandsRequested = false;
}


void Automation::Resume()
{
	if (storage.experimentStatus.experimentInProgress)
	{
		storage.timerWaiting.Resume();
		storage.experimentStatus.experimentRecording = true;

		// Log the resume
		LOG(logINFO) << MESSAGE_EXPRESUME;
	}
	storage.timerRecording.Resume();
	storage.experimentStatus.experimentCommandsRequested = true;
}