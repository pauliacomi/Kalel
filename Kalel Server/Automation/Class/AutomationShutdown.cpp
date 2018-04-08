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

		// Close all valves
		controls.valveControls.CloseAll(true);

		// Reset all data from the experiment
		storage.experimentSettings.ResetData();

		// Run reset funtion
		ResetAutomation();

		break;

	case Stop::Normal:			// This option is used if the experiment finishes correctly
								// It then resets everything

		LOG(logINFO) << MESSAGE_EXPFINISH;

		// Close all valves
		controls.valveControls.CloseAll(true);

		// Reset all data from the experiment
		storage.experimentSettings.ResetData();

		// Run reset funtion
		ResetAutomation();

		break;

	case Stop::Complete:		// This option is used if the automation thread is to be closed

		// When thread finishes, let main window know to unlock menu
		LOG(logINFO) << MESSAGE_THREAD_SHUTDOWN_AUTO;

		// Close all valves
		controls.valveControls.CloseAll(true);

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
	if (storage.experimentStatus.inProgress)
	{
		storage.timerWaiting.Pause();

		// Log the pause
		LOG(logINFO) << MESSAGE_EXPPAUSE;
	}
	storage.timerRecording.Pause();
	storage.experimentStatus.isRunningAutomation = false;
}


void Automation::Resume()
{
	if (storage.experimentStatus.inProgress)
	{
		storage.timerWaiting.Resume();

		// Log the resume
		LOG(logINFO) << MESSAGE_EXPRESUME;
	}
	storage.timerRecording.Resume();
	storage.experimentStatus.isRunningAutomation = true;
}