#pragma once

#define finl "\r\n"

// REQUIRED INCLUDES

#include "Classes_experiences.h"			// Resource where templates for all the data are stored, might be worth replacing with a single data type/class 
#include "StringTable.h"					// Definitions for the text in the messages

// Defines
#include "DefineAutomationSettings.h"		// All settings for automation are stored here
#include "DefineInstruments.h"				// All instruments definitions are here
#include "DefineStages.h"					// All stage, experiment type definitions are here

// Synchronization classes
#include "MFCMessageHandler.h"				// Handles all the messages from this class to the UI. Great if we want to upgrade to somehting that's not MFC
#include "ExperimentData.h"
#include "ExperimentSettings.h"	

// Measurement and manipulation classes
#include "Vannes.h"					// Controlling valves
#include "Temperature.h"			// Temperature recording
#include "SerialInstruments.h"		// Pressure & Calorimeter recording

#include "Chrono.h"					// Time keeping

// std::functionality
#include <sstream>
#include <atomic>

class Automation
{
public:
	Automation(ExperimentSettings* exps);
	~Automation();


/**********************************************************************************************************************************
// Global Variables
***********************************************************************************************************************************/

protected:

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------

	// Instruments
	CVannes* g_pVanne;									// Pointer to the valve opening class
	CTemperature* g_pTemperature;						// Pointer to the class that deals with temperature recording
	SerialInstruments * g_pSerialInstruments;			// Pointer to the class that deals with calorimeter & pressure recording
	ExperimentSettings* experimentSettings;				// Pointer to the experiment settings from the main class, this is only read, never written

	//------------------------------------------------------------
	// Locally stored settings and data
	//------------------------------------------------------------

	ExperimentSettings experimentLocalSettings;			// Pointer to local storage of settings
	ExperimentData experimentLocalData;					// 
	
	
	//------------------------------------------------------------
	// Objects 
	//------------------------------------------------------------
	
	ofstream fileStream;								// The file stream is stored in this variable

	MFCMessageHandler messageHandler;					// This class will send all the messages to the GUI using MFC's message pump

	CChrono timerExperiment;							// Class for measuring the time from the experiment start
	CChrono timerMeasurement;							// Class for measuring the time between each measurement
	CChrono timerWaiting;								// Class for measuring the time to wait
	
														
	//------------------------------------------------------------
	// Syncronisation primitives and threads
	//------------------------------------------------------------

	CRITICAL_SECTION criticalSection;					// Critical section for measurement thread sinchronisation

	// Events
	HANDLE h_MeasurementThreadStartEvent;				// Handle event doing measurement thread signalling
	
	HANDLE events[5];									// Keeps all the events below in one array
public:
	std::atomic_bool sb_settingsModified;				// Atomic variable that can be set to let the thread know that there are new experiment settings 
	std::atomic_bool sb_userContinue;						

	HANDLE h_eventShutdown;								// Handle event shutting down the thread
	HANDLE h_eventResume;								// Handle event resuming the thread
	HANDLE h_eventPause;								// Handle event pausing the thread
	HANDLE h_eventReset;								// Handle event resetting the thread for a new experiment
	HANDLE h_eventUserInput;							// Handle event waiting for the user to do something

protected:
	// Threads
	HANDLE h_MeasurementThread[4];						// Threads for measurement

	//------------------------------------------------------------
	// Standards data types 
	//------------------------------------------------------------


	// Case switch int for the experiment states (running/paused/etc)
	bool running;
	bool paused;
	int shutdownReason;


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
// Security measures and checks
***********************************************************************************************************************************/

protected:

	bool securityActivated;

	// Keep limits in memory
	float security_PressureHigh_HighRange;
	float security_PressureHigh_LowRange;

	// Bool flags to keep track of security
	bool security_PressureHigh_flag;
	bool security_TemperatureHigh_flag;
	bool security_TemperatureLow_flag;

	void InitialisationSecurity();

	void SecuriteHautePression();
	void SecuriteHautePressionManuelle();
	void SecuriteHautePressionAuto();

