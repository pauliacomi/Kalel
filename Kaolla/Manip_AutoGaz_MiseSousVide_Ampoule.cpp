#include "StdAfx.h"
#include "Manip_AutoGaz.h"
#include "Define_Manip_AutoGaz_Mise_Sous_Vide.h"

using namespace std;

HANDLE hEventDebutAttente,hEventFinAttente,hEventConfirmationFinAttente;
HANDLE hThreadAttente;
DWORD ThreadAttenteId;

void CManip_AutoGaz::MiseSousVideAmpoule()
{

	//
	// Boite de dialogue
	//

	if(AfxMessageBox(_T("Voulez-vous mettre sous vide l'ampoule ?\t\n"), MB_YESNO | MB_ICONQUESTION)==IDNO)
	{
		ToutFermer();
		return;
	}
	
	AffichageMessages(_T("Début de la mise sous vide sur l'ampoule\r\n"));
	
	InitialisationManip();

	// Normalement, toutes les vannes sont fermées
	// Mais on les ferme toutes par mesure de sécurité

	ToutFermer();

	// Mettre en route la pompe
	MettreEnRouteLaPompe();

	Ouverture_Vanne(5);


	AffichageMessages(_T("Début Mise sous vide de la haute pression\r\n"));
	AffichageEtape(_T("Mise sous vide : Haute Pression"));

	do{
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);

		LireAfficherHautePression();
	}while(resultat_hp >= GetPressionSecuriteBassePression());

	AffichageMessages(_T("Fin Mise sous vide de la haute pression\r\n"));

	AffichageMessages(_T("Début Mise sous vide de la basse pression\r\n"));
	AffichageEtape(_T("Mise sous vide : Basse Pression"));

	Ouverture_Vanne(6);

	do{
		OuvrirEtFermer_Vanne(8);
		OuvrirEtFermer_Vanne(7);
		LireAfficherHautePression();
		LireAfficherBassePression();
	}while(resultat_hp >= pression_pompe);

	AffichageMessages(_T("Fin Mise sous vide de la basse pression\r\n"));

	Ouverture_Vanne(8);
	Ouverture_Vanne(7);

	AffichageEtape(_T("Fin de la Mise Sous Vide sur l'ampoule\r\n"));
	

	hEventDebutAttente=CreateEvent(NULL,TRUE,FALSE,NULL);
	hEventFinAttente=CreateEvent(NULL,TRUE,FALSE,NULL);
	hEventConfirmationFinAttente=CreateEvent(NULL,TRUE,FALSE,NULL);
	hThreadAttente=CreateThread(NULL,NULL,ThreadAttente,this,NULL,&ThreadAttenteId);
	
	SetEvent(hEventDebutAttente);

	AfxMessageBox(_T("Fin de la Mise Sous Vide de l'ampoule.\t\n"), MB_OK | MB_ICONWARNING);

	// Lancer Event pour arrêter le thread
	SetEvent(hEventFinAttente);

	WaitForSingleObject(hEventConfirmationFinAttente,INFINITE);


	ToutFermer();
	FermerInstruments();
	AffichageEtape(_T("\r\n"));

}



DWORD WINAPI CManip_AutoGaz::ThreadAttente(LPVOID lpParam)
{
	WaitForSingleObject(hEventDebutAttente,INFINITE);

	CManip_AutoGaz *manipulation = reinterpret_cast<CManip_AutoGaz *>(lpParam);

	do{
		manipulation->LireAfficherHautePression();
		manipulation->LireAfficherBassePression();
		manipulation->LireAfficherCalo(); 	
	}while(WaitForSingleObject(hEventFinAttente,100)==WAIT_TIMEOUT);
	
	SetEvent(hEventConfirmationFinAttente);

	return 0;
}
