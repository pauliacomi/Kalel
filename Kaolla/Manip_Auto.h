#pragma once
#include "Manip.h"



class CManip_Auto :
	public CManip
{
public:
	CManip_Auto(void);
	~CManip_Auto(void);

	// ---------- Variables --------------- //

	Donnees_Divers divers;
	float PressionInitiale, PressionFinale;
	float precedent_injection;
	int compteur_injection;


	// ----------- Fonctions ------------- //

 
/**********************************************************************************************************************************
// Manip_Auto_Arret
***********************************************************************************************************************************/

	void DemandePause();
	void DemandeMiseSousVide();
	void ArretImmediat();
	void ProchaineCommande();
	void ProchaineEtape();
	void ProchaineDose();

	void MiseEnPause();
	void Reprise();
	void AffichageArret();

	bool ContinuerExperience();


/**********************************************************************************************************************************
// Manip_Auto_Donnees
***********************************************************************************************************************************/
	
	void DonneesAutoGrapheEtape();
	Donnees_Divers DonneesActuellesDivers();


/**********************************************************************************************************************************
// Manip_Auto_Fichier
***********************************************************************************************************************************/

	std::string EnteteBase();
	std::string EnteteBaseCSV();
	std::string EnteteDivers();
	std::string EnteteDiversCSV();
	
	void RajoutFichierEntete(CString StrChangement);
	void RajoutFichierEntete(std::string stringChangement);
	void RajoutFichierEnteteCSV(CString StrChangement);
	void RajoutFichierEnteteCSV(std::string stringChangement);


/**********************************************************************************************************************************
// Manip_Auto_LireAfficher
***********************************************************************************************************************************/

	void LireAfficherBassePression();
	void LireAfficherHautePression();
	void LireAfficherCalo();

	void InitialiserAfficherPressionInitiale(double init);
	void InitialiserAfficherPressionFinale(double init);


/**********************************************************************************************************************************
// Manip_Auto_Vannes
***********************************************************************************************************************************/

	bool Ouverture_Vanne(int num_vanne);
	bool Fermeture_Vanne(int num_vanne);
	bool OuvrirEtFermer_Vanne(int num_vanne);
	bool ActiverEV(int num_ev);
	bool DesactiverEV(int num_ev);
	bool ActiverPompe();
	bool DesactiverPompe();
	
	bool MettreEnRouteLaPompe();
	bool ArreterLaPompe();
	bool PompeEnRoute();
	bool PompeEnArret();
	bool ToutFermer();


/**********************************************************************************************************************************
// Manip_Auto_Probleme_Injection
***********************************************************************************************************************************/

	void ReinitialisationVerificationInjection();
	void VerificationInjection();
	static DWORD WINAPI PauseInjection(LPVOID lpParam);


/**********************************************************************************************************************************
// Manip_Auto_Verifications
***********************************************************************************************************************************/

	int Verifications();
	int VerificationSecurite();
	int VerificationVannes();
	int VerificationPressionResiduelle();
	int VerificationTemperature();

};