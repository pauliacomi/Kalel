#pragma once
#include "Manip_Auto.h"

#include "Dialogue_TypeExperience.h"


class CManip_AutoGaz :
	public CManip_Auto
{
public:
	CManip_AutoGaz(void);
	~CManip_AutoGaz(void);

	// ---------- Variables --------------- //

	Donnees_Petites_Doses petites_doses;
	Donnees_Moyennes_Doses moyennes_doses;
	Donnees_Grandes_Doses grandes_doses;
	Donnees_Desorption desorption;
	Donnees_Adsorption_Continue adsorption_continue;

	CString goto_message;
	int dose,injection; // Petites Doses et Grandes Doses
	int tentative,enlevement; // désorption

	float delta_pression_demande;
	bool derniere_dose;

	float multiplicateur;



	// ----------- Fonctions ------------- //

/**********************************************************************************************************************************
// Manip_AutoGaz
***********************************************************************************************************************************/

	void LancementExperience(LPVOID pParam);
	void ArretExperience();
	void ReinitialisationAuto();

/**********************************************************************************************************************************
// Manip_AutoGaz_AEffectuer
***********************************************************************************************************************************/

	bool PetitesDosesAEffectuer();
	bool MoyennesDosesAEffectuer();
	bool GrandesDosesAEffectuer();
	bool DesorptionAEffectuer();


/**********************************************************************************************************************************
// Manip_AutoGaz_Donnees
***********************************************************************************************************************************/

	void RecuperationDonnees();
	Donnees_General DonneesActuellesGeneral();
	Donnees_Divers DonneesActuellesDivers();
	Donnees_Petites_Doses DonneesActuellesPetitesDoses();
	Donnees_Moyennes_Doses DonneesActuellesMoyennesDoses();
	Donnees_Grandes_Doses DonneesActuellesGrandesDoses();
	Donnees_Desorption DonneesActuellesDesorption();
	Donnees_Adsorption_Continue DonneesActuellesAdsorptionContinue();
	CString GetDonneesExperience();

/**********************************************************************************************************************************
// Manip_AutoGaz_Execution
***********************************************************************************************************************************/

	void Execution(LPVOID pParam);
	void ExecutionAuto(LPVOID pParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_Fichier
***********************************************************************************************************************************/ 

	std::string EnteteBase();
	std::string EnteteBaseCSV();
	std::string EnteteAdsorptionContinue();
	std::string EnteteAdsorptionContinueCSV();
	std::string EntetePetitesDoses();
	std::string EntetePetitesDosesCSV();
	std::string EnteteMoyennesDoses();
	std::string EnteteMoyennesDosesCSV();
	std::string EnteteGrandesDoses();
	std::string EnteteGrandesDosesCSV();
	std::string EnteteDesorption();
	std::string EnteteDesorptionCSV();

	void EcritureEntete();
	void EcritureEnteteCSV();

	void RecuperationDonneesPetitesDoses();
	void RecuperationDonneesMoyennesDoses();
	void RecuperationDonneesGrandesDoses();
	void RecuperationDonneesDesorption();

/**********************************************************************************************************************************
// Manip_Auto_LEA
***********************************************************************************************************************************/

	void LireEcrireAfficher(int duree_evenement,CString nom_etape,LPVOID pParam);
	bool LireEcrireAfficher(LPVOID pParam);
	

/**********************************************************************************************************************************
// Manip_AutoGaz_Mesure
***********************************************************************************************************************************/

	void OuvertureFichierMesures();
	void EnregistrementFichierMesures();
	void FermetureFichierMesures();


/**********************************************************************************************************************************
// Manip_AutoGaz_Proprietes_Experience
***********************************************************************************************************************************/

	void ModifierParametresExperience();


/**********************************************************************************************************************************
// Manip_AutoGaz_Type_Experience
***********************************************************************************************************************************/

CDialogue_TypeExperience* DialogueTypeExperience;
void InitialisationDialogueTypeExperience();


/**********************************************************************************************************************************
// Manip_AutoGaz_Vannes
***********************************************************************************************************************************/

	bool Ouverture_Vanne(int num_vanne);
	bool Fermeture_Vanne(int num_vanne);
	bool OuvrirEtFermer_Vanne(int num_vanne);



	// Etapes

/**********************************************************************************************************************************
// Manip_AutoGaz_Ligne_Base_Equilibre
***********************************************************************************************************************************/

	void LigneBaseEtEquilibre(LPVOID pParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_Adsorption_Continue
***********************************************************************************************************************************/

	HANDLE hThreadDemandeOuverture;
	DWORD ThreadDemandeOuvertureId;

	void AdsorptionContinue(LPVOID pParam);
	void EquilibreInitialeDuVideDynamique(LPVOID pParam);
	void CalibrationDuDebit(LPVOID pParam);
	void CalibrationDuVolumeMort(LPVOID pParam);
	void EquilibreDuVideDynamiqueAvantAdsorption(LPVOID pParam);
	void DemandeOuvertureVanneAmpoule(LPVOID pParam);
	void ExperienceAdsorption(LPVOID pParam);
	void EquilibreFinAdsorption(LPVOID pParam);
	static DWORD WINAPI ThreadAttenteDemande(LPVOID lpParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_Petites_Doses
***********************************************************************************************************************************/

	void PetitesDoses(LPVOID pParam);
	void PetitesDoses_InitialisationPressionInitiale();
	void PetitesDoses_InjectionGaz();
	void PetitesDoses_Injection();
	void PetitesDoses_EnlevementGaz();
	void PetitesDoses_EquilibreDansVolumeRef(LPVOID pParam);
	void PetitesDoses_AdsorptionBassePression(LPVOID pParam);


/**********************************************************************************************************************************
// Manip_AutoGaz_Moyennes_Doses
***********************************************************************************************************************************/

	void MoyennesDoses(LPVOID pParam);
	void MoyennesDoses_InitialisationPressionInitiale();
	void MoyennesDoses_InjectionGaz();
	void MoyennesDoses_Injection();
	void MoyennesDoses_EnlevementGaz();
	void MoyennesDoses_EquilibreDansVolumeRef(LPVOID pParam);
	void MoyennesDoses_AdsorptionHautePression(LPVOID pParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_Grandes_Doses
***********************************************************************************************************************************/

	void GrandesDoses(LPVOID pParam);
	void GrandesDoses_InitialisationPressionInitiale();
	void GrandesDoses_InjectionGaz();
	void GrandesDoses_Injection();
	void GrandesDoses_EnlevementGaz();
	void GrandesDoses_EquilibreDansVolumeRef(LPVOID pParam);
	void GrandesDoses_AdsorptionHautePression(LPVOID pParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_Desorption
***********************************************************************************************************************************/

	void Desorption(LPVOID pParam);
	void Desorption_InitialisationPressionInitiale();
	void Desorption_EnlevementGaz();
	void Desorption_EquilibreDansVolumeRef(LPVOID pParam);
	void Desorption_DesorptionEchantillonEtEquilibre(LPVOID pParam);
	void Desorption_DerniereEtape(LPVOID pParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_Mise_Sous_Vide
***********************************************************************************************************************************/

	void MiseSousVide(LPVOID pParam);



	// MSV

/**********************************************************************************************************************************
// Manip_AutoGaz_MiseSousVide_Ampoule
***********************************************************************************************************************************/

	void MiseSousVideAmpoule();
	static DWORD WINAPI ThreadAttente(LPVOID lpParam);

/**********************************************************************************************************************************
// Manip_AutoGaz_MiseSousVide_Bouteille
***********************************************************************************************************************************/

	void MiseSousVideBouteille();
	void ProcedureMiseSousVideBouteille();
	void ProcedureMiseSousVideBouteille2();
	void EvacuationDuSysteme();

/**********************************************************************************************************************************
// Manip_AutoGaz_MiseSousVide_ChangementBouteille
***********************************************************************************************************************************/

	void ChangementBouteille();

};