#include "StdAfx.h"
#include "Manip_Auto.h"
#include "Define_Manip_Auto_Verifications.h"

using namespace std;


int CManip_Auto::Verifications()
{	
	if(VerificationSecurite()== IDCANCEL)
		return IDCANCEL;
	if(VerificationVannes()== IDCANCEL)
		return IDCANCEL;
	if(VerificationPressionResiduelle()== IDCANCEL)
		return IDCANCEL;
	if(VerificationTemperature()== IDCANCEL)
		return IDCANCEL;

	return 0;
}




int CManip_Auto::VerificationSecurite()
{
	bool secu = GetActivationSecurite();
	if(!secu)
	{
		CString MessageInBox;
		MessageInBox = _T("ATTENTION PAS DE SECURITE !!!!!!!\t\nCONTINUER TOUT DE MEME ???\t\n");
		return AfxMessageBox(MessageInBox,MB_ICONWARNING | MB_OKCANCEL);
	}
	return 0;
}


int CManip_Auto::VerificationVannes()
{
	// vérification de l'état initial des vannes
	// Normalement, toutes les vannes sont fermées...
	AffichageMessages("Vérification de l'état initial\r\n");
	AffichageEtape("Vérification de l'état initial");
	CString MessageInBox;
	MessageInBox = _T("Vérifier que toutes les électrovannes sont en automatique et fermés\t\n");
	MessageInBox +=_T("et que la vanne sur l'ampoule est ouverte\t");
	
	int verif_etat_initial = AfxMessageBox(MessageInBox,MB_ICONQUESTION | MB_OKCANCEL);

	if (verif_etat_initial == IDCANCEL)
		return IDCANCEL;

	ToutFermer();

	return 0;
}




int CManip_Auto::VerificationPressionResiduelle()
{
	CString MessageInBox;
	// Vérification de la pression résiduelle
	AffichageMessages("Vérification de la pression résiduelle\r\n");
	AffichageEtape("Vérification de la pression résiduelle");

	LireAfficherHautePression();

	CString mess;
	if (resultat_hp < GetPressionSecuriteBassePression() && GetMesureBassePression() && GetMesureHautePression())
	{
		mess.Format(_T("Haute Pression : %f  Possibilité d'ouvrir la vanne 6\r\n"),resultat_hp);
		AffichageMessages(mess);
		Ouverture_Vanne(6);
		mess.Format(_T("Attente de %d secondes..."),Intervalle1/1000);//convertion en secondes
		AffichageMessages(mess);
		//Sleep(Intervalle1);
		AttenteSecondes(Intervalle1/1000);
	}
	Ouverture_Vanne(5);
	mess.Format(_T("Attente de %d secondes..."),Intervalle1/1000);//convertion en secondes
	AffichageMessages(mess);
	//Sleep(Intervalle1);
	AttenteSecondes(Intervalle1/1000);

	LireAfficherHautePression();

	if (resultat_hp >= GetPressionLimiteVide())
	{
		MessageInBox.Format(_T("La Haute Pression (= %f) est supérieure à la Pression limite du vide (= %f)\t\n"), resultat_hp,GetPressionLimiteVide());
		MessageInBox += _T("Voulez-vous tout de même continuer ?\t\n");
		int verif_pression = AfxMessageBox(MessageInBox,MB_ICONQUESTION | MB_OKCANCEL);
		if (verif_pression == IDCANCEL)
			return IDCANCEL;
	}

	return 0;
}



int CManip_Auto::VerificationTemperature()
{
	CString MessageInBox;
	// Vérification des températures
	AffichageMessages("Vérification des températures\r\n");
	AffichageEtape("Vérification des températures");

	etape_en_cours = TEMPERATURES;

	LectureTemperatures();
	AfficherTemperatures();


	float Texp = general.temperature_experience;
	// préciser quelle température dans la boite de dialogue ???
	if( (TemperatureCalo < Texp-1) || (TemperatureCalo > Texp+1) ||
		(TemperatureCage < Texp-1) || (TemperatureCage > Texp+1) )
	{
		MessageInBox  = _T("La température n'est pas prête\t\n");
		MessageInBox += _T("Voulez-vous attendre (oui/non) ou abandonner(annuler) ?\t");
		int verif_temperatures = AfxMessageBox(MessageInBox,MB_ICONQUESTION | MB_YESNOCANCEL);
		if(verif_temperatures == IDCANCEL)
			return IDCANCEL;
		if(verif_temperatures == IDYES)
		{
			AffichageMessages("Attente de la température\r\n");

			while( (TemperatureCalo < Texp-1) || (TemperatureCalo > Texp+1) ||
				   (TemperatureCage < Texp-1) || (TemperatureCage > Texp+1) )
			{
				int attente_pause = 500;//ms
				int attente_temperature = 10000;//ms
				for(int i=0;i < attente_temperature/attente_pause;i++)
				{
					PointArretTemperature();
					Sleep(attente_pause);
				}
				LectureTemperatures();
				AfficherTemperatures();
			}
		}
	}

	return 0;
}


