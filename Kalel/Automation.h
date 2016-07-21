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

class Automation
{
public:
	Automation(ExperimentSettings* exps);
	~Automation();


/**********************************************************************************************************************************
// Global Variables
***********************************************************************************************************************************/

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------


	// Instruments
	CVannes* g_pVanne;									// Pointer to the valve opening class
	CTemperature* g_pTemperature;						// Pointer to the class that deals with temperature recording
	SerialInstruments * g_pSerialInstruments;			// Pointer to the class that deals with calorimeter & pressure recording

	ExperimentSettings* experimentSettings;				//
	ExperimentSettings experimentLocalSettings;			// 
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
	
	HANDLE events[4];									// Keeps all the events below in one array
	HANDLE h_eventShutdown;								// Handle event shutting down the thread
	HANDLE h_eventResume;								// Handle event resuming the thread
	HANDLE h_eventPause;								// Handle event pausing the thread
	HANDLE h_eventReset;								// Handle event resetting the thread for a new experiment

	// Threads
	HANDLE h_MeasurementThread[4];						// Threads for measurement

	//------------------------------------------------------------
	// Standards data types 
	//------------------------------------------------------------


	// Case switch int for the experiment states (running/paused/etc)
	bool checking;
	bool running;
	bool paused;
	int shutdownReason;


/**********************************************************************************************************************************
// Functions main cpp
***********************************************************************************************************************************/
	//------------------------------------------------------------
	// Set Data
	//------------------------------------------------------------

public:
	void SetData();
	bool DataIsNew();
	void Execution();

private:
	//------------------------------------------------------------
	// Execution
	//------------------------------------------------------------

	bool ExecutionManual();
	bool ExecutionAuto();

	//------------------------------------------------------------
	// Initialisation
	//------------------------------------------------------------

	void Initialisation();

	//------------------------------------------------------------
	// Termination
	//------------------------------------------------------------

	void DeInitialise();


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
// File Writing Functions
***********************************************************************************************************************************/

protected:

	/**********************************************************************
	Returns the full path and name of the written file or its associated entete
	* Inputs:
	*        string extension: Extension you want the file to have
	*        bool entete: specify true to get the entete string or false for the regular file
	***********************************************************************/
	std::string NomFichier(std::string extension, bool entete);

	// Opens the measurement file for the first time and stores its link in the fileStream ofstream
	void FileMeasurementOpen();
	// Closes the measurement file
	void FileMeasurementClose();
	// Records a measurement
	void EnregistrementFichierMesures();

	// Write the start of an entete
	std::string EnteteBase();
	// Write the start of the CSV entete
	std::string EnteteBaseCSV();
	// Write the main body of an entete
	std::string EnteteGeneral();
	// Write the main body of a CSV entete
	std::string EnteteGeneralCSV();

	// Write the start of an entete and save it??
	void EcritureEntete();
	// Write the start of a CSV entete and save it??
	void EcritureEnteteCSV();

	void RecordDataChange();

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

	int VerificationSecurity();

	int VerificationValves();

	int VerificationResidualPressure();

	int VerificationTemperature();

	int VerificationComplete();

	/**********************************************************************************************************************************
	// Functions for shutdown checks
	***********************************************************************************************************************************/

	void Shutdown();


	/**********************************************************************************************************************************
	// Steps
	***********************************************************************************************************************************/

	void StageEquilibration();
	void StageAdsorption();
	void SubstepsAdsorption();
	void StageDesorption();
	void SubstepsDesorption();


	/**********************************************************************************************************************************
	// Vacuum
	***********************************************************************************************************************************/

	void SampleVacuum();
	void BottleVacuum();

};

