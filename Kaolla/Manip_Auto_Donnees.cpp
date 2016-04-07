#include "StdAfx.h"
#include "Manip_Auto.h"

#include "ListeEtape.h"

using namespace std;


void CManip_Auto::DonneesAutoGrapheEtape()
{
	CString titre;
	int i;

	for(i=0;i<nb_etape;i++)
	{
		if(etape_en_cours==ListeEtape[i].Index)
		{
			titre=ListeEtape[i].Nom;
			break;
		}
	}

	if(i==nb_etape)
		titre.Format(_T("Etape inconnu : %d"),etape_en_cours);


	messageHandler.GraphDataAutoStep(temps_experience.TempsActuel(), numero_mesure - 1, etape_en_cours);
	messageHandler.GraphSetTitle(titre);
}


Donnees_General CManip_Auto::DonneesActuellesGeneral()
{	return CManip::DonneesActuellesGeneral();}

Donnees_Divers CManip_Auto::DonneesActuellesDivers()
{	return CManip::m_proprietes_experience->m_divers.donnees_actuelles;}


