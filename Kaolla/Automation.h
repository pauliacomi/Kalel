#pragma once

// REQUIRED INCLUDES

#include "Classes_experiences.h"	// Resource where templates for all the data are stored, might be worth replacing with a single data type/class 
#include "ConnectionMesure.h"		// 
#include "MessageBoxTexts.h"		// Definitions for the text in the messages

// Synchronization classes
#include "MFCMessageHandler.h"		// Handles all the messages from this class to the UI. Great if we want to upgrade to somehting that's not MFC
#include "ExperimentData.h"

// Measurement and manipulation classes
#include "Keithley.h"				// Keithley
#include "Mensor.h"					// Mensor
#include "Vannes.h"					// Valves
#include "Instrument.h"				// Instruments
#include "Temperature.h"			// Temperature recording
#include "Chrono.h"					// Time keeping


class Automation
{
public:
	Automation();
	~Automation();


/**********************************************************************************************************************************
// Variables
***********************************************************************************************************************************/

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------

	Keithley* g_pKeithley;								// Pointer to the class that deals with the Keithley
	Mensor* g_pMensor;									// Pointer to the class that deals with the Mensor
	CVannes* g_pVanne;									// Pointer to the valve opening class
	CInstrument* instrument[NB_OF_INSTRUMENTS];			// Array of pointers that hold CInstrument classes
	CTemperature* g_pTemperature;						// Pointer to the class that deals with temperature recording

	ExperimentData* experimentData;						// Pointer to the class that deals with temperature recording
	
	
	//------------------------------------------------------------
	// Objects 
	//------------------------------------------------------------

	MFCMessageHandler messageHandler;					// This class will send all the messages to the GUI using MFC's message pump

	ConnectionMesure AppareilCalo;						// Stores where each instrument index and number is
	ConnectionMesure AppareilHP;						// Stores where each instrument index and number is
	ConnectionMesure AppareilBP;						// Stores where each instrument index and number is

	CChrono timerExperiment;							// Class for measuring the time from the experiment start
	CChrono timerMeasurement;							// Class for measuring the time between each measurement
	
	//------------------------------------------------------------
	// Standards data types 
	//------------------------------------------------------------

	// Synchronisation
	int synchCalo;
	int synchHP;
	int synchBP;

	CString message;									// Used to store the message before passing it


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
	void SetData(ExperimentData * eData);

	//------------------------------------------------------------
	// Execution
	//------------------------------------------------------------

	void Execution();
	void ExecutionManual();
	void ExecutionAuto();

	void Initialisation();
	void InitialisationInstruments();


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

	static DWORD WINAPI MesureSecuriteHautePression(LPVOID lpParam);
	static DWORD WINAPI MesureSecuriteTemperatureElevee(LPVOID lpParam);
	static DWORD WINAPI MesureSecuriteTemperatureFaible(LPVOID lpParam);

/**********************************************************************************************************************************
// Time keeping
***********************************************************************************************************************************/

protected:

	void AttenteMinutes(int nbminutes);
	void AttenteSecondes(int nbsecondes);
	CString MessageAttente(int nbsecondes);
	CString MessageTemps(int duree);


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
	void OuvertureFichierMesures();
	// Closes the measurement file
	void FermetureFichierMesures();
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

};

