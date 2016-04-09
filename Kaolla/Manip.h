#pragma once
#ifndef MANIP_H
#define MANIP_H

#define finl "\r\n"


// REQUIRED INCLUDES
#include "MessageBoxTexts.h"		// Resource for the text. Ideally only a variable that describes the error should be returned, but this is a compromise
#include "MFCMessageHandler.h"		// Handles all the messages from this class to the UI. Great if we want to upgrade to somehting that's not MFC
#include "Classes_experiences.h"	// Resource where templates for all the data are stored, might be worth replacing with a single data type/class 


// CHECK THESE INCLUDES

#include "Kaolla.h"
#include "KaollaDoc.h"

#include "KaollaView.h"

#include "time.h"

#include "Keithley.h"
#include "Mensor.h"
#include "Instrument.h"
#include "Temperature.h"
#include "Parametres.h"
#include "FluxConverter.h"
#include "ConnectionMesure.h"

#include "Vannes.h"

#include "Parametres_experience.h"

#include "Chrono.h"

#include <string>
#include <sstream>  //ostringstream
#include <fstream>


class CManip
{
	// ------------------ Variables ------------------

public:

	// Pointers to classes that will be used

	Keithley* g_pKeithley;								// Pointer to the class that deals with the Keithley
	Mensor* g_pMensor;									// Pointer to the class that deals with the Mensor
	CVannes* g_pVanne;									// Pointer to the valve opening class
	CInstrument* instrument[NB_OF_INSTRUMENTS];			// Array of pointers that hold CInstrument classes
	CTemperature* g_pTemperature;						// Pointer to the class that deals with temperature recording

	CProprietes_experience* m_proprietes_experience;	// Pointer to the dialog SHOULD REALLY REMOVE THIS

	//------------------------------------------------------------
	// Objects instantiated here
	//------------------------------------------------------------

	ofstream fileStream;								// The file stream is stored in this variable

	MFCMessageHandler messageHandler;					// This class will send all the messages to the GUI using MFC's message pump

	ConnectionMesure AppareilCalo;						// Stores where each instrument index and number is
	ConnectionMesure AppareilHP;						// Stores where each instrument index and number is
	ConnectionMesure AppareilBP;						// Stores where each instrument index and number is

	CChrono temps_experience;							// Deals with 
	CChrono temps_intermediaire;						//

	FluxConverter fluxConverter;						//
	Donnees_General general;							//
	Donnees_Divers divers;								//

	// Other variables

	int synchCalo, synchHP, synchBP;
	bool manip_en_cours;						// TRUE if the manipulation is running, might be replaced by main window flag
	double temps_manip;                         // Calculation of time
	
	float TemperatureCalo;						// Result of the USB 9211 for the temperature
	float TemperatureCage;						// Result of the USB 9211 for the temperature
    float TemperaturePiece;						// Result of the USB 9211 for the temperature

	double resultat_calo;						//
	double resultat_bp;							//
	double resultat_hp;							//

	int numero_mesure;							//
	int TypeExperience;							//
	int GenreExperience;						//

	// Manip_Auto

	int demande_arret;							//
	int etape_en_cours;							//
	int etape_perturbe;							//

protected:
	DWORD ThreadId;								//
	HANDLE hThread[3];							//

	/// from manipSecurity

	bool PressionTropHaute; //SecuriteHautePressionManuelle()
	bool TemperatureElevee; //SecuriteTemperatureManuelle
	bool TemperatureFaible;

	HANDLE hEventPression, hEventTemperatureElevee, hEventTemperatureFaible;

	// from manipManualExecution

	CEvent g_eventFinAffichage;

	// from manip Measurement

	CRITICAL_SECTION Sync_view_instrument[NB_OF_INSTRUMENTS + 1];
	HANDLE hEvent;


	// ------------------ Functions ------------------

public:

	// Manip

	CManip(void);
	~CManip(void);
	void SetKeithley(Keithley* Keith);
	void SetMensor(Mensor* Mens);
	void SetVannes(CVannes* pVannes);
	void SetTemperature(CTemperature* pTemperature);

	void InitialisationInstruments();
	void InitialisationManip();

	void SetManipType(int experimentType);
	void ReinitialisationManuelle();

/**********************************************************************************************************************************
// Data interface functions
***********************************************************************************************************************************/

public:
	void AffichageMesures();
	void GraphAddMeasurement();
	void RecuperationDonnees();
	CString GetDonneesExperience();
	void DonneesManuelleGrapheEtape();

protected:
	Donnees_General DonneesActuellesGeneral();
	Donnees_Divers DonneesActuellesDivers();
	Donnees_Petites_Doses DonneesActuellesPetitesDoses();
	Donnees_Grandes_Doses DonneesActuellesGrandesDoses();
	Donnees_Desorption DonneesActuellesDesorption();
	Donnees_Adsorption_Continue DonneesActuellesAdsorptionContinue();
	
	
	// Manip_ExecutionManuelle

public:
	void ExecutionManuelle(LPVOID pParam);
	void FinAffichage();


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


/**********************************************************************************************************************************
// Instrument interfaces for measurement
***********************************************************************************************************************************/

protected :
	// Call openPort for each instrument
	void OuvrirInstruments();

	// Call closePort for each instrument
	void FermerInstruments();

	//Returns the value that is being read from the instrument passed in
	double LectureMesure(ConnectionMesure Appareil);

	// Reads the calorimeter and stores it in the coresponding variable
	void LectureCalo();

	// Reads the low pressure and stores it in the coresponding variable
	void LectureHautePression();
	// Reads the high pressure and stores it in the coresponding variable
	void LectureBassePression();
	// Reads the temperatures and stores it in the coresponding variable
	void LectureTemperatures();

	// Starts the threads for reading the data
	void ThreadMesures();

	// Threads for reading the data
	static DWORD WINAPI ThreadProc_LectureCalo(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_LectureHautePression(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_LectureBassePression(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_LectureTemperature(LPVOID lpParam);


/**********************************************************************************************************************************
// Security measures and checks
***********************************************************************************************************************************/

protected:

	void InitialisationMesureSecuriteManuelle();
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

public:

	void AttenteMinutes (int nbminutes);
	void AttenteSecondes (int nbsecondes);
	CString MessageAttente(int nbsecondes);
	CString MessageTemps(int duree);


/**********************************************************************************************************************************
// Valve operations
***********************************************************************************************************************************/

public:
	bool OuvrirEtFermer_Vanne(int num_vanne);
	bool Ouverture_Vanne(int num_vanne);
	bool Fermeture_Vanne(int num_vanne);
	bool FermetureDeToutesLesVannes();
	bool ActiverEV(int num_ev);
	bool DesactiverEV(int num_ev);
	bool ActiverPompe();
	bool DesactiverPompe();
	bool FermerLesValvesEtLaPompe();
	bool ToutFermer();

	bool EstOuvert_Vanne(int num_vanne);
	bool EstFerme_Vanne(int num_vanne);

	bool EV1EstActive();
	bool EV1EstDesactive();
	bool EV2EstActive();
	bool EV2EstDesactive();
	bool PompeEstActive();
	bool PompeEstDesactive();

};

#endif