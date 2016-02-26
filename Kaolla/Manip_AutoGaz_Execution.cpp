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
	InitialisationManip();

	RecuperationDonnees();

	OuvertureFichierMesures();

	EcritureEntete();
	EcritureEnteteCSV();

	dose = 0;
	injection = 0;
	temps_manip = 0;
	numero_mesure = 1;
	demande_arret = INACTIF;
	etape_en_cours = STAGE_UNDEF;
	//MesureBassePression = GetMesureBassePression();

	if (Verifications() == IDCANCEL)
	{
		AffichageMessages(_T("Expérience annulée\r\n"));
		//GREY_OUT les boutons

		FermerInstruments();

		FermetureFichierMesures();
		DebloqueMenu();

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

	DebloqueMenu();
	AffichageMessages(_T("Expérience terminée\r\n"));

	AfxMessageBox(_T("Expérience terminée\t\n"), MB_ICONEXCLAMATION | MB_OK);
}