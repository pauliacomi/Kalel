#include "StdAfx.h"
#include "Manip_AutoGaz.h"
#include "Define_Manip_AutoGaz_Doses.h"

using namespace std;


void CManip_AutoGaz::GrandesDoses(LPVOID pParam)
{
	PointArretFonction();
	etape_en_cours = STAGE_BIG_DOSES;

	DonneesAutoGrapheEtape();

	AffichageMessages(_T("Début des grandes doses\r\n"));
	Fermeture_Vanne(5);
	dose = 0;

	derniere_dose = FALSE;


	do {
		dose++;
		RecuperationDonneesGrandesDoses();

		goto_message.Format(_T("Début de la grande dose %d\r\n"), dose);
		AffichageMessages(goto_message);
		GrandesDoses_InitialisationPressionInitiale(); PointArretFonction();

		GrandesDoses_InjectionGaz(); PointArretFonction();

		GrandesDoses_EquilibreDansVolumeRef(pParam); PointArretFonction();

		GrandesDoses_AdsorptionHautePression(pParam); PointArretFonction();

	FinDose:
		AffichageMessages(_T("Isolation de l'échantillon\r\n"));
		Fermeture_Vanne(5);

		goto_message.Format(_T("Fin de la grande dose %d\r\n"), dose);
		AffichageMessages(goto_message);

		//Vérification de la limite basse pression
	} while (PressionFinale < grandes_doses.pression_finale && !derniere_dose);

	AffichageEtape(_T("Fin des grandes doses\r\n"));
	AffichageMessages(_T("Fin des grandes doses\r\n"));

}


void CManip_AutoGaz::GrandesDoses_InitialisationPressionInitiale()
{
	CString etape;
	etape.Format(_T("Grandes Doses %d : Initialisation de la Pression Initiale"), dose);
	AffichageEtape(etape);
	AffichageMessages(_T("Initialisation de la Pression Initiale\r\n"));
	Fermeture_Vanne(6);

	LireAfficherBassePression();
	LireAfficherHautePression();
	InitialiserAfficherPressionInitiale(resultat_hp);
}



