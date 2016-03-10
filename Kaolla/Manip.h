#pragma once

#ifndef MANIP_H
#define MANIP_H

#include "Kaolla.h"
#include "KaollaDoc.h"

#include "KaollaView.h"

#include "time.h"

#include "Keithley.h"
#include "Mensor.h"
#include "Instrument.h"
#include "Temperature.h"
#include "Parametres.h"
#include "Conversions.h"
#include "ConnectionMesure.h"

#include "Vannes.h"

#include "Parametres_experience.h"

#include "Chrono.h"

#include "shlwapi.h" // pour PathIsDirectory

#include <string>

#include <sstream>  //ostringstream
/*
#include <ctime>*/
//#include <iostream>


#include <fstream>

#define finl "\r\n" 

class CManip
{
public:

	// -------- Variables -----------

	CKaollaView* m_KaollaView;
	CKaollaDoc* m_Doc;
	bool manip_en_cours;
	double temps_manip;                          //calcul du temps
	Keithley* g_pKeithley;
	Mensor* g_pMensor;
	CVannes* g_pVanne;
	//CInstrument instrument[3];
	CInstrument* instrument[3];
	CTemperature* g_pTemperature;
	CProprietes_experience* m_proprietes_experience;

	ConnectionMesure AppareilCalo, AppareilHP, AppareilBP;
	int synchCalo, synchHP, synchBP;

	float TemperatureCalo;
	float TemperatureCage; 
    float TemperaturePiece;       //Résultats de l'USB 9211 pour les températures

	double resultat_calo;
	double resultat_bp;
	double resultat_hp;

	CChrono temps_experience;
	CChrono temps_intermediaire;


	Donnees_General general;
	// Donnees_Divers divers;

	int numero_mesure;



	int TypeExperience;
	int GenreExperience;

	// Manip_Auto

	int demande_arret;
	int etape_en_cours;
	int etape_perturbe;

protected:
	DWORD ThreadId;
	HANDLE hThread[3];

public:

	// ------- Fonctions --------

	// Manip

	CManip(void);
	~CManip(void);
	void SetKaollaView(CKaollaView* pKV);
	void SetKeithley(Keithley* Keith);
	void SetMensor(Mensor* Mens);
	void SetVannes(CVannes* pVannes);
	void SetTemperature(CTemperature* pTemperature);

	void InitialisationDocument();
	void InitialisationInstruments();
	void InitialisationManip();

	void SetManipManuelle();
	void SetManipAuto();

	void ReinitialisationManuelle();
protected:
	void MiseAJour();

	void DebloqueMenu();
	void RemettreBoutonLancer();


public:
	void DonneesGrapheEtape();


	// Manip_Affichage

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
public:
	// Manip_Donnees

	void EchangeDonnees();
	void DonneesManuelleGrapheEtape();

protected:
	Donnees_General DonneesActuellesGeneral();
/*	Donnees_Divers DonneesActuellesDivers();
	Donnees_Petites_Doses DonneesActuellesPetitesDoses();
	Donnees_Grandes_Doses DonneesActuellesGrandesDoses();
	Donnees_Desorption DonneesActuellesDesorption();
	Donnees_Adsorption_Continue DonneesActuellesAdsorptionContinue();*/
public:
	void RecuperationDonnees();
	CString GetDonneesExperience();

	// Manip_ExecutionManuelle

	void ExecutionManuelle(LPVOID pParam);
	void FinAffichage();

	// Manip_Fichier


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

public:
	// Manip_Securite

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
public:

	// Manip_Temps

	void AttenteMinutes (int nbminutes);
	void AttenteSecondes (int nbsecondes);
	CString MessageAttente(int nbsecondes);
	CString MessageTemps(int duree);

	// Manip_Vannes

	bool OuvrirEtFermer_Vanne(int num_vanne);
	bool Ouverture_Vanne(int num_vanne);
	bool Fermeture_Vanne(int num_vanne);
	bool FermetureDeToutesLesVannes();
	bool ActiverEV1();
	bool DesactiverEV1();
	bool ActiverEV2();
	bool DesactiverEV2();
	bool ActiverPompe();
	bool DesactiverPompe();
	bool FermerLesValvesEtLaPompe();
	/*
	bool MettreEnRouteLaPompe();
	bool ArreterLaPompe();
	*/
	bool ToutFermer();

	void SignalerOuvertureVanne(int num_vanne);
	void SignalerFermetureVanne(int num_vanne);
	void SignalerFermetureDeToutesLesVannes();

	void SignalerActivationEV1();
	void SignalerDesactivationEV1();
	void SignalerActivationEV2();
	void SignalerDesactivationEV2();
	void SignalerActivationPompe();
	void SignalerDesactivationPompe();
	void SignalerDesactivationValvesEtPompe();

	bool EstOuvert_Vanne(int num_vanne);
	bool EstFerme_Vanne(int num_vanne);

	bool EV1EstActive();
	bool EV1EstDesactive();
	bool EV2EstActive();
	bool EV2EstDesactive();
	bool PompeEstActive();
	bool PompeEstDesactive();

	// Autres


};

#endif