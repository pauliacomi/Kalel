#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;

void CManip_AutoGaz::Desorption(LPVOID pParam)
{
	PointArretFonction();
	etape_en_cours = STAGE_DESORPTION;
	DonneesAutoGrapheEtape();

	messageHandler.DisplayMessage(_T("Début de la désorption\r\n"));
	ToutFermer();

	tentative = 0;
	derniere_dose = FALSE;
	do {
		// si ouvert, fermer la pompe
		tentative++;
		RecuperationDonneesDesorption();

		goto_message.Format(_T("Début de la tentative %d\r\n"), tentative);
		messageHandler.DisplayMessage(goto_message);

		Desorption_InitialisationPressionInitiale(); PointArretFonction();

		MettreEnRouteLaPompe();

		Desorption_EnlevementGaz(); PointArretFonction();

		ArreterLaPompe();

		Desorption_EquilibreDansVolumeRef(pParam); PointArretFonction();

		Desorption_DesorptionEchantillonEtEquilibre(pParam); PointArretFonction();


	FinDose:
		messageHandler.DisplayMessage(_T("Isolation de l'échantillon\r\n"));
		Fermeture_Vanne(5);

		goto_message.Format(_T("Fin de la tentative %d\r\n"), tentative);
		messageHandler.DisplayMessage(goto_message);

	} while (PressionFinale >= desorption.pression_finale && !derniere_dose);

	PointArretFonction();

	if (desorption.derniere_etape)
		Desorption_DerniereEtape(pParam);

	PointArretFonction();

	messageHandler.DisplayStep(_T("Fin de la désorption\r\n"));
	messageHandler.DisplayMessage(_T("Fin de la désorption\r\n"));
}


void CManip_AutoGaz::Desorption_InitialisationPressionInitiale()
{
	CString etape;
	etape.Format(_T("Désorption %d : Initialisation de la Pression Initiale"), tentative);
	messageHandler.DisplayStep(etape);
	messageHandler.DisplayMessage(_T("Initialisation de la Pression Initiale\r\n"));

	LireAfficherBassePression();
	LireAfficherHautePression();
	InitialiserAfficherPressionInitiale(resultat_hp);
}


void CManip_AutoGaz::Desorption_EnlevementGaz()
{
	ReinitialisationVerificationInjection();
	enlevement = 0;

	if (PressionInitiale - desorption.delta_pression < desorption.pression_finale)
	{
		delta_pression_demande = 1.1 * (PressionInitiale - desorption.pression_finale);
		derniere_dose = TRUE;
	}
	else
		delta_pression_demande = desorption.delta_pression;

	do {
		enlevement++;
		messageHandler.DisplayMessage(_T("Début Enlèvement de gaz dans le volume de référence\r\n"));
		CString etape;
		etape.Format(_T("Désorption %d : Enlèvement de gaz dans le volume de référence %d"), tentative, enlevement);
		messageHandler.DisplayStep(etape);

		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);

		LireAfficherHautePression();
		InitialiserAfficherPressionFinale(resultat_hp);

		CString temp_mess;
		temp_mess.Format(_T("Pression Initiale = %f\r\n"), PressionInitiale);
		messageHandler.DisplayMessage(temp_mess);
		temp_mess.Format(_T("Pression Finale = %f\r\n"), PressionFinale);
		messageHandler.DisplayMessage(temp_mess);
		temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
		messageHandler.DisplayMessage(temp_mess);
		temp_mess.Format(_T("delta pression = %f\r\n"), (delta_pression_demande));
		messageHandler.DisplayMessage(temp_mess);

		temp_mess.Format(_T("Fin enlèvement de gaz %d\r\n"), enlevement);
		messageHandler.DisplayMessage(temp_mess);

		VerificationInjection();

		PointArretSousFonction();
	} while (PressionInitiale - PressionFinale <= delta_pression_demande &&
		PressionFinale > 0.1);
}


void CManip_AutoGaz::Desorption_EquilibreDansVolumeRef(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Equilibre dans le volume de référence\r\n"));



	CString nom_etape;
	nom_etape.Format(_T("Désorption %d : Equilibre dans le volume de référence"), tentative);
	LireEcrireAfficher(desorption.temps_volume, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Equilibre dans le volume de référence\r\n"));
}


void CManip_AutoGaz::Desorption_DesorptionEchantillonEtEquilibre(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Désorption de l'échantillon et équilibre\r\n"));
	Ouverture_Vanne(5);
	CString nom_etape;
	nom_etape.Format(_T("Désorption %d : Désorption de l'échantillon et équilibre"), tentative);
	LireEcrireAfficher(desorption.temps_desorption, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Désorption de l'échantillon et équilibre\r\n"));
}

void CManip_AutoGaz::Desorption_DerniereEtape(LPVOID pParam)
{
	ToutFermer(); // Vérification, normalement tout est fermé

	MettreEnRouteLaPompe();
	Ouverture_Vanne(8);
	Ouverture_Vanne(7);
	//Sleep(5000);
	AttenteSecondes(5);
	//Ouverture_Vanne(6);

	PointArretSousFonction();

	messageHandler.DisplayMessage(_T("Equilibre dans le volume de référence dans la dernière étape 1\r\n"));
	CString nom_etape = _T("Désorption Dernière Etape : Equilibre dans le volume de référence 1");
	LireEcrireAfficher(desorption.temps_volume, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Equilibre dans le volume de référence dans la dernière étape 1\r\n"));

	ArreterLaPompe();
	Fermeture_Vanne(8);
	Fermeture_Vanne(7);

	PointArretSousFonction();

	messageHandler.DisplayMessage(_T("Equilibre dans le volume de référence dans la dernière étape 2\r\n"));
	/*CString*/ nom_etape = _T("Désorption Dernière Etape : Equilibre dans le volume de référence 2");
	LireEcrireAfficher(desorption.temps_volume, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Equilibre dans le volume de référence dans la dernière étape 2\r\n"));

	PointArretSousFonction();

	Ouverture_Vanne(5);

	PointArretSousFonction();

	messageHandler.DisplayMessage(_T("Désorption de l'échantillon et équilibre dans la dernière étape \r\n"));
	/*CString*/ nom_etape = _T("Désorption Dernière Etape : Désorption de l'échantillon et équilibre");
	LireEcrireAfficher(desorption.temps_desorption, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Désorption de l'échantillon et équilibre dans la dernière étape \r\n"));

	PointArretSousFonction();

	Fermeture_Vanne(5);
	//Fermeture_Vanne(6);

	PointArretSousFonction();



}