#include "StdAfx.h"
#include "Manip.h"



// Exchange the data between the class and the main window. This should be in a command format (rewrite)
void CManip::EchangeDonnees()
{
	m_KaollaView->m_StrCalo.Format(_T("%.8E"),resultat_calo);
	m_KaollaView->m_StrBassePression.Format(_T("%f"),resultat_bp);
	m_KaollaView->m_StrHautePression.Format(_T("%f"),resultat_hp);
	m_KaollaView->m_StrTemperatureCalo.Format(_T("%f"),TemperatureCalo);	
	m_KaollaView->m_StrTemperatureCage.Format(_T("%f"),TemperatureCage);
	m_KaollaView->m_StrTemperaturePiece.Format(_T("%f"),TemperaturePiece);
	m_KaollaView->m_StrTemps.Format(_T("%.2f"),temps_manip);
}

// Manually sets the graph data. The graph coefficients are defined here, although they shouldn't
void CManip::DonneesManuelleGrapheEtape()
{
	int nb_heures = 2;
	int nb_secondes = nb_heures * 3600;

	int coeff = (int)(temps_manip / nb_secondes);
	float temps_min = coeff * nb_secondes;


	if (m_Doc->NumeroEtape < coeff + 1)
	{
		m_Doc->TempsMinimum = temps_min;
		m_Doc->MesureMinimum = numero_mesure-1;
		m_Doc->NumeroEtape = coeff + 1;
		CString titre;
		titre.Format(_T("%dh - %dh d'expérience"), nb_heures * coeff , nb_heures*(coeff+1) );
		m_Doc->TitreGrapheEtape = titre;
	}
}



Donnees_General CManip::DonneesActuellesGeneral()
{	return m_proprietes_experience->m_general.donnees_actuelles;}

Donnees_Divers CManip::DonneesActuellesDivers()
{	return m_proprietes_experience->m_divers.donnees_actuelles;}

Donnees_Petites_Doses CManip::DonneesActuellesPetitesDoses()
{	return m_proprietes_experience->m_petitesdoses.donnees_actuelles;}

Donnees_Grandes_Doses CManip::DonneesActuellesGrandesDoses()
{	return m_proprietes_experience->m_grandesdoses.donnees_actuelles;}

Donnees_Desorption CManip::DonneesActuellesDesorption()
{	return m_proprietes_experience->m_desorption.donnees_actuelles;}

Donnees_Adsorption_Continue CManip::DonneesActuellesAdsorptionContinue()
{	return m_proprietes_experience->m_adsorptioncontinue.donnees_actuelles;}



void CManip::RecuperationDonnees()
{
	general = DonneesActuellesGeneral();
}

CString CManip::GetDonneesExperience()
{
	CString message;
	ostringstream message_stream("", ios_base::app);

	switch(TypeExperience)
	{
		case EXPERIMENT_TYPE_UNDEF :
			message.Format(ERROR_EXPUNDEF);
			break;
		case EXPERIMENT_TYPE_MANUAL :
			message_stream << EnteteBase();
			message = _T(message_stream.str().c_str());
			break;
		default :
			message.Format(ERROR_EXPDEFNERR,TypeExperience);
	}

	return message;
}