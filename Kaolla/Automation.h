#pragma once

#define finl "\r\n"

// REQUIRED INCLUDES

#include "Classes_experiences.h"			// Resource where templates for all the data are stored, might be worth replacing with a single data type/class 
#include "ConnectionMesure.h"				// 
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
#include "Keithley.h"				// Keithley
#include "Mensor.h"					// Mensor
#include "Vannes.h"					// Valves
#include "Instrument.h"				// Instruments
#include "Temperature.h"			// Temperature recording
#include "Chrono.h"					// Time keeping
#include "FluxConverter.h"			// Conversions

// std::functionality
#include <sstream>			

class Automation
{
public:
	Automation();
	~Automation();


/**********************************************************************************************************************************
// Global Variables
***********************************************************************************************************************************/

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------

	Keithley* g_pKeithley;								// Pointer to the class that deals with the Keithley
	Mensor* g_pMensor;									// Pointer to the class that deals with the Mensor
	CVannes* g_pVanne;									// Pointer to the valve opening class
	CInstrument* instrument[NB_OF_INSTRUMENTS];			// Array of pointers that hold CInstrument classes
	CTemperature* g_pTemperature;						// Pointer to the class that deals with temperature recording

	ExperimentSettings* experimentSettings;				//
	ExperimentSettings experimentLocalSettings;			// 

	ExperimentData experimentLocalData;					// 
	
	
	//------------------------------------------------------------
	// Objects 
	//------------------------------------------------------------
	ofstream fileStream;								// The file stream is stored in this variable

	MFCMessageHandler messageHandler;					// This class will send all the messages to the GUI using MFC's message pump

	ConnectionMesure AppareilCalo;						// Stores where each instrument index and number is
	ConnectionMesure AppareilHP;						// Stores where each instrument index and number is
	ConnectionMesure AppareilBP;						// Stores where each instrument index and number is

	CChrono timerExperiment;							// Class for measuring the time from the experiment start
	CChrono timerMeasurement;							// Class for measuring the time between each measurement
	CChrono timerWaiting;								// Class for measuring the time to wait
	
	FluxConverter fluxConverter;						// Will convert between several types of raw results from instruments
														
	//------------------------------------------------------------
	// Syncronisation primitives and threads
	//------------------------------------------------------------

	CRITICAL_SECTION criticalSection;					// Critical section for measurement thread sinchronisation

	// Events
	HANDLE h_MeasurementThreadStartEvent;				// Handle event doing measurement thread signalling
	HANDLE h_eventShutdown;								// Handle event shutting down the thread
	HANDLE h_eventResume;								// Handle event resuming the thread
	HANDLE h_eventPause;								// Handle event pausing the thread
	HANDLE h_eventReset;								// Handle event resetting the thread for a new experiment

	// Threads
	HANDLE h_MeasurementThread[4];						// Threads for measurement

	//------------------------------------------------------------
	// Standards data types 
	//------------------------------------------------------------

	// Bools to keep track of security
	bool security_PressureHigh;
	bool security_TemperatureHigh;
	bool security_TemperatureLow;

	// Case switch int for the experiment states (running/paused/etc)
	bool checking;
	bool running;
	bool paused;
	int shutdownReason;

	// Synchronisation?
	int synchCalo;
	int synchHP;
	int synchBP;



/**********************************************************************************************************************************
// Functions main cpp
***********************************************************************************************************************************/
	//------------------------------------------------------------
	// Set Data
	//------------------------------------------------------------

	void SetKeithley(Keithley * Keith);
	void SetMensor(Mensor * Mens);
	void SetVannes(CVannes * pVannes);
	void SetTemperature(CTemperature * pTemperature);
	void SetDataPointer(ExperimentSettings * eSettings);

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
	void InitialisationInstruments();
	void InstrumentsOpen();

	//------------------------------------------------------------
	// Termination
	//------------------------------------------------------------

	void InstrumentsClose();
	void DeInitialise();


/**********************************************************************************************************************************
// Security measures and checks
***********************************************************************************************************************************/

protected:

	void InitialisationSecurityManual();

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

	double ReadMeasurementFromDevice(ConnectionMesure Appareil);


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

