#include "StdAfx.h"
#include "Manip_AutoGaz.h"



using namespace std;


string CManip_AutoGaz::EnteteBase()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << "Experience Automatique" << finl;
	chaine_char << CManip_Auto::EnteteBase();

	return chaine_char.str();
}

string CManip_AutoGaz::EnteteBaseCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << "Experience Automatique" << endl;
	chaine_char << CManip_Auto::EnteteBaseCSV();

	return chaine_char.str();
}



string CManip_AutoGaz::EnteteAdsorptionContinue()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << finl << "-----------------------------------------------------" << finl;
	chaine_char << "Application de l'adsorption continue" << finl;
	chaine_char <<  "Temps d'�talonnage d�bit : " << adsorption_continue.temps_etalonnage_debit << " min" << finl;
	chaine_char <<  "Temps d'�talonnage du volume interm�diaire : " << adsorption_continue.temps_etalonnage_volume_inter << " min"<< finl;
	chaine_char <<  "Temps d'�quilibre continue : " << adsorption_continue.temps_equilibre_continue << " min"<< finl;
	chaine_char <<  "Temps final d'�quilibre : " << adsorption_continue.temps_equilibre_continue << " min"<< finl;
	chaine_char <<  "Pression finale : " << adsorption_continue.pression_finale_adsorption_continue << " bar"<< finl;
	
	return chaine_char.str();
}

string CManip_AutoGaz::EnteteAdsorptionContinueCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << endl << "-----------------------------------------------------" << endl;
	chaine_char << "Application de l'adsorption continue" << endl;
	chaine_char <<  "Temps d'�talonnage d�bit;" << adsorption_continue.temps_etalonnage_debit << ";min" << endl;
	chaine_char <<  "Temps d'�talonnage du volume interm�diaire;" << adsorption_continue.temps_etalonnage_volume_inter << ";min"<< endl;
	chaine_char <<  "Temps d'�quilibre continue;" << adsorption_continue.temps_equilibre_continue << ";min"<< endl;
	chaine_char <<  "Temps final d'�quilibre;" << adsorption_continue.temps_equilibre_continue << ";min"<< endl;
	chaine_char <<  "Pression finale;" << adsorption_continue.pression_finale_adsorption_continue << ";bar"<< endl;
	
	return chaine_char.str();
}

string CManip_AutoGaz::EntetePetitesDoses()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << finl << "-----------------------------------------------------" << finl;
	chaine_char << "Remplissage par petites doses" << finl;
	chaine_char <<  "Delta pression par petites doses : " << petites_doses.delta_pression << " bar" << finl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel : " << petites_doses.temps_volume << " min"<< finl;
	chaine_char <<  "Temps de l'exp�rience dans l'adsorption : " << petites_doses.temps_adsorption << " min"<< finl;
	chaine_char <<  "Pression finale : " << petites_doses.pression_finale << " bar"<< finl;
	
	return chaine_char.str();
}

string CManip_AutoGaz::EntetePetitesDosesCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << endl << "-----------------------------------------------------" << endl;
	chaine_char << "Remplissage par petites doses" << endl;
	chaine_char <<  "Delta pression par petites doses;" << petites_doses.delta_pression << ";bar" << endl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel;" << petites_doses.temps_volume << ";min"<< endl;
	chaine_char <<  "Temps de l'exp�rience dans l'adsorption;" << petites_doses.temps_adsorption << ";min"<< endl;
	chaine_char <<  "Pression finale;" << petites_doses.pression_finale << ";bar"<< endl;
	
	return chaine_char.str();
}


string CManip_AutoGaz::EnteteMoyennesDoses()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << finl << "-----------------------------------------------------" << finl;
	chaine_char << "Remplissage par moyennes doses" << finl;
	chaine_char <<  "Delta pression par moyennes doses : " << moyennes_doses.delta_pression << " bar" << finl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel : " << moyennes_doses.temps_volume << " min"<< finl;
	chaine_char <<  "Temps de l'exp�rience dans l'adsorption : " << moyennes_doses.temps_adsorption << " min"<< finl;
	chaine_char <<  "Pression finale : " << moyennes_doses.pression_finale << " bar"<< finl;
	
	return chaine_char.str();
}

string CManip_AutoGaz::EnteteMoyennesDosesCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << endl << "-----------------------------------------------------" << endl;
	chaine_char << "Remplissage par moyennes doses" << endl;
	chaine_char <<  "Delta pression par moyennes doses;" << moyennes_doses.delta_pression << ";bar" << endl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel;" << moyennes_doses.temps_volume << ";min"<< endl;
	chaine_char <<  "Temps de l'exp�rience dans l'adsorption;" << moyennes_doses.temps_adsorption << ";min"<< endl;
	chaine_char <<  "Pression finale;" << moyennes_doses.pression_finale << ";bar"<< endl;
	
	return chaine_char.str();
}