void CManip_AutoGaz::GrandesDoses_InjectionGaz()
{
	multiplicateur = 1.1;
	int nb_tentatives = 1;
	bool InjectionReussie = FALSE;
	injection = 0;


	while (!InjectionReussie)
	{
		injection++;
		CString temp_mess;
		temp_mess.Format(_T("Multiplicateur = %f\r\n"), multiplicateur);
		AffichageMessages(temp_mess);
		ReinitialisationVerificationInjection();

		if (PressionInitiale + grandes_doses.delta_pression >= min(grandes_doses.pression_finale, GetPressionSecuriteHautePression()))
		{
			if (grandes_doses.pression_finale < GetPressionSecuriteHautePression())
				delta_pression_demande = 1.1 * (grandes_doses.pression_finale - PressionInitiale);
			else
				delta_pression_demande = 0.9 * (GetPressionSecuriteHautePression() - PressionInitiale);
			derniere_dose = TRUE;
		}
		else
			delta_pression_demande = grandes_doses.delta_pression;

		do {
			GrandesDoses_Injection();
			PointArretSousFonction();
		} while (PressionFinale - PressionInitiale <= delta_pression_demande);

		ReinitialisationVerificationInjection();

		if (PressionFinale - PressionInitiale > multiplicateur*(delta_pression_demande))
		{
			if (PompeEnArret())
				MettreEnRouteLaPompe();

			GrandesDoses_EnlevementGaz();
			PointArretSousFonction();
		}

		if (PressionFinale - PressionInitiale < 0.9*(delta_pression_demande))
		{
			if (nb_tentatives >= tentatives_possible)
			{
				multiplicateur += 0.1;
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
		AffichageMessages(_T("La Haute Pression est inférieure à la sécurité de la Basse Pression\r\n"));
		AffichageMessages(_T("Possibilité d'ouvrir la vanne 6\r\n"));
		Ouverture_Vanne(6);
		AffichageMessages(_T("Attente de 5 secondes\r\n"));
		Sleep(5000); // 5 secondes
	}

	AffichageMessages(_T("Injection réussie\r\n"));
}




void CManip_AutoGaz::GrandesDoses_Injection()
{

	CString message;
	message.Format(_T("Début Injection dans le volume de référence %d\r\n"), injection);
	AffichageMessages(message);
	CString etape;
	etape.Format(_T("Grandes Doses %d : Injection dans le volume de référence %d"), dose, injection);
	AffichageEtape(etape);

	//Injection basse pression

	OuvrirEtFermer_Vanne(1);
	OuvrirEtFermer_Vanne(4);

	LireAfficherHautePression();
	InitialiserAfficherPressionFinale(resultat_hp);


	CString temp_mess;
	temp_mess.Format(_T("Pression Initiale = %f\r\n"), PressionInitiale);
	AffichageMessages(temp_mess);
	temp_mess.Format(_T("Pression Finale = %f\r\n"), PressionFinale);
	AffichageMessages(temp_mess);
	temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
	AffichageMessages(temp_mess);
	temp_mess.Format(_T("delta pression = %f\r\n"), (delta_pression_demande));
	AffichageMessages(temp_mess);

	message.Format(_T("Fin injection %d\r\n"), injection);
	AffichageMessages(message);

	VerificationInjection();

}



void CManip_AutoGaz::GrandesDoses_EnlevementGaz()
{
	//MettreEnRouteLaPompe();

	CString message;
	message.Format(_T("Debut enlèvement de gaz %d\r\n"), injection);
	AffichageMessages(message);
	CString etape;
	etape.Format(_T("Grandes Doses %d : Enlèvement de gaz %d"), dose, injection);
	AffichageEtape(etape);

	CString temp_mess;
	temp_mess.Format("Pression Initiale = %f\r\n", PressionInitiale);
	AffichageMessages(temp_mess);
	temp_mess.Format("Pression Finale = %f\r\n", PressionFinale);
	AffichageMessages(temp_mess);
	temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
	AffichageMessages(temp_mess);
	temp_mess.Format(_T("multiplicateur(%f) * delta pression = %f\r\n"), multiplicateur, multiplicateur*(delta_pression_demande));
	AffichageMessages(temp_mess);
	temp_mess.Format(_T("0.9 * delta pression = %f\r\n"), 0.9*(delta_pression_demande));
	AffichageMessages(temp_mess);

	do {
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);


		LireAfficherHautePression();
		InitialiserAfficherPressionFinale(resultat_hp);

		temp_mess.Format("Pression Initiale = %f\r\n", PressionInitiale);
		AffichageMessages(temp_mess);
		temp_mess.Format("Pression Finale = %f\r\n", PressionFinale);
		AffichageMessages(temp_mess);
		temp_mess.Format(_T("Pression Finale - Pression Initiale = %f\r\n"), PressionFinale - PressionInitiale);
		AffichageMessages(temp_mess);
		temp_mess.Format(_T("multiplicateur(%f) * delta pression = %f\r\n"), multiplicateur, multiplicateur*(delta_pression_demande));
		AffichageMessages(temp_mess);

		PointArretSousFonction();

		VerificationInjection();

	} while (PressionFinale - PressionInitiale > multiplicateur*(delta_pression_demande));

	message.Format(_T("Fin enlèvement de gaz %d\r\n"), injection);
	AffichageMessages(message);

	//ArreterLaPompe();
}




void CManip_AutoGaz::GrandesDoses_EquilibreDansVolumeRef(LPVOID pParam)
{
	AffichageMessages(_T("Equilibre dans le volume de référence\r\n"));
	CString nom_etape;
	nom_etape.Format(_T("Grandes Doses %d : Equilibre dans le volume de référence"), dose);
	LireEcrireAfficher(grandes_doses.temps_volume, nom_etape, pParam);
	AffichageMessages(_T("Fin Equilibre dans le volume de référence\r\n"));
}


void CManip_AutoGaz::GrandesDoses_AdsorptionHautePression(LPVOID pParam)
{
	AffichageMessages(_T("Adsorption Haute Pression"));
	AffichageMessages(_T("Demande d'ouverture de la vanne 5 pour l'adsorption\r\n"));
	Ouverture_Vanne(5);

	CString nom_etape;
	nom_etape.Format(_T("Grandes Doses %d : Adsorption Haute Pression"), dose);
	LireEcrireAfficher(grandes_doses.temps_adsorption, nom_etape, pParam);
	AffichageMessages(_T("Fin Adsorption Haute Pression"));
}
