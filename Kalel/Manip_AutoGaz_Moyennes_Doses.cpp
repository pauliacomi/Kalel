#include "StdAfx.h"
#include "Manip_AutoGaz.h"
#include "Define_Manip_AutoGaz_Doses.h"

using namespace std;


void CManip_AutoGaz::MoyennesDoses(LPVOID pParam)
{
	PointArretFonction();
	etape_en_cours = STAGE_MEDIUM_DOSES;

	DonneesAutoGrapheEtape();

	messageHandler.DisplayMessage(_T("Début des moyennes doses\r\n"));
	Fermeture_Vanne(5);
	dose = 0;

	derniere_dose = FALSE;


	do {
		dose++;
		RecuperationDonneesMoyennesDoses();

		goto_message.Format(_T("Début de la moyenne dose %d\r\n"), dose);
		messageHandler.DisplayMessage(goto_message);
		MoyennesDoses_InitialisationPressionInitiale(); PointArretFonction();

		MoyennesDoses_InjectionGaz(); PointArretFonction();

		MoyennesDoses_EquilibreDansVolumeRef(pParam); PointArretFonction();

		MoyennesDoses_AdsorptionHautePression(pParam); PointArretFonction();

	FinDose:
		messageHandler.DisplayMessage(_T("Isolation de l'échantillon\r\n"));
		Fermeture_Vanne(5);

		goto_message.Format(_T("Fin de la moyenne dose %d\r\n"), dose);
		messageHandler.DisplayMessage(goto_message);

		//Vérification de la limite basse pression
	} while (PressionFinale < moyennes_doses.pression_finale && !derniere_dose);

	messageHandler.DisplayStep(_T("Fin des moyennes doses\r\n"));
	messageHandler.DisplayMessage(_T("Fin des moyennes doses\r\n"));

}


void CManip_AutoGaz::MoyennesDoses_InitialisationPressionInitiale()
{
	CString etape;
	etape.Format(_T("Moyennes Doses %d : Initialisation de la Pression Initiale"), dose);
	messageHandler.DisplayStep(etape);
	messageHandler.DisplayMessage(_T("Initialisation de la Pression Initiale\r\n"));
	Fermeture_Vanne(6);

	LireAfficherBassePression();
	LireAfficherHautePression();
	InitialiserAfficherPressionInitiale(resultat_hp);
}



void CManip_AutoGaz::MoyennesDoses_InjectionGaz()
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

		if (PressionInitiale + moyennes_doses.delta_pression >= min(moyennes_doses.pression_finale, GetPressionSecuriteHautePression()))
		{
			if (moyennes_doses.pression_finale < GetPressionSecuriteHautePression())
				delta_pression_demande = 1.1 * (moyennes_doses.pression_finale - PressionInitiale);
			else
				delta_pression_demande = 0.9 * (GetPressionSecuriteHautePression() - PressionInitiale);
			derniere_dose = TRUE;
		}
		else
			delta_pression_demande = moyennes_doses.delta_pression;

		do {
			MoyennesDoses_Injection();
			PointArretSousFonction();
		} while (PressionFinale - PressionInitiale <= delta_pression_demande);

		ReinitialisationVerificationInjection();

		if (PressionFinale - PressionInitiale > multiplicateur*(delta_pression_demande))
		{
			if (PompeEnArret())
				MettreEnRouteLaPompe();

			MoyennesDoses_EnlevementGaz();
			PointArretSousFonction();
		}

		if (PressionFinale - PressionInitiale < 0.9*(delta_pression_demande))
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
		Sleep(5000); // 5 secondes
	}

	messageHandler.DisplayMessage(_T("Injection réussie\r\n"));
}




void CManip_AutoGaz::MoyennesDoses_Injection()
{

	CString message;
	message.Format(_T("Début Injection dans le volume de référence %d\r\n"), injection);
	messageHandler.DisplayMessage(message);
	CString etape;
	etape.Format(_T("Moyennes Doses %d : Injection dans le volume de référence %d"), dose, injection);
	messageHandler.DisplayStep(etape);

	//Injection basse pression

	OuvrirEtFermer_Vanne(1);
	OuvrirEtFermer_Vanne(4);

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

	message.Format(_T("Fin injection %d\r\n"), injection);
	messageHandler.DisplayMessage(message);

	VerificationInjection();

}



void CManip_AutoGaz::MoyennesDoses_EnlevementGaz()
{
	//MettreEnRouteLaPompe();

	CString message;
	message.Format(_T("Debut enlèvement de gaz %d\r\n"), injection);
	messageHandler.DisplayMessage(message);
	CString etape;
	etape.Format(_T("Moyennes Doses %d : Enlèvement de gaz %d"), dose, injection);
	messageHandler.DisplayStep(etape);

	CString temp_mess;
	temp_mess.Format("Pression Initiale = %f\r\n", PressionInitiale);
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format("Pression Finale = %f\r\n", PressionFinale);
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format(_T("multiplicateur(%f) * delta pression = %f\r\n"), multiplicateur, multiplicateur*(delta_pression_demande));
	messageHandler.DisplayMessage(temp_mess);
	temp_mess.Format(_T("0.9 * delta pression = %f\r\n"), 0.9*(delta_pression_demande));
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
		temp_mess.Format(_T("multiplicateur(%f) * delta pression = %f\r\n"), multiplicateur, multiplicateur*(delta_pression_demande));
		messageHandler.DisplayMessage(temp_mess);

		PointArretSousFonction();

		VerificationInjection();

	} while (PressionFinale - PressionInitiale > multiplicateur*(delta_pression_demande));

	message.Format(_T("Fin enlèvement de gaz %d\r\n"), injection);
	messageHandler.DisplayMessage(message);

	//ArreterLaPompe();
}




void CManip_AutoGaz::MoyennesDoses_EquilibreDansVolumeRef(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Equilibre dans le volume de référence\r\n"));
	CString nom_etape;
	nom_etape.Format(_T("Moyennes Doses %d : Equilibre dans le volume de référence"), dose);
	LireEcrireAfficher(moyennes_doses.temps_volume, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Equilibre dans le volume de référence\r\n"));
}


void CManip_AutoGaz::MoyennesDoses_AdsorptionHautePression(LPVOID pParam)
{
	messageHandler.DisplayMessage(_T("Adsorption Haute Pression"));
	messageHandler.DisplayMessage(_T("Demande d'ouverture de la vanne 5 pour l'adsorption\r\n"));
	Ouverture_Vanne(5);

	CString nom_etape;
	nom_etape.Format(_T("Moyennes Doses %d : Adsorption Haute Pression"), dose);
	LireEcrireAfficher(moyennes_doses.temps_adsorption, nom_etape, pParam);
	messageHandler.DisplayMessage(_T("Fin Adsorption Haute Pression"));
}
