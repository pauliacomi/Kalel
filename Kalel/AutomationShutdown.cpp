#include "StdAfx.h"
#include "Automation.h"



void Automation::Shutdown()
{
	switch (shutdownReason)
	{

	case STOP_CANCEL:		// This option is used if the experiment is cancelled
							// It then resets everything

		//messageHandler.DisplayMessageBox(PROMPT_CANCELEXP, MB_ICONQUESTION | MB_OKCANCEL, true);
		//::SetEvent(h_eventPause);

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

		// Experiment has been cancelled
		messageHandler.DisplayMessage(MESSAGE_EXPCANCEL);

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

		break;

	case STOP_COMPLETE:		// This option is used if the application is closed
							// It then resets everything

		//When thread finishes, let main window know to unlock menu
		messageHandler.ExperimentEnd();

		// Close measurement file
		FileMeasurementClose();

		// Experiment has been finished normally
		CString tempString = _T("Thread completely shutdown");
		messageHandler.DisplayMessage(tempString);

		break;

	default:
		ASSERT(0);		// Error, should never be reached
		break;
	}
}