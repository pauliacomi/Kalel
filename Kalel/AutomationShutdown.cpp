#include "StdAfx.h"
#include "Automation.h"



void Automation::Shutdown()
{
	switch (shutdownReason)
	{

	case STOP_CANCEL:		// This cancels an experiment in progress, GUI must ask check for experiment running
		
		//When thread finishes, let main window know to unlock menu and reset graph
		messageHandler.DisplayMessage(MESSAGE_EXPCANCEL);		// Experiment has been cancelled
		messageHandler.ExperimentEnd();

		// Close measurement file
		FileMeasurementClose();

		// Stop all timers 
		timerExperiment.ArretTemps();
		timerMeasurement.ArretTemps();
		timerWaiting.ArretTemps();

		// Reset the event
		::ResetEvent(h_eventReset);

		// Run reset funtion
		ResetAutomation();

		break;

	case STOP_NORMAL:		// This option is used if the experiment finishes correctly
							// It then resets everything

		//When thread finishes, let main window know to unlock menu
		messageHandler.ExperimentEnd();

		// Close measurement file
		FileMeasurementClose();

		// Stop all timers 
		timerExperiment.ArretTemps();
		timerMeasurement.ArretTemps();
		timerWaiting.ArretTemps();

		// Reset all data from the experiment
		experimentLocalData.ResetData();

		// Experiment has been finished normally
		messageHandler.DisplayMessage(MESSAGE_EXPFINISH);

		// Reset the event
		::ResetEvent(h_eventReset);

		break;

	case STOP_COMPLETE:		// This option is used if the automation thread is to be closed

		//When thread finishes, let main window know to unlock menu
		messageHandler.ExperimentEnd();

		// Close measurement file
		FileMeasurementClose();

		// Experiment has been finished normally
		messageHandler.DisplayMessage(MESSAGE_THREAD_SHUTDOWN);

		break;

	default:
		ASSERT(0);		// Error, should never be reached
		break;
	}
}


void Automation::Pause()
{
	if (experimentLocalData.experimentInProgress)
	{
		timerWaiting.ArretTemps();
		experimentLocalData.experimentRecording = false;

		messageHandler.DisplayMessage(MESSAGE_EXPPAUSE);
	}
	timerExperiment.ArretTemps();
	timerMeasurement.ArretTemps();
	experimentLocalData.experimentCommandsRequested = false;
}


void Automation::Resume()
{
	if (experimentLocalData.experimentInProgress)
	{
		timerWaiting.RepriseTemps();
		experimentLocalData.experimentRecording = true;

		messageHandler.DisplayMessage(MESSAGE_EXPRESUME);
	}
	timerExperiment.RepriseTemps();
	timerMeasurement.RepriseTemps();
	experimentLocalData.experimentCommandsRequested = true;
}