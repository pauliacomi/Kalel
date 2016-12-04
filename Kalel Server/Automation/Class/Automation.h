#pragma once

#include "../../stdafx.h"

// REQUIRED INCLUDES

#include "../../../Kalel Shared/Com Classes/ExperimentHelperClasses.h"		// Resource where templates for all the data are stored

// Defines
#include "../../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineAutomationSettings.h"		// All settings for automation are stored here
#include "../../../Kalel Shared/Resources/DefineInstruments.h"				// All instruments definitions are here
#include "../../../Kalel Shared/Resources/DefineStages.h"					// All stage, experiment type definitions are here

// Synchronization classes
#include "../../MessageHandler.h"											// Handles all the messages from this class to the client
#include "../../../Kalel Shared/Com Classes/ExperimentData.h"
#include "../../../Kalel Shared/Com Classes/ExperimentSettings.h"	

// Measurement and manipulation classes
#include "../../Backend/Wrapper Classes/ValveController.h"					// Controlling valves

#include "../Utils/Chrono.h"												// Time keeping


#include "../CommonControls.h"
#include "../CommonPointers.h"

// std::functionality
#include <atomic>

class Automation
{
public:
	Automation(Storage &s, Controls &c);
	~Automation();


/**********************************************************************************************************************************
// Global Variables
***********************************************************************************************************************************/

protected:

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------
	Storage & storage;
	Controls & controls;	
														
	//------------------------------------------------------------
	// Syncronisation primitives and threads
	//------------------------------------------------------------
protected:

	// Events
	HANDLE events[5];									// Keeps all the events below in one array

	// Case switch int for the experiment states (running/paused/etc)
	volatile int shutdownReason;
	std::atomic_bool running = true;

public:
	std::atomic_bool sb_settingsModified;				// Atomic variable that can be set to let the thread know that there are new experiment settings 
	std::atomic_bool sb_userContinue;						

	HANDLE h_eventShutdown;								// Handle event shutting down the thread
	HANDLE h_eventResume;								// Handle event resuming the thread
	HANDLE h_eventPause;								// Handle event pausing the thread
	HANDLE h_eventReset;								// Handle event resetting the thread for a new experiment
	HANDLE h_eventUserInput;							// Handle event waiting for the user to do something


/**********************************************************************************************************************************
// Functions main cpp
***********************************************************************************************************************************/
public:
	void Execution();

protected:
	//------------------------------------------------------------
	// Set Data
	//------------------------------------------------------------
	ExperimentSettings GetSettings();

	//------------------------------------------------------------
	// Execution
	//------------------------------------------------------------

	bool ExecutionManual();
	bool ExecutionAuto();

	//------------------------------------------------------------
	// Initialisation
	//------------------------------------------------------------

	void ResetAutomation();


/**********************************************************************************************************************************
// Time keeping
***********************************************************************************************************************************/

protected:

	void WaitMinutes(int nbminutes);
	void WaitSeconds(int nbsecondes);


	/**********************************************************************************************************************************
	// Checks for automatic functionality
	***********************************************************************************************************************************/

	void Verifications();

	bool VerificationSecurity();
	bool VerificationValves();
	bool VerificationResidualPressure();
	bool VerificationTemperature();
	bool VerificationComplete();

	/**********************************************************************************************************************************
	// Functions for shutdown checks
	***********************************************************************************************************************************/

	void Shutdown();

	void Pause();

	void Resume();


	/**********************************************************************************************************************************
	// Steps
	***********************************************************************************************************************************/

	void StageEquilibration();
	void StageAdsorption();
	void SubstepsAdsorption();
	void StageDesorption();
	void SubstepsDesorption();
	void StageVacuum(bool separateFunctionality = false);


	/**********************************************************************************************************************************
	// Vacuum
	***********************************************************************************************************************************/

	void SampleVacuum();
	void BottleVacuum();

};

