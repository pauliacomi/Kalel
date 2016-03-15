#pragma once
#ifndef MANIP_H
#define MANIP_H

#define finl "\r\n" 

#include "MessageBoxTexts.h"	// Resource for the text. Ideally only a variable that describes the error should be returned, but this is a compromise

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

	CKaollaView* m_KaollaView;							// Pointer to the main view
	CKaollaDoc* m_Doc;									// Pointer to the main document
	Keithley* g_pKeithley;								// Pointer to the class that deals with the Keithley
	Mensor* g_pMensor;									// Pointer to the class that deals with the Mensor
	CVannes* g_pVanne;									// Pointer to the valve opening class
	CInstrument* instrument[NB_OF_INSTRUMENTS];			// Array of pointers that hold CInstrument classes
	CTemperature* g_pTemperature;						// Pointer to the class that deals with temperature recording
	CProprietes_experience* m_proprietes_experience;	// Pointer to the valve opening class

	// Objects instantiated here

	ConnectionMesure AppareilCalo, AppareilHP, AppareilBP;
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


	// ------------------ Functions ------------------

public:

	// Manip

	CManip(void);
	~CManip(void);
	void SetKaollaView(CKaollaView* pKV);
	void SetKeithley(Keithley* Keith);
	void SetMensor(Mensor* Mens);
	void SetVannes(CVannes* pVannes);
	void SetTemperature(CTemperature* pTemperature);

	void InitialisationInstruments();
	void InitialisationManip();

	void SetManipType(int experimentType);
	void ReinitialisationManuelle();

protected:
	void MiseAJour();

	void DebloqueMenu();
	void RemettreBoutonLancer();



	// Manip_Affichage

public:
	void AffichageMesures();
	void AffichageMesures(CString rajout_mesure);

protected:
	void AffichageMessages(CString Message);
	void RajoutAffichageMessages(CString rajout);

	void AffichageEtape(CString etape);
	void RajoutAffichageEtape(CString rajout);
	void AncienAffichageEtape();

	void AfficherCalo();
	void AfficherBassePression();
	void AfficherHautePression();
	void AfficherTemperatures();
	int MessageBoxConfirmation(CString message, UINT nType);
	

	// Manip_Donnees
public:

	void RecuperationDonnees();
	CString GetDonneesExperience();
	void EchangeDonnees();
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


	// Manip_Fichier

public:
	std::string NomFichier(std::string extention);
	std::string NomFichierEntete(std::string extention);

	void EnregistrementFichierMesures();
	void OuvertureFichierMesures();
	void FermetureFichierMesures();

	std::string EnteteBase();
	std::string EnteteBaseCSV();
	std::string EnteteGeneral();
	std::string EnteteGeneralCSV();
	void EcritureEntete();
	void EcritureEnteteCSV();


	// Manip_Mesure

public:
	void RajoutMesure();

protected :
	void OuvrirInstruments();
	void FermerInstruments();
	double LectureMesure(ConnectionMesure Appareil);
	void LectureCalo();
	void LectureHautePression();
	void LectureBassePression();
	void LectureTemperatures();

	void ThreadMesures();

	static DWORD WINAPI ThreadProc_LectureCalo(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_LectureHautePression(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_LectureBassePression(LPVOID lpParam);
	static DWORD WINAPI ThreadProc_LectureTemperature(LPVOID lpParam);


	// Manip_Securite

public:

	void InitialisationMesureSecuriteManuelle();
	void SecuriteHautePression();
	void SecuriteHautePressionManuelle();
	void SecuriteHautePressionAuto();

	void SecuriteTemperatures();
	void SecuriteTemperaturesManuelle();
	void SecuriteTemperaturesAuto();
protected:
	static DWORD WINAPI MesureSecuriteHautePression(LPVOID lpParam);
	static DWORD WINAPI MesureSecuriteTemperatureElevee(LPVOID lpParam);
	static DWORD WINAPI MesureSecuriteTemperatureFaible(LPVOID lpParam);


	// Manip_Temps

public:

	void AttenteMinutes (int nbminutes);
	void AttenteSecondes (int nbsecondes);
	CString MessageAttente(int nbsecondes);
	CString MessageTemps(int duree);


	// Manip_Vannes

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