	void SecuriteTemperatures();
	void SecuriteTemperaturesManuelle();
	void SecuriteTemperaturesAuto();


/**********************************************************************************************************************************
// Time keeping
***********************************************************************************************************************************/

protected:

	void WaitMinutes(int nbminutes);
	void WaitSeconds(int nbsecondes);



/**********************************************************************************************************************************
*
// File Writing Functions
*
***********************************************************************************************************************************/

protected:

	/**********************************************************************
	* Write the first section of an entete and save it
	* Inputs: none
	***********************************************************************/
	void EnteteCreate();

	/**********************************************************************
	* Write the first section of an CSV entete and save it
	* Inputs: none
	***********************************************************************/
	void EnteteCSVCreate();

	/**********************************************************************
	* Opens the measurement file for the first time and stores its link in the fileStream ofstream
	* Also writes columns in the CSV
	* Inputs: none
	***********************************************************************/
	void FileMeasurementOpen();

	/**********************************************************************
	* Closes the measurement file
	* Inputs: none
	***********************************************************************/
	void FileMeasurementClose();
	
	/**********************************************************************
	* Records a measurement
	* Inputs: none
	***********************************************************************/
	void FileMeasurementRecord();



	/**********************************************************************
	* Writes the base of the entete
	* Inputs:
	*        bool csv: Ask for a comma separated value format if true
	***********************************************************************/
	std::string EnteteBase(bool csv);

	/**********************************************************************
	* Writes the settings from the general tab
	* Inputs:
	*        bool csv: Ask for a comma separated value format if true
	***********************************************************************/
	std::string EnteteGeneral(bool csv);

	/**********************************************************************
	* Writes the settings from the diverse tab
	* Inputs:
	*        bool csv: Ask for a comma separated value format if true
	***********************************************************************/
	std::string EnteteDivers(bool csv);
	
	/**********************************************************************
	* Writes the adsorption steps
	* Inputs:
	*        bool csv: Ask for a comma separated value format if true
	***********************************************************************/
	std::string EnteteAdsorption(bool csv);

	/**********************************************************************
	* Writes the desorption steps
	* Inputs:
	*        bool csv: Ask for a comma separated value format if true
	***********************************************************************/
	std::string EnteteDesorption(bool csv);


	/**********************************************************************
	* Writes any settings changes into the entete files
	* Inputs:
	*        ExperimentSettings newSettings: The settings file to compare to the current one
	*        bool csv: Ask to write to the comma separated value file if true
	***********************************************************************/
	void RecordDataChange(const ExperimentSettings& newSettings, bool csv);


	/**********************************************************************
	* Returns the full path and title of the file to be written
	* Inputs:
	*        string extension: Extension you want the file to have
	*        bool entete: specify true to get the entete string or false for the regular file
	***********************************************************************/
	std::string BuildFileName(std::string extension, bool entete);

/**********************************************************************************************************************************
// Measurements functions and threads
***********************************************************************************************************************************/
		
	void ThreadMeasurement();

	static DWORD WINAPI ThreadProc_ReadCalorimeter(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_ReadHighPressure(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_ReadLowPressure(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_ReadTemperature(LPVOID lpParam);

	void ReadCalorimeter();
	void ReadLowPressure();
	void ReadHighPressure();
	void ReadTemperatures();


/**********************************************************************************************************************************
// Valve and control mechanism functions
***********************************************************************************************************************************/

	bool ValveOpen(int num_vanne);
	bool ValveClose(int num_vanne);
	bool ValveOpenClose(int num_vanne);
	bool ValvesCloseAll();

	bool EVActivate(int num_ev);
	bool EVDeactivate(int num_ev);

	bool ActivatePump();
	bool DeactivatePump();

	bool ValvesAndPumpClose();
	bool ControlMechanismsCloseAll();

	bool EstOuvert_Vanne(int num_vanne);
	bool EstFerme_Vanne(int num_vanne);
	bool EV1EstActive();
	bool EV1EstDesactive();
	bool EV2EstActive();
	bool EV2EstDesactive();
	bool PompeEstActive();
	bool PompeEstDesactive();


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

