#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;

void CManip_AutoGaz::Desorption(LPVOID pParam)
{
	PointArretFonction();
	etape_en_cours = STAGE_DESORPTION;
	DonneesAutoGrapheEtape();

	AffichageMessages(_T("D�but de la d�sorption\r\n"));
	ToutFermer();

	tentative = 0;
	derniere_dose = FALSE;
	do {
		// si ouvert, fermer la pompe
		tentative++;
		RecuperationDonneesDesorption();

		goto_message.Format(_T("D�but de la tentative %d\r\n"), tentative);
		AffichageMessages(goto_message);

		Desorption_InitialisationPressionInitiale(); PointArretFonction();

		MettreEnRouteLaPompe();

		Desorption_EnlevementGaz(); PointArretFonction();

		ArreterLaPompe();

		Desorption_EquilibreDansVolumeRef(pParam); PointArretFonction();

		Desorption_DesorptionEchantillonEtEquilibre(pParam); PointArretFonction();


	FinDose:
		AffichageMessages(_T("Isolation de l'�chantillon\r\n"));
		Fermeture_Vanne(5);

		goto_message.Format(_T("Fin de la tentative %d\r\n"), tentative);
		AffichageMessages(goto_message);

	} while (PressionFinale >= desorption.pression_finale && !derniere_dose);

	PointArretFonction();

	if (desorption.derniere_etape)
		Desorption_DerniereEtape(pParam);

	PointArretFonction();

	AffichageEtape(_T("Fin de la d�sorption\r\n"));
	AffichageMessages(_T("Fin de la d�sorption\r\n"));
}


void CManip_AutoGaz::Desorption_InitialisationPressionInitiale()
{
	CString etape;
	etape.Format(_T("D�sorption %d : Initialisation de la Pression Initiale"), tentative);
	AffichageEtape(etape);
	AffichageMessages(_T("Initialisation de la Pression Initiale\r\n"));

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
		AffichageMessages(_T("D�but Enl�vement de gaz dans le volume de r�f�rence\r\n"));
		CString etape;
		etape.Format(_T("D�sorption %d : Enl�vement de gaz dans le volume de r�f�rence %d"), tentative, enlevement);
		AffichageEtape(etape);

		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);

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

		temp_mess.Format(_T("Fin enl�vement de gaz %d\r\n"), enlevement);
		AffichageMessages(temp_mess);

		VerificationInjection();

		PointArretSousFonction();
	} while (PressionInitiale - PressionFinale <= delta_pression_demande &&
		PressionFinale > 0.1);
}


void CManip_AutoGaz::Desorption_EquilibreDansVolumeRef(LPVOID pParam)
{
	AffichageMessages(_T("Equilibre dans le volume de r�f�rence\r\n"));



	CString nom_etape;
	nom_etape.Format(_T("D�sorption %d : Equilibre dans le volume de r�f�rence"), tentative);
	LireEcrireAfficher(desorption.temps_volume, nom_etape, pParam);
	AffichageMessages(_T("Fin Equilibre dans le volume de r�f�rence\r\n"));
}


void CManip_AutoGaz::Desorption_DesorptionEchantillonEtEquilibre(LPVOID pParam)
{
	AffichageMessages(_T("D�sorption de l'�chantillon et �quilibre\r\n"));
	Ouverture_Vanne(5);
	CString nom_etape;
	nom_etape.Format(_T("D�sorption %d : D�sorption de l'�chantillon et �quilibre"), tentative);
	LireEcrireAfficher(desorption.temps_desorption, nom_etape, pParam);
	AffichageMessages(_T("Fin D�sorption de l'�chantillon et �quilibre\r\n"));
}

void CManip_AutoGaz::Desorption_DerniereEtape(LPVOID pParam)
{
	ToutFermer(); // V�rification, normalement tout est ferm�

	MettreEnRouteLaPompe();
	Ouverture_Vanne(8);
	Ouverture_Vanne(7);
	//Sleep(5000);
	AttenteSecondes(5);
	//Ouverture_Vanne(6);

	PointArretSousFonction();

	AffichageMessages(_T("Equilibre dans le volume de r�f�rence dans la derni�re �tape 1\r\n"));
	CString nom_etape = _T("D�sorption Derni�re Etape : Equilibre dans le volume de r�f�rence 1");
	LireEcrireAfficher(desorption.temps_volume, nom_etape, pParam);
	AffichageMessages(_T("Fin Equilibre dans le volume de r�f�rence dans la derni�re �tape 1\r\n"));

	ArreterLaPompe();
	Fermeture_Vanne(8);
	Fermeture_Vanne(7);

	PointArretSousFonction();

	AffichageMessages(_T("Equilibre dans le volume de r�f�rence dans la derni�re �tape 2\r\n"));
	/*CString*/ nom_etape = _T("D�sorption Derni�re Etape : Equilibre dans le volume de r�f�rence 2");
	LireEcrireAfficher(desorption.temps_volume, nom_etape, pParam);
	AffichageMessages(_T("Fin Equilibre dans le volume de r�f�rence dans la derni�re �tape 2\r\n"));

	PointArretSousFonction();

	Ouverture_Vanne(5);

	PointArretSousFonction();

	AffichageMessages(_T("D�sorption de l'�chantillon et �quilibre dans la derni�re �tape \r\n"));
	/*CString*/ nom_etape = _T("D�sorption Derni�re Etape : D�sorption de l'�chantillon et �quilibre");
	LireEcrireAfficher(desorption.temps_desorption, nom_etape, pParam);
	AffichageMessages(_T("Fin D�sorption de l'�chantillon et �quilibre dans la derni�re �tape \r\n"));

	PointArretSousFonction();

	Fermeture_Vanne(5);
	//Fermeture_Vanne(6);

	PointArretSousFonction();



}