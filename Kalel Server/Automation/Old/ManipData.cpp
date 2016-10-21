#include "StdAfx.h"
#include "Manip.h"


// Manually sets the graph data. The graph coefficients are defined here, although they shouldn't
void CManip::DonneesManuelleGrapheEtape()
{
	int nb_heures = 2;
	int nb_secondes = nb_heures * 3600;

	int coeff = (int)(temps_manip / nb_secondes);
	float temps_min = coeff * nb_secondes;


	if (true /*m_Doc->NumeroEtape < coeff + 1*/)
	{
		ASSERT(0);
		messageHandler.GraphDataAutoStep(temps_min, numero_mesure - 1, coeff + 1);

		CString titre;
		titre.Format(_T("%dh - %dh d'expérience"), nb_heures * coeff , nb_heures*(coeff+1) );
		messageHandler.GraphSetTitle(titre);
	}
}

// ---------- Measurements ----------------

void CManip::DiplayMeasurements()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo, "%.8E", resultat_calo);

	ostringstream message;
	message << numero_mesure << ")  temps=" << temps_manip << "  ";
	message << "calo=" << char_resultat_calo << "  bp=" << resultat_bp << "  hp=" << resultat_hp << "  ";

	CString rajout_mesure;
	rajout_mesure.Format(_T("%s\r\n"), message.str().c_str());

	messageHandler.DisplayMeasurement(rajout_mesure);
}

void CManip::GraphAddMeasurement()
{
	messageHandler.GraphAddData(numero_mesure,
		temps_manip,
		resultat_calo,
		resultat_bp,
		resultat_hp,
		TemperatureCalo,
		TemperatureCage,
		TemperaturePiece);
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