#include "StdAfx.h"
#include "Manip_AutoGaz.h"
#include "Define_Manip_AutoGaz_Mise_Sous_Vide.h"


using namespace std;


void CManip_AutoGaz::MiseSousVide(LPVOID pParam)
{
	PointArretMiseSousVide();
	etape_en_cours = STAGE_UNDER_VACUUM;
	DonneesAutoGrapheEtape();

	messageHandler.DisplayMessage(_T("Début de la mise sous vide\r\n"));
	

	// Normalement, toutes les vannes sont fermées
	// Mais on les ferme toutes par mesure de sécurité

	//FermetureDeToutesLesVannes();
	ToutFermer();

	// Mettre en route la pompe
	MettreEnRouteLaPompe();

	Ouverture_Vanne(5);
	PointArretMiseSousVide();

	messageHandler.DisplayMessage(_T("Début Mise sous vide de la haute pression\r\n"));
	messageHandler.DisplayStep(_T("Mise sous vide : Haute Pression"));

	do{
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);

		LireAfficherHautePression();
		PointArretMiseSousVide();
	}while(resultat_hp >= GetPressionSecuriteBassePression());

	messageHandler.DisplayMessage(_T("Fin Mise sous vide de la haute pression\r\n"));

	messageHandler.DisplayMessage(_T("Début Mise sous vide de la basse pression\r\n"));
	messageHandler.DisplayStep(_T("Mise sous vide : Basse Pression"));

	Ouverture_Vanne(6);

	do{
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);
		LireAfficherHautePression();
		PointArretMiseSousVide();
	}while(resultat_hp >= pression_pompe);

	messageHandler.DisplayMessage(_T("Fin Mise sous vide de la basse pression\r\n"));

	Ouverture_Vanne(8);
	Ouverture_Vanne(7);

	// si mise ou vide pas à effectuer : 30 minutes
	// temps (min) * 60 (s)
	int temps_mise_sous_vide;
	if (demande_arret == ARRET_URGENCE_HP || demande_arret == ARRET_URGENCE_TCH || demande_arret == ARRET_URGENCE_TCB)
	{
		temps_mise_sous_vide = temps_en_cas_d_urgence; // Minutes...
	}
	else
	{
		if(divers.mise_sous_vide_fin_experience)
			temps_mise_sous_vide = divers.temps_vide;// * 60000; // conversion de minutes en secondes
		else
			temps_mise_sous_vide = temps_defaut;// * 60000;
	}


	CString nom_etape = _T("Mise sous vide : Dernier équilibre sous vide");
	LireEcrireAfficher(temps_mise_sous_vide,nom_etape,pParam);


	Fermeture_Vanne(5);
	Fermeture_Vanne(6);
	Fermeture_Vanne(7);
	Fermeture_Vanne(8);

	ArreterLaPompe();

}