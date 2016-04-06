#include "StdAfx.h"
#include "Manip_AutoGaz.h"

#include "ListeEtape.h"

using namespace std;


bool verif_existance_etape;
bool verif_existance_arret;



void CManip_AutoGaz::LireEcrireAfficher(int duree_evenement,CString nom_etape,LPVOID pParam)
{
	verif_existance_etape = FALSE;
	verif_existance_arret = FALSE;

	CString message;
	message.Format(_T("Début %s\r\n"),nom_etape);
	messageHandler.DisplayMessage(message);

	temps_intermediaire.TopChrono();
	CString Etape;

	bool continuer;

	do{
		Etape.Format(_T("%s   %s / %d min"), 
			nom_etape,temps_intermediaire.StrTempsActuel(), duree_evenement);
		messageHandler.DisplayStep(Etape);

		continuer = LireEcrireAfficher(pParam);

	}while(temps_intermediaire.TempsActuel() < duree_evenement * 60 && continuer == TRUE);

	//PointArretSansDoseFonction
	Etape.Format(_T("%s   Terminé"),nom_etape);
	message.Format(_T("Fin %s\r\n"),nom_etape);

	messageHandler.DisplayStep(Etape);
	messageHandler.DisplayMessage(message);

}



bool CManip_AutoGaz::LireEcrireAfficher(LPVOID pParam)
{
	CChrono temps_boucle;
	temps_boucle.TopChrono();

	ThreadMesures();

	temps_manip=temps_experience.TempsActuel();

	messageHandler.ExchangeData();
	RajoutMesure();
	AffichageMesures();
	EnregistrementFichierMesures();

	::PostMessage((HWND) pParam, WM_THREADAFFICHAGE, 0, 0);

	int i;
	int FctArretDemande = NULL;

	for(i=0;i<nb_etape;i++)
	{
		if(etape_en_cours==ListeEtape[i].Index)
		{
			FctArretDemande=ListeEtape[i].FctArret;
			break;
		}
	}
	if(i==nb_etape)
	{
		FctArretDemande=POINT_ARRET_NON_DEFINI;
		if(!verif_existance_etape)
		{
			CString message_alerte;
			message_alerte.Format(_T("ERREUR PROGRAMMATION : ETAPE %d NON DEFINIE DANS LireEcrireAfficher\r\n"),etape_en_cours);
			messageHandler.DisplayMessage(message_alerte);
			verif_existance_etape = TRUE;
		}
	}

	numero_mesure++;
	do{
		if(temps_boucle.TempsActuel()< T_BETWEEN_MEASURE)
		{
			Sleep(100);

			switch(FctArretDemande)
			{
				case POINT_ARRET_LEA:
					PointArretLEA();
					break;
				case POINT_ARRET_SANS_DOSE_LEA:
					PointArretSansDoseLEA();
					break;
				case POINT_ARRET_MISE_SOUS_VIDE_LEA:
					PointArretMiseSousVideLEA();
					break;
				case POINT_ARRET_NON_DEFINI:
					break;
				default:
					if(!verif_existance_arret)
					{
						CString message_alerte;
						message_alerte.Format(_T("ERREUR : DEMANDE ARRET %d NON DEFINIE DANS LireEcrireAfficher\r\n"),FctArretDemande);
						messageHandler.DisplayMessage(message_alerte);
						verif_existance_arret = TRUE;
					}
			}
		}
		else
			break;
	}while(TRUE);
	//PointArretSansDoseFonction

	return TRUE;
}