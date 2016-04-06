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
	textMessageBox += _T("\t5)Rin�age du syst�me 3 fois\t\n");
	textMessageBox += _T("\nVoulez-vous continuer ?\t\n");
	

	if(AfxMessageBox(textMessageBox, MB_YESNO)==IDNO)
	{
		ToutFermer();
		return;
	}
	
	InitialisationManip();

	messageHandler.DisplayMessage(_T("Changement de bouteille\r\n"));
	messageHandler.DisplayStep(_T("Changement de bouteille\r\n"));
	
	if(messageHandler.DisplayMessageBox(_T("Fermer la bouteille !\t\nPuis appuyer sur OK\t\n"), MB_OKCANCEL)==IDCANCEL)
	{
		ToutFermer();
		FermerInstruments();
		return;
	}

	ProcedureMiseSousVideBouteille();
	
	if(messageHandler.DisplayMessageBox(_T("Changer la bouteille -> Ouvrir le manom�tre\t\nPuis appuyer sur OK\t\n"), MB_OKCANCEL)==IDCANCEL)
	{
		ToutFermer();
		FermerInstruments();
		return;
	}
	
	ProcedureMiseSousVideBouteille();
	
	for(int i=1; i<=3; i++)
	{
		if(messageHandler.DisplayMessageBox(_T("Fermer le manom�tre\t\nOuvrir puis fermer la bouteille\t\nOuvrir le manom�tre 1 bar\t\n"), MB_OKCANCEL)==IDCANCEL)
		{
			ToutFermer();
			FermerInstruments();
			return;
		}
		
		if(i==1)
		{
			messageHandler.DisplayMessage(_T("Changement de bouteille : 1er rin�age\r\n"));
			messageHandler.DisplayStep(_T("Changement de bouteille : 1er rin�age\r\n"));
		}
		
		else
		{
			CString text;
			text.Format(_T("Changement de bouteille : %deme rin�age\r\n"),i);
			messageHandler.DisplayMessage(text);
			messageHandler.DisplayStep(text);
		}
		
		ProcedureMiseSousVideBouteille();
	}
	
	AfxMessageBox(_T("Fermer le manom�tre\t\nOuvrir la bouteille\t\nOuvrir le manom�tre � la pression requise\t\n"), MB_OK);
	
	AfxMessageBox(_T("Changement de bouteille termin�\t\n"), MB_OK);

	messageHandler.DisplayMessage(_T("Fin de changement de bouteille\r\n"));


	// Normalement, toutes les vannes sont ferm�es
	// Mais on les ferme toutes par mesure de s�curit�

	ToutFermer();
	FermerInstruments();
	messageHandler.DisplayStep(_T("\r\n"));
}

