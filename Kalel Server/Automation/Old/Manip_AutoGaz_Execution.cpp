#include "StdAfx.h"
#include "Manip_AutoGaz.h"


using namespace std;



void CManip_AutoGaz::Execution(LPVOID pParam)
{
	if (TypeExperience == EXPERIMENT_TYPE_MANUAL)
		ExecutionManuelle(pParam);
	if (TypeExperience == EXPERIMENT_TYPE_AUTO)
		ExecutionAuto(pParam);
}



void CManip_AutoGaz::ExecutionAuto(LPVOID pParam)
{
	// Initialise the instruments, might be redundant if it is already called at the start of the creation
	InitialisationManip();

	// This gets the data from the MFCcontrol, should be deleted
	RecuperationDonnees();

	// Create, open and write columns to the measurement file
	OuvertureFichierMesures();

	// Create, open and write instrument parameters to CSV file
	EcritureEntete();
	EcritureEnteteCSV();

	// Initialise variables
	dose = 0;
	injection = 0;
	temps_manip = 0;
	numero_mesure = 1;
	demande_arret = INACTIF;
	etape_en_cours = STAGE_UNDEF;


	if (Verifications() == IDCANCEL)
	{
		messageHandler.DisplayMessage(_T("Exp�rience annul�e\r\n"));
		//GREY_OUT les boutons

		FermerInstruments();

		FermetureFichierMesures();
		messageHandler.UnlockMenu();

		return;
	}

	// top chrono de temps_manip
	temps_experience.TopChrono();



	LigneBaseEtEquilibre(pParam);

	if (adsorption_continue.a_effectuer && ContinuerExperience())
		AdsorptionContinue(pParam);


	//if (petites_doses.a_effectuer && ContinuerExperience())
	if (PetitesDosesAEffectuer())
		PetitesDoses(pParam);

	//if (moyennes_doses.a_effectuer && ContinuerExperience())
	if (MoyennesDosesAEffectuer())
		MoyennesDoses(pParam);

	//if (grandes_doses.a_effectuer && ContinuerExperience())
	if (GrandesDosesAEffectuer())
		GrandesDoses(pParam);

	//if (desorption.a_effectuer && ContinuerExperience())
	if (DesorptionAEffectuer())
		Desorption(pParam);

	if (divers.mise_sous_vide_fin_experience || demande_arret == ARRET_SOUSVIDE ||
		demande_arret == ARRET_URGENCE_HP || demande_arret == ARRET_URGENCE_TCH || demande_arret == ARRET_URGENCE_TCB)
		MiseSousVide(pParam);//MiseSousVide();


	FermerInstruments();

	ToutFermer();

	FermetureFichierMesures();

	ReinitialisationAuto();

	messageHandler.UnlockMenu();
	messageHandler.DisplayMessage(_T("Exp�rience termin�e\r\n"));
}