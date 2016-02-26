#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;


void CManip_AutoGaz::ChangementBouteille()
{
	CString textMessageBox;
	textMessageBox  = _T("Changement de la bouteille : \t\n");
	textMessageBox += _T("\t1)Fermer la bouteille en cours\t\n");
	textMessageBox += _T("\t2)Mise sous vide de la bouteille\t\n");
	textMessageBox += _T("\t3)Changement de bouteille\t\n");
	textMessageBox += _T("\t4)Mise sous vide sur la nouvelle bouteille\t\n");
	textMessageBox += _T("\t5)Rinçage du système 3 fois\t\n");
	textMessageBox += _T("\nVoulez-vous continuer ?\t\n");
	

	if(AfxMessageBox(textMessageBox, MB_YESNO)==IDNO)
	{
		ToutFermer();
		return;
	}
	
	InitialisationManip();

	AffichageMessages(_T("Changement de bouteille\r\n"));
	AffichageEtape(_T("Changement de bouteille\r\n"));
	
	if(MessageBoxConfirmation(_T("Fermer la bouteille !\t\nPuis appuyer sur OK\t\n"), MB_OKCANCEL)==IDCANCEL)
	{
		ToutFermer();
		FermerInstruments();
		return;
	}

	ProcedureMiseSousVideBouteille();
	
	if(MessageBoxConfirmation(_T("Changer la bouteille -> Ouvrir le manomètre\t\nPuis appuyer sur OK\t\n"), MB_OKCANCEL)==IDCANCEL)
	{
		ToutFermer();
		FermerInstruments();
		return;
	}
	
	ProcedureMiseSousVideBouteille();
	
	for(int i=1; i<=3; i++)
	{
		if(MessageBoxConfirmation(_T("Fermer le manomètre\t\nOuvrir puis fermer la bouteille\t\nOuvrir le manomètre 1 bar\t\n"), MB_OKCANCEL)==IDCANCEL)
		{
			ToutFermer();
			FermerInstruments();
			return;
		}
		
		if(i==1)
		{
			AffichageMessages(_T("Changement de bouteille : 1er rinçage\r\n"));
			AffichageEtape(_T("Changement de bouteille : 1er rinçage\r\n"));
		}
		
		else
		{
			CString text;
			text.Format(_T("Changement de bouteille : %deme rinçage\r\n"),i);
			AffichageMessages(text);
			AffichageEtape(text);
		}
		
		ProcedureMiseSousVideBouteille();
	}
	
	AfxMessageBox(_T("Fermer le manomètre\t\nOuvrir la bouteille\t\nOuvrir le manomètre à la pression requise\t\n"), MB_OK);
	
	AfxMessageBox(_T("Changement de bouteille terminé\t\n"), MB_OK);

	AffichageMessages(_T("Fin de changement de bouteille\r\n"));


	// Normalement, toutes les vannes sont fermées
	// Mais on les ferme toutes par mesure de sécurité

	ToutFermer();
	FermerInstruments();
	AffichageEtape(_T("\r\n"));
}

