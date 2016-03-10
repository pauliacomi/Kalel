#include "StdAfx.h"
#include "Manip_AutoGaz.h"


using namespace std;


bool CManip_AutoGaz::PetitesDosesAEffectuer()
{
	if(!ContinuerExperience())
		return false;

	Donnees_Petites_Doses recup_donnees = DonneesActuellesPetitesDoses();

	if(petites_doses.a_effectuer != recup_donnees.a_effectuer)
	{
		petites_doses = recup_donnees;
		
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	

		if(petites_doses.a_effectuer)
		{
			char_changement << "Rajout de l'étape : Petites Doses" << endl;
			char_changementCSV << "Rajout de l'étape : Petites Doses" << endl;
			
			char_changement << EntetePetitesDoses();
			char_changementCSV << EntetePetitesDosesCSV();

		}
		else 
		{
			char_changement << "Suppression de l'étape : Petites Doses" << endl;
			char_changementCSV << "Suppression de l'étape : Petites Doses" << endl;
		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
	return petites_doses.a_effectuer;
	
}


bool CManip_AutoGaz::MoyennesDosesAEffectuer()
{
	if(!ContinuerExperience())
		return false;

	Donnees_Moyennes_Doses recup_donnees = DonneesActuellesMoyennesDoses();
	if(moyennes_doses.a_effectuer != recup_donnees.a_effectuer)
	{
		moyennes_doses = recup_donnees;
		
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	

		if(moyennes_doses.a_effectuer)
		{
		
			char_changement << "Rajout de l'étape : Moyennes Doses" << endl;
			char_changementCSV << "Rajout de l'étape : Moyennes Doses" << endl;
			
			char_changement << EnteteMoyennesDoses();
			char_changementCSV << EnteteMoyennesDosesCSV();
		
		}
		else 
		{
			char_changement << "Suppression de l'étape : Moyennes Doses" << endl;
			char_changementCSV << "Suppression de l'étape : Moyennes Doses" << endl;
		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
	return moyennes_doses.a_effectuer;
}

bool CManip_AutoGaz::GrandesDosesAEffectuer()
{
	if(!ContinuerExperience())
		return false;

	Donnees_Grandes_Doses recup_donnees = DonneesActuellesGrandesDoses();
	if(grandes_doses.a_effectuer != recup_donnees.a_effectuer)
	{
		grandes_doses = recup_donnees;
		
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	

		if(grandes_doses.a_effectuer)
		{
		
			char_changement << "Rajout de l'étape : Grandes Doses" << endl;
			char_changementCSV << "Rajout de l'étape : Grandes Doses" << endl;
			
			char_changement << EnteteGrandesDoses();
			char_changementCSV << EnteteGrandesDosesCSV();
		
		}
		else 
		{
			char_changement << "Suppression de l'étape : Grandes Doses" << endl;
			char_changementCSV << "Suppression de l'étape : Grandes Doses" << endl;
		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
	return grandes_doses.a_effectuer;
}


bool CManip_AutoGaz::DesorptionAEffectuer()
{
	if(!ContinuerExperience())
		return false;

	Donnees_Desorption recup_donnees = DonneesActuellesDesorption();
	if(desorption.a_effectuer != recup_donnees.a_effectuer)
	{
		desorption = recup_donnees;
		
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	
		
		if(desorption.a_effectuer)
		{
			char_changement << "Rajout de l'étape : Désorption" << endl;
			char_changementCSV << "Rajout de l'étape : Désorption" << endl;
			
			char_changement << EnteteDesorption();
			char_changementCSV << EnteteDesorptionCSV();
		}
		else 
		{
			char_changement << "Suppression de l'étape : Désorption" << endl;
			char_changementCSV << "Suppression de l'étape : Désorption" << endl;
		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
	return desorption.a_effectuer;
}