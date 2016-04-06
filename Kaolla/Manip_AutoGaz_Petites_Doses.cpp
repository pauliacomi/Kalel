#include "StdAfx.h"
#include "Manip_AutoGaz.h"
#include "Define_Manip_AutoGaz_Doses.h"

using namespace std;


void CManip_AutoGaz::PetitesDoses(LPVOID pParam)
{
	etape_en_cours = STAGE_SMALL_DOSES;
	PointArretFonction();

	DonneesAutoGrapheEtape();

	messageHandler.DisplayMessage(_T("Début des petites doses\r\n"));
	Fermeture_Vanne(5);

	ToutFermer();

	dose = 0;


	do {
		dose++;
		RecuperationDonneesPetitesDoses();

		goto_message.Format(_T("Début de la petite dose %d\r\n"), dose);
		messageHandler.DisplayMessage(goto_message);

		PetitesDoses_InitialisationPressionInitiale(); PointArretFonction();

		PetitesDoses_InjectionGaz(); PointArretFonction();

		PetitesDoses_EquilibreDansVolumeRef(pParam); PointArretFonction();

		PetitesDoses_AdsorptionBassePression(pParam); PointArretFonction();


	FinDose:
		messageHandler.DisplayMessage(_T("Isolation de l'échantillon\r\n"));
		Fermeture_Vanne(5);

		goto_message.Format(_T("Fin de la petite dose %d\r\n"), dose);
		messageHandler.DisplayMessage(goto_message);

		//Vérification de la limite basse pression
	} while (PressionFinale < petites_doses.pression_finale);

	messageHandler.DisplayStep(_T("Fin des petites doses\r\n"));
	messageHandler.DisplayMessage(_T("Fin des petites doses\r\n"));

}


void CManip_AutoGaz::PetitesDoses_InitialisationPressionInitiale()
{
	CString etape;
	etape.Format(_T("Petites Doses %d : Initialisation de la Pression Initiale"), dose);
	messageHandler.DisplayStep(etape);
	messageHandler.DisplayMessage(_T("Initialisation de la Pression Initiale\r\n"));
	Fermeture_Vanne(6);

	LireAfficherBassePression();
	LireAfficherHautePression();
	InitialiserAfficherPressionInitiale(resultat_hp);
}



void CManip_AutoGaz::PetitesDoses_InjectionGaz()
{
	multiplicateur = 1.1f;
	int nb_tentatives = 1;
	bool InjectionReussie = FALSE;
	injection = 0;


	while (!InjectionReussie)
	{
		injection++;
		CString temp_mess;
		temp_mess.Format(_T("Multiplicateur = %f\r\n"), multiplicateur);
		messageHandler.DisplayMessage(temp_mess);
		ReinitialisationVerificationInjection();
		do {
			PetitesDoses_Injection();
			PointArretSousFonction();
		} while (PressionFinale - PressionInitiale <= petites_doses.delta_pression);

		ReinitialisationVerificationInjection();

		if (PressionFinale - PressionInitiale > multiplicateur*(petites_doses.delta_pression))
		{
			if (PompeEnArret())
				MettreEnRouteLaPompe();
			PetitesDoses_EnlevementGaz();
			PointArretSousFonction();
		}

		if (PressionFinale - PressionInitiale < 0.9*(petites_doses.delta_pression))
		{
			if (nb_tentatives >= tentatives_possible)
			{
				multiplicateur += 0.1f;
				nb_tentatives = 1;
			}

			else
				nb_tentatives += 1;
		}
		else
		{
			InjectionReussie = TRUE;
		}
	}

	if (PompeEnRoute())
		ArreterLaPompe();

	LireAfficherHautePression();

	if (resultat_hp < GetPressionSecuriteBassePression())
	{
		messageHandler.DisplayMessage(_T("La Haute Pression est inférieure à la sécurité de la Basse Pression\r\n"));
		messageHandler.DisplayMessage(_T("Possibilité d'ouvrir la vanne 6\r\n"));
		Ouverture_Vanne(6);
		messageHandler.DisplayMessage(_T("Attente de 5 secondes\r\n"));
		//Sleep(5000);
		AttenteSecondes(5);
	}

	messageHandler.DisplayMessage(_T("Injection réussie\r\n"));
}