string CManip_AutoGaz::EnteteGrandesDoses()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << finl << "-----------------------------------------------------" << finl;
	chaine_char << "Remplissage par grandes doses" << finl;
	chaine_char <<  "Delta pression par grandes doses : " << grandes_doses.delta_pression << " bar" << finl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel : " << grandes_doses.temps_volume << " min"<< finl;
	chaine_char <<  "Temps de l'exp�rience dans l'adsorption : " << grandes_doses.temps_adsorption << " min"<< finl;
	chaine_char <<  "Pression finale : " << grandes_doses.pression_finale << " bar"<< finl;
	
	return chaine_char.str();
}

string CManip_AutoGaz::EnteteGrandesDosesCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << endl << "-----------------------------------------------------" << endl;
	chaine_char << "Remplissage par grandes doses" << endl;
	chaine_char <<  "Delta pression par grandes doses;" << grandes_doses.delta_pression << ";bar" << endl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel;" << grandes_doses.temps_volume << ";min"<< endl;
	chaine_char <<  "Temps de l'exp�rience dans l'adsorption;" << grandes_doses.temps_adsorption << ";min"<< endl;
	chaine_char <<  "Pression finale;" << grandes_doses.pression_finale << ";bar"<< endl;
	
	return chaine_char.str();
}



string CManip_AutoGaz::EnteteDesorption()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << finl << "-----------------------------------------------------" << finl;
	chaine_char << "D�sorption" << finl;
	chaine_char <<  "Delta pression par d�sorption : " << desorption.delta_pression << " bar" << finl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel : " << desorption.temps_volume << " min"<< finl;
	chaine_char <<  "Temps de l'exp�rience dans la desorption : " << desorption.temps_desorption << " min"<< finl;
	chaine_char <<  "Pression finale : " << desorption.pression_finale << " bar"<< finl;
	if(desorption.derniere_etape)
		chaine_char <<  "Avec derni�re �tape � partie du vide" << finl;
	else
		chaine_char <<  "Sans derni�re �tape � partir du vide" << finl;
		
	return chaine_char.str();
}


string CManip_AutoGaz::EnteteDesorptionCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << endl << "-----------------------------------------------------" << endl;
	chaine_char << "D�sorption" << endl;
	chaine_char <<  "Delta pression par d�sorption;" << desorption.delta_pression << ";bar" << endl;
	chaine_char <<  "Temps de l'exp�rience dans le volume r�f�renciel;" << desorption.temps_volume << ";min"<< endl;
	chaine_char <<  "Temps de l'exp�rience dans la desorption;" << desorption.temps_desorption << ";min"<< endl;
	chaine_char <<  "Pression finale;" << desorption.pression_finale << ";bar"<< endl;
	if(desorption.derniere_etape)
		chaine_char <<  "Avec derni�re �tape � partie du vide" << endl;
	else
		chaine_char <<  "Sans derni�re �tape � partir du vide" << endl;
		
	return chaine_char.str();
}


void CManip_AutoGaz::EcritureEntete()
{
	ofstream fichier_entete; 

	fichier_entete.open(CManip::NomFichier("txt", true).c_str(), ios::out | ios::trunc);

	fichier_entete.clear(); 
	// Ecriture des noms des colonnes
	fichier_entete << EnteteBase();

	if(adsorption_continue.a_effectuer)
	{
		fichier_entete << EnteteAdsorptionContinue();
	}

	if(petites_doses.a_effectuer)
	{
		fichier_entete << EntetePetitesDoses();
	}

	if(moyennes_doses.a_effectuer)
	{
		fichier_entete << EnteteMoyennesDoses();
	}

	if(grandes_doses.a_effectuer)
	{
		fichier_entete << EnteteGrandesDoses();
	}

	if(desorption.a_effectuer)
	{
		fichier_entete << EnteteDesorption();
	}

	fichier_entete.close();
}


void CManip_AutoGaz::EcritureEnteteCSV()
{
	ofstream fichier_entete; 
	fichier_entete.open(CManip::NomFichier("csv", true).c_str(), ios::out | ios::trunc);

	fichier_entete.clear(); 
	// Ecriture des noms des colonnes
	fichier_entete << EnteteBaseCSV();

	if(adsorption_continue.a_effectuer)
	{
		fichier_entete << EnteteAdsorptionContinueCSV();
	}

	if(petites_doses.a_effectuer)
	{
		fichier_entete << EntetePetitesDosesCSV();
	}

	if(moyennes_doses.a_effectuer)
	{
		fichier_entete << EnteteMoyennesDosesCSV();
	}

	if(grandes_doses.a_effectuer)
	{
		fichier_entete << EnteteGrandesDosesCSV();
	}



	if(desorption.a_effectuer)
	{
		fichier_entete << EnteteDesorptionCSV();
	}

	fichier_entete.close();
}


