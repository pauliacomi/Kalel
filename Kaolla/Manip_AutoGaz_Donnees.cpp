#include "StdAfx.h"
#include "Manip_AutoGaz.h"

#include "ListeEtape.h"

using namespace std;



void CManip_AutoGaz::RecuperationDonnees()
{
	general = DonneesActuellesGeneral();
	divers = DonneesActuellesDivers();
	petites_doses = DonneesActuellesPetitesDoses();
	moyennes_doses = DonneesActuellesMoyennesDoses();
	grandes_doses = DonneesActuellesGrandesDoses();
	desorption = DonneesActuellesDesorption();
	adsorption_continue = DonneesActuellesAdsorptionContinue();
}




Donnees_General CManip_AutoGaz::DonneesActuellesGeneral()
{	return CManip_Auto::DonneesActuellesGeneral();}

Donnees_Divers CManip_AutoGaz::DonneesActuellesDivers()
{	return CManip_Auto::DonneesActuellesDivers();}

Donnees_Petites_Doses CManip_AutoGaz::DonneesActuellesPetitesDoses()
{	return m_proprietes_experience->m_petitesdoses.donnees_actuelles;}

Donnees_Grandes_Doses CManip_AutoGaz::DonneesActuellesGrandesDoses()
{	return m_proprietes_experience->m_grandesdoses.donnees_actuelles;}

Donnees_Moyennes_Doses CManip_AutoGaz::DonneesActuellesMoyennesDoses()
{	return m_proprietes_experience->m_moyennesdoses.donnees_actuelles;}

Donnees_Desorption CManip_AutoGaz::DonneesActuellesDesorption()
{	return m_proprietes_experience->m_desorption.donnees_actuelles;}

Donnees_Adsorption_Continue CManip_AutoGaz::DonneesActuellesAdsorptionContinue()
{	return m_proprietes_experience->m_adsorptioncontinue.donnees_actuelles;}


CString CManip_AutoGaz::GetDonneesExperience()
{
	CString message;
	ostringstream message_stream("", ios_base::app);

	if(TypeExperience == AUTO)
	{
		message_stream << EnteteBase();

		if(adsorption_continue.a_effectuer)
		{
			message_stream << EnteteAdsorptionContinue();
		}

		if(petites_doses.a_effectuer)
		{
			message_stream << EntetePetitesDoses();
		}

		if(grandes_doses.a_effectuer)
		{
			message_stream << EnteteGrandesDoses();
		}

		if(moyennes_doses.a_effectuer)
		{
			message_stream << EnteteMoyennesDoses();
		}

		if(desorption.a_effectuer)
		{
			message_stream << EnteteDesorption();
		}

		message = _T(message_stream.str().c_str());

		return message;
	}

	//else
	return CManip::GetDonneesExperience();
}