void CManip_AutoGaz::PetitesDoses_Injection()
{
	CString message;
	message.Format(_T("Début Injection dans le volume de référence %d\r\n"), injection);
	messageHandler.DisplayMessage(message);
	CString etape;
	etape.Format(_T("Petites Doses %d : Injection dans le volume de référence %d"), dose, injection);
	messageHandler.DisplayStep(etape);

	//Injection basse pression

	OuvrirEtFermer_Vanne(2);
	OuvrirEtFermer_Vanne(3);
	OuvrirEtFermer_Vanne(4);

	LireAfficherHautePression();
	InitialiserAfficherPressionFinale(resultat_hp);



	message.Format(_T("Pression Initiale = %f\r\n"), PressionInitiale);
	messageHandler.DisplayMessage(message);
	message.Format(_T("Pression Finale = %f\r\n"), PressionFinale);
	messageHandler.DisplayMessage(message);
	message.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
	messageHandler.DisplayMessage(message);
	message.Format(_T("delta pression = %f\r\n"), (petites_doses.delta_pression));
	messageHandler.DisplayMessage(message);

	message.Format(_T("Fin injection %d\r\n"), injection);
	messageHandler.DisplayMessage(message);

	VerificationInjection();

}



void CManip_AutoGaz::PetitesDoses_EnlevementGaz()
{
	//MettreEnRouteLaPompe();

	CString message;
	message.Format(_T("Debut enlèvement de gaz %d\r\n"), injection);
	messageHandler.DisplayMessage(message);
	CString etape;
	etape.Format(_T("Petites Doses %d : Enlèvement de gaz %d"), dose, injection);
	messageHandler.DisplayStep(etape);

	CString temp_mess;
	temp_mess.Format("Pression Initiale = %f\r\n", PressionInitiale);
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format("Pression Finale = %f\r\n", PressionFinale);
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format(_T("multiplicateur(%f) * delta pression = %f\r\n"), multiplicateur, multiplicateur*(petites_doses.delta_pression));
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format(_T("0.9 * delta pression = %f\r\n"), 0.9*(petites_doses.delta_pression));
	messageHandler.DisplayMessage(temp_mess);

	do {
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);


		LireAfficherHautePression();
		InitialiserAfficherPressionFinale(resultat_hp);

		temp_mess.Format("Pression Initiale = %f\r\n", PressionInitiale);
		messageHandler.DisplayMessage(temp_mess);
		temp_mess.Format("Pression Finale = %f\r\n", PressionFinale);
		messageHandler.DisplayMessage(temp_mess);
		temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
		messageHandler.DisplayMessage(temp_mess);
		temp_mess.Format(_T("multiplicateur(%f) * delta pression = %f\r\n"), multiplicateur, multiplicateur*(petites_doses.delta_pression));
		messageHandler.DisplayMessage(temp_mess);

		PointArretSousFonction();

		VerificationInjection();

	} while (PressionFinale - PressionInitiale > multiplicateur*(petites_doses.delta_pression));

	message.Format(_T("Fin enlèvement de gaz %d\r\n"), injection);
	messageHandler.DisplayMessage(message);

	//ArreterLaPompe();
}




void CManip_AutoGaz::PetitesDoses_EquilibreDansVolumeRef(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Equilibre dans le volume de référence\r\n"));
	CString nom_etape;
	nom_etape.Format(_T("Petites Doses %d : Equilibre dans le volume de référence"), dose);
	LireEcrireAfficher(petites_doses.temps_volume, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Equilibre dans le volume de référence\r\n"));
}


void CManip_AutoGaz::PetitesDoses_AdsorptionBassePression(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Adsorption Basse Pression\r\n"));
	messageHandler.DisplayMessage(_T("Demande d'ouverture de la vanne 5 pour l'adsorption\r\n"));
	Ouverture_Vanne(5);

	CString nom_etape;
	nom_etape.Format(_T("Petites Doses %d : Adsorption Basse Pression"), dose);
	LireEcrireAfficher(petites_doses.temps_adsorption, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Adsorption Basse Pression\r\n"));
}
