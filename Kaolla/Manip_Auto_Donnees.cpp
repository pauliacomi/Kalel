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

	m_Doc->TempsMinimum = temps_experience.TempsActuel();
	m_Doc->MesureMinimum = numero_mesure-1;
	m_Doc->TitreGrapheEtape = titre;
	m_Doc->NumeroEtape = etape_en_cours;
}


void CManip_Auto::DonneesAutoGrapheEtape(float temps, int mesure, int num_etape)
{
	m_Doc->TempsMinimum = temps;
	m_Doc->MesureMinimum = mesure-1;
	m_Doc->NumeroEtape = num_etape;
}


Donnees_General CManip_Auto::DonneesActuellesGeneral()
{	return CManip::DonneesActuellesGeneral();}

Donnees_Divers CManip_Auto::DonneesActuellesDivers()
{	return CManip::m_proprietes_experience->m_divers.donnees_actuelles;}


