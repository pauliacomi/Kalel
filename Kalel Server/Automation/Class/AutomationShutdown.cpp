#include "Automation.h"



void Automation::Shutdown()
{
	switch (shutdownReason)
	{

	case STOP_CANCEL:		// This cancels an experiment in progress, GUI must ask check for experiment running
		
		//When thread finishes, let main window know to unlock menu and reset graph
		controls.messageHandler->DisplayMessage(MESSAGE_EXPCANCEL);		// Experiment has been cancelled
		controls.messageHandler->ExperimentEnd();

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
		controls.messageHandler->DisplayMessage(MESSAGE_EXPFINISH);		// Experiment has been finished normally
		controls.messageHandler->ExperimentEnd();

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
		controls.messageHandler->ThreadShutdown();

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

		controls.messageHandler->DisplayMessage(MESSAGE_EXPPAUSE);
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

		controls.messageHandler->DisplayMessage(MESSAGE_EXPRESUME);
	}
	controls.timerExperiment.Resume();
	controls.timerMeasurement.Resume();
	storage.currentData->experimentCommandsRequested = true;
}