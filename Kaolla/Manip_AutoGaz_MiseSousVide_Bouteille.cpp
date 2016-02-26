#include "StdAfx.h"
#include "Manip_AutoGaz.h"
#include "Define_Manip_AutoGaz_Mise_Sous_Vide.h"

using namespace std;


void CManip_AutoGaz::MiseSousVideBouteille()
{
	if(AfxMessageBox(_T("Voulez-vous mettre sous vide jusqu'à la bouteille ?\t\n"), MB_YESNO | MB_ICONQUESTION)==IDNO)
	{
		ToutFermer();
		return;
	}

	InitialisationManip();
	
	if(MessageBoxConfirmation(_T("Fermer la bouteille !\tPuis appuyer sur OK\n"), MB_OKCANCEL | MB_ICONQUESTION)==IDCANCEL)
	{
		ToutFermer();
		FermerInstruments();
		return;
	}

	AffichageMessages(_T("Mise sous vide jusqu'à la bouteille\r\n"));
	AffichageEtape(_T("Mise sous vide jusqu'à la bouteille"));

	
	ProcedureMiseSousVideBouteille();
	
	AfxMessageBox(_T("Mise sous vide jusqu'à la bouteille terminée\t\n"), MB_OK);

	// Normalement, toutes les vannes sont fermées
	// Mais on les ferme toutes par mesure de sécurité

	ToutFermer();
	FermerInstruments();
	AffichageEtape(_T("\r\n"));
}


void CManip_AutoGaz::ProcedureMiseSousVideBouteille()
{
	AffichageMessages(_T("Procédure Mise sous vide de la bouteille\r\n"));
	// Mettre en route la pompe
	MettreEnRouteLaPompe();

	EvacuationDuSysteme();
	
	Ouverture_Vanne(4);
	//AttenteSecondes(5);
	
	//EvacuationDuSysteme();
	
	Ouverture_Vanne(3);
	Ouverture_Vanne(2);
	Ouverture_Vanne(1);
	//Sleep(5000);
	AttenteSecondes(5);	
	
	EvacuationDuSysteme();
	
	Ouverture_Vanne(8);
	Ouverture_Vanne(7);
	/*
	int tps = 10*60*1000; // 10 minutes
	Sleep(tps);
	*/
	AttenteMinutes(5);
	
	//Fermeture_Vanne(7);
	//Fermeture_Vanne(8);
	//Ouverture_Vanne(1);	
	
	//EvacuationDuSysteme();
	
	Ouverture_Vanne(6);
	//Ouverture_Vanne(8);
	//Ouverture_Vanne(7);	
	//Sleep(tps);
	AttenteMinutes(10);

	ToutFermer();
	AffichageMessages(_T("Fin de la procédure Mise sous vide de la bouteille\r\n"));
}

void CManip_AutoGaz::ProcedureMiseSousVideBouteille2()
{
	AffichageMessages(_T("Procédure Mise sous vide de la bouteille\r\n"));
	// Mettre en route la pompe
	MettreEnRouteLaPompe();

	EvacuationDuSysteme();
	
	Ouverture_Vanne(4);
	//AttenteSecondes(5);
	
	//EvacuationDuSysteme();
	
	Ouverture_Vanne(3);
	Ouverture_Vanne(2);
	Ouverture_Vanne(1);
	//Sleep(5000);
	AttenteSecondes(5);	
	
	EvacuationDuSysteme();
	
	Ouverture_Vanne(8);
	Ouverture_Vanne(7);
	/*
	int tps = 10*60*1000; // 10 minutes
	Sleep(tps);
	*/
	AttenteMinutes(10);
	
	//Fermeture_Vanne(7);
	//Fermeture_Vanne(8);
	//Ouverture_Vanne(1);	
	
	//EvacuationDuSysteme();
	

	ToutFermer();
	AffichageMessages(_T("Fin de la procédure Mise sous vide de la bouteille\r\n"));
}


void CManip_AutoGaz::EvacuationDuSysteme()
{
	do{
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);
		LireAfficherHautePression();
	}while(resultat_hp >= pression_pompe);	
}