// Rajout pour bouton parametres

void CManip_AutoGaz::RecuperationDonneesPetitesDoses()
{
	Donnees_Petites_Doses recup_donnees = DonneesActuellesPetitesDoses();
	if (petites_doses != recup_donnees)
	{
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	
		
		char_changement << "Changement des donn�es de l'�tape : Petites Doses � partir de la dose " << dose << endl;
		char_changementCSV << "Changement des donn�es de l'�tape : Petites Doses � partir de la dose " << dose << endl;

		if (petites_doses.delta_pression != recup_donnees.delta_pression)
		{
			petites_doses.delta_pression = recup_donnees.delta_pression;
			
			char_changement << "Delta pression par petites doses : " << petites_doses.delta_pression << " bar" << endl;
			char_changementCSV << "Delta pression par petites doses;" << petites_doses.delta_pression << ";bar" << endl;
		}
		
		if(petites_doses.temps_volume != recup_donnees.temps_volume)
		{
			petites_doses.temps_volume = recup_donnees.temps_volume;
			
			char_changement << "Temps de l'exp�rience dans le volume r�f�renciel : " << petites_doses.temps_volume << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans le volume r�f�renciel;" << petites_doses.temps_volume << ";min" << endl;
		}
		
		if(petites_doses.temps_adsorption != recup_donnees.temps_adsorption)
		{
			petites_doses.temps_adsorption = recup_donnees.temps_adsorption;
			
			char_changement << "Temps de l'exp�rience dans l'adsorption : " << petites_doses.temps_adsorption << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans l'adsorption;" << petites_doses.temps_adsorption << ";min" << endl;
		}
		
		if(petites_doses.pression_finale != recup_donnees.pression_finale)
		{
			petites_doses.pression_finale = recup_donnees.pression_finale;
			
			char_changement << "Pression finale : " << petites_doses.pression_finale << " bar" << endl;
			char_changementCSV << "Pression finale;" << petites_doses.pression_finale << ";bar" << endl;
			
		}

		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
}


void CManip_AutoGaz::RecuperationDonneesMoyennesDoses()
{
	Donnees_Moyennes_Doses recup_donnees = DonneesActuellesMoyennesDoses();
	if (moyennes_doses != recup_donnees)
	{
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	
		
		char_changement << "Changement des donn�es de l'�tape : Moyennes Doses � partir de la dose " << dose << endl;
		char_changementCSV << "Changement des donn�es de l'�tape : Moyennes Doses � partir de la dose " << dose << endl;
		
		
		if (moyennes_doses.delta_pression != recup_donnees.delta_pression)
		{
			moyennes_doses.delta_pression = recup_donnees.delta_pression;
			
			char_changement << "Delta pression par moyennes doses : " << moyennes_doses.delta_pression << " bar" << endl;
			char_changementCSV << "Delta pression par moyennes doses;" << moyennes_doses.delta_pression << ";bar" << endl;
		}
		
		if(moyennes_doses.temps_volume != recup_donnees.temps_volume)
		{
			moyennes_doses.temps_volume = recup_donnees.temps_volume;

			char_changement << "Temps de l'exp�rience dans le volume r�f�renciel : " << moyennes_doses.temps_volume << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans le volume r�f�renciel;" << moyennes_doses.temps_volume << ";min" << endl;
		}
		
		if(moyennes_doses.temps_adsorption != recup_donnees.temps_adsorption)
		{
			moyennes_doses.temps_adsorption = recup_donnees.temps_adsorption;
			
			char_changement << "Temps de l'exp�rience dans l'adsorption : " << moyennes_doses.temps_adsorption << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans l'adsorption;" << moyennes_doses.temps_adsorption << ";min" << endl;
		}
		
		if(moyennes_doses.pression_finale != recup_donnees.pression_finale)
		{
			moyennes_doses.pression_finale = recup_donnees.pression_finale;
			
			char_changement << "Pression finale : " << moyennes_doses.pression_finale << " bar" << endl;
			char_changementCSV << "Pression finale;" << moyennes_doses.pression_finale << ";bar" << endl;
		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
}




void CManip_AutoGaz::RecuperationDonneesGrandesDoses()
{
	Donnees_Grandes_Doses recup_donnees = DonneesActuellesGrandesDoses();
	if (grandes_doses != recup_donnees)
	{
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	
		
		char_changement << "Changement des donn�es de l'�tape : Grandes Doses � partir de la dose " << dose << endl;
		char_changementCSV << "Changement des donn�es de l'�tape : Grandes Doses � partir de la dose " << dose << endl;
		
		
		if (grandes_doses.delta_pression != recup_donnees.delta_pression)
		{
			grandes_doses.delta_pression = recup_donnees.delta_pression;
			
			char_changement << "Delta pression par grandes doses : " << grandes_doses.delta_pression << " bar" << endl;
			char_changementCSV << "Delta pression par grandes doses;" << grandes_doses.delta_pression << ";bar" << endl;
		}
		
		if(grandes_doses.temps_volume != recup_donnees.temps_volume)
		{
			grandes_doses.temps_volume = recup_donnees.temps_volume;

			char_changement << "Temps de l'exp�rience dans le volume r�f�renciel : " << grandes_doses.temps_volume << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans le volume r�f�renciel;" << grandes_doses.temps_volume << ";min" << endl;
		}
		
		if(grandes_doses.temps_adsorption != recup_donnees.temps_adsorption)
		{
			grandes_doses.temps_adsorption = recup_donnees.temps_adsorption;
			
			char_changement << "Temps de l'exp�rience dans l'adsorption : " << grandes_doses.temps_adsorption << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans l'adsorption;" << grandes_doses.temps_adsorption << ";min" << endl;
		}
		
		if(grandes_doses.pression_finale != recup_donnees.pression_finale)
		{
			grandes_doses.pression_finale = recup_donnees.pression_finale;
			
			char_changement << "Pression finale : " << grandes_doses.pression_finale << " bar" << endl;
			char_changementCSV << "Pression finale;" << grandes_doses.pression_finale << ";bar" << endl;
		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
	
}





void CManip_AutoGaz::RecuperationDonneesDesorption()
{
	Donnees_Desorption recup_donnees = DonneesActuellesDesorption();
	if(desorption != recup_donnees)
	{
	
		ostringstream char_changement("", ios_base::app);
		ostringstream char_changementCSV("", ios_base::app);
		
		char_changement << endl << "-----------------------------------------------------" << endl;
		char_changementCSV << endl << "-----------------------------------------------------" << endl;	
		
		char_changement << "Changement des donn�es de l'�tape : D�sorption � partir de la tentative " << tentative << endl;
		char_changementCSV << "Changement des donn�es de l'�tape : D�sorption � partir de la tentative " << tentative << endl;

		if (desorption.delta_pression != recup_donnees.delta_pression)
		{
			desorption.delta_pression = recup_donnees.delta_pression;
			
			char_changement << "Delta pression par d�sorption : " << desorption.delta_pression << " bar" << endl;
			char_changementCSV << "Delta pression par d�sorption;" << desorption.delta_pression << ";bar" << endl;
		}
		
		if(desorption.temps_volume != recup_donnees.temps_volume)
		{
			desorption.temps_volume = recup_donnees.temps_volume;
			
			char_changement << "Temps de l'exp�rience dans le volume r�f�renciel : " << desorption.temps_volume << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans le volume r�f�renciel;" << desorption.temps_volume << ";min" << endl;
		}
		
		if(desorption.temps_desorption != recup_donnees.temps_desorption)
		{
			desorption.temps_desorption = recup_donnees.temps_desorption;
			
			char_changement << "Temps de l'exp�rience dans la d�sorption : " << desorption.temps_desorption << " min" << endl;
			char_changementCSV << "Temps de l'exp�rience dans la d�sorption;" << desorption.temps_desorption << ";min" << endl;
		}
		
		if(desorption.pression_finale != recup_donnees.pression_finale)
		{
			desorption.pression_finale = recup_donnees.pression_finale;
			
			char_changement << "Pression finale : " << desorption.pression_finale << " bar" << endl;
			char_changementCSV << "Pression finale;" << desorption.pression_finale << ";bar" << endl;
		}

		if(desorption.derniere_etape != recup_donnees.derniere_etape)
		{
			desorption.derniere_etape = recup_donnees.derniere_etape;
			if(desorption.derniere_etape)
			{
				char_changement << "Avec derni�re �tape � partie du vide" << endl;
				char_changementCSV << "Avec derni�re �tape � partie du vide" << endl;
			}
			else
			{
				char_changement << "Sans derni�re �tape � partie du vide" << endl;
				char_changementCSV << "Sans derni�re �tape � partie du vide" << endl;
			}

		}
		
		string strChangement = char_changement.str();
		string strChangementCSV = char_changementCSV.str();
		
		RajoutFichierEntete(strChangement);
		RajoutFichierEnteteCSV(strChangementCSV);
	}
}




