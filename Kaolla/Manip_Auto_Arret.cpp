#include "StdAfx.h"
#include "Manip_Auto.h"

#include "ListeEtape.h"

using namespace std;

CEvent EventReprise;

void CManip_Auto::DemandePause()
{
	AffichageMessages(_T("Demande Pause\r\n"));
	demande_arret=PAUSE;
	etape_perturbe=etape_en_cours;
}

void CManip_Auto::DemandeMiseSousVide()
{
	AffichageMessages(_T("Demande d'arr�t sous vide\r\n"));
	demande_arret=ARRET_SOUSVIDE;
	etape_perturbe=etape_en_cours;
}

void CManip_Auto::ArretImmediat()
{
	AffichageMessages(_T("Demande d'arr�t imm�diat\r\n"));
	demande_arret=ARRET_IMMEDIAT;
	etape_perturbe=etape_en_cours;
}

void CManip_Auto::ProchaineCommande()
{
	AffichageMessages(_T("Demande d'arr�t de la commande\r\n"));
	demande_arret=ARRET_COMMANDE;
	etape_perturbe=etape_en_cours;
}

void CManip_Auto::ProchaineEtape()
{
	AffichageMessages(_T("Prochaine Etape demand�e\r\n"));
	demande_arret=ARRET_ETAPE;
	etape_perturbe=etape_en_cours;
}

void CManip_Auto::ProchaineDose()
{
	AffichageMessages(_T("Prochaine Dose demand�e\r\n"));
	demande_arret=ARRET_DOSE;
	etape_perturbe=etape_en_cours;
}

void CManip_Auto::MiseEnPause()
{
	if (demande_arret == PAUSE)
	{
		AffichageMessages(_T("Pause\r\n"));
		RajoutAffichageEtape(_T("  |  Pause"));
		temps_intermediaire.ArretTemps();
		WaitForSingleObject(EventReprise,INFINITE);
		//degriser
		temps_intermediaire.RepriseTemps();
		AffichageMessages(_T("Reprise\r\n"));
		AncienAffichageEtape();
	}
}

void CManip_Auto::Reprise()
{
	demande_arret = INACTIF;
	EventReprise.SetEvent();
}



void CManip_Auto::AffichageArret()
{
	
	CString etape,message;
	if(demande_arret != INACTIF) // Alors �criture dans editMessage pour pr�venir d'un arret
	{
		int i;
		for(i=0;i<nb_etape;i++)
		{
			if(etape_en_cours==ListeEtape[i].Index)
			{
				etape=ListeEtape[i].Nom;
				break;
			}
		}

		if(i==nb_etape)
			etape=_T("non d�finie");

		switch(demande_arret)
		{
			case ARRET_IMMEDIAT :
			case ARRET_SOUSVIDE :	
				if (etape_perturbe!=etape_en_cours)
				{
					message.Format(_T("Etape %s non effectu�e\r\n"), etape);
					break;
				}
			case ARRET_ETAPE :
				message.Format(_T("Etape %s interrompue\r\n"), etape);
				break;
			case ARRET_DOSE	 :
				message.Format(_T("Dose interrompue dans l'�tape %s\r\n"), etape);
				break;
			case ARRET_COMMANDE :
				message.Format(_T("Commande interrompue\r\n"));
				break;
			case ARRET_URGENCE_HP :
				if (etape_perturbe==etape_en_cours)
				{
					message.Format(_T("ARRET D'URGENCE : HAUTE PRESSION DEPASSANT LA PRESSION DE SECURITE : %f\r\n"),
						GetPressionSecuriteHautePression());
				}
				break;
			case ARRET_URGENCE_TCH : 
				if (etape_perturbe==etape_en_cours)
				{
					message.Format(_T("ARRET D'URGENCE : TEMPERATURE DU CALORIMETRE TROP ELEVE PAR RAPPORT A LA TEMPERATURE D'EXPERIENCE : %d\r\n"),
						general.temperature_experience);
				}
				break;
			case ARRET_URGENCE_TCB :
				if (etape_perturbe==etape_en_cours)
				{
					message.Format(_T("ARRET D'URGENCE : TEMPERATURE DU CALORIMETRE TROP FAIBLE PAR RAPPORT A LA TEMPERATURE D'EXPERIENCE : %d\r\n"),
						general.temperature_experience);
				}
				break;
			default:
				message.Format(_T("Arret non d�fini de %s\r\n"), etape);
				break;
		}

		// Emmettre un son peut-�tre
		AffichageMessages(message);

	}	

}



bool CManip_Auto::ContinuerExperience()
{
	switch(demande_arret)
	{
		case ARRET_IMMEDIAT :
		case ARRET_SOUSVIDE :
		case ARRET_URGENCE_HP :
		case ARRET_URGENCE_TCH :
		case ARRET_URGENCE_TCB :
			return FALSE;
		default : 
			break;
	}
	return TRUE;
}