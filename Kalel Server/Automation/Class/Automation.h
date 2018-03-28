#pragma once

// REQUIRED INCLUDES

#include "../../../Kalel Shared/Com Classes/ExperimentHelperClasses.h"		// Resource where templates for all the data are stored

// Defines
#include "../../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineInstruments.h"				// All instruments definitions are here
#include "../../../Kalel Shared/Resources/DefineStages.h"					// All stage, experiment type definitions are here

// Synchronization classes
#include "../CommonControls.h"
#include "../CommonStorage.h"


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
	// References
	//------------------------------------------------------------
	Storage & storage;
	Controls & controls;
														
	//------------------------------------------------------------
	// Syncronisation primitives
	//------------------------------------------------------------
protected:

	bool running = true;
	std::atomic_bool waitingUser = false;

public:
	std::atomic<Stop> shutdownReason = Stop::Cancel;
	std::atomic_uint userChoice;							// Atomic uint for user choice

	std::atomic_bool eventPause = false;					// Bool for pausing the thread
	std::atomic_bool eventResume = false;					// Bool for resuming the thread
	std::atomic_bool eventShutdown = false;					// Bool for shutting down the thread
	std::atomic_bool eventUserInput = false;				// Bool for waiting for the user to do something
	std::atomic_bool eventChangeExpSett = false;			// Bool for changing experiment settings


/**********************************************************************************************************************************
// Functions main cpp
***********************************************************************************************************************************/
public:
	void Execution();

protected:
	//------------------------------------------------------------
	// Execution
	//------------------------------------------------------------

	void ExecutionManual();
	void ExecutionAuto();
	void ExecutionContinuous();

	//------------------------------------------------------------
	// Initialisation
	//------------------------------------------------------------

	void ResetAutomation();


/**********************************************************************************************************************************
// Time keeping
***********************************************************************************************************************************/

protected:

	void WaitMinutes(int nbminutes, bool verbose = false);
	void WaitSeconds(int nbsecondes, bool verbose = false);


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
	void StageContinuous();
	void StageVacuum(bool separateFunctionality = false);


	/**********************************************************************************************************************************
	// Vacuum
	***********************************************************************************************************************************/

	void SampleVacuum();
	void BottleVacuum();


};

