#include "StdAfx.h"
#include "Manip_Auto.h"



using namespace std;


string CManip_Auto::EnteteBase()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << EnteteGeneral();
	chaine_char << EnteteDivers();


	return chaine_char.str();
}

string CManip_Auto::EnteteBaseCSV()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << EnteteGeneralCSV();
	chaine_char << EnteteDiversCSV();
	
	return chaine_char.str();
}


string CManip_Auto::EnteteDivers()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << finl << "Numéro de Cellule : " << divers.cellule.numero << finl;
	chaine_char << "Volume du calo : " << divers.cellule.volume_calo << " cm3" << finl;
	chaine_char << "Volume total : " << divers.cellule.volume_total << " cm3" << finl;
	chaine_char << "Temps de ligne de Base et d'équilibre : " << divers.temps_ligne_base << " min" << finl;
	if(divers.mise_sous_vide_fin_experience)
		chaine_char << "Temps de mise sous vide : " << divers.temps_vide << " min" << finl;
	else
		chaine_char << "Pas de mise sous vide en fin d'expérience" << finl;
	
	return chaine_char.str();
}

string CManip_Auto::EnteteDiversCSV()
{
	ostringstream chaine_char("", ios_base::app);
	
	chaine_char << endl << "Numéro de Cellule;" << divers.cellule.numero << endl;
	chaine_char << "Volume du calo;" << divers.cellule.volume_calo << ";cm3" << endl;
	chaine_char << "Volume total;" << divers.cellule.volume_total << ";cm3" << endl;
	chaine_char << "Temps de ligne de Base et d'équilibre;" << divers.temps_ligne_base << ";min" << endl;
	if(divers.mise_sous_vide_fin_experience)
		chaine_char << "Temps de mise sous vide;" << divers.temps_vide << ";min" << endl;
	else
		chaine_char << "Pas de mise sous vide en fin d'expérience;" << endl;
	
	return chaine_char.str();
}


void CManip_Auto::RajoutFichierEntete(CString StrChangement)
{
	string stringChangement = StrChangement;
	RajoutFichierEntete(stringChangement);
}

void CManip_Auto::RajoutFichierEntete(string stringChangement)
{
	ofstream fichier_entete;
	//fichier_entete.open(CManip::NomFichier("txt").c_str(),ios::out | ios::app);//ios::ate); //ios::ate : Permet de se placer en fin de fichier. 
	fichier_entete.open(CManip::NomFichierEntete("txt").c_str(),ios::out | ios::app);//ios::ate); //ios::ate : Permet de se placer en fin de fichier. 

	fichier_entete << stringChangement << endl;
	
	fichier_entete.close();
}


void CManip_Auto::RajoutFichierEnteteCSV(CString StrChangementCSV)
{
	string stringChangementCSV = StrChangementCSV;
	RajoutFichierEnteteCSV(stringChangementCSV);
}

void CManip_Auto::RajoutFichierEnteteCSV(string stringChangementCSV)
{
	ofstream fichier_entete;
	//fichier_entete.open(CManip::NomFichier("txt").c_str(),ios::out | ios::app);//ios::ate); //ios::ate : Permet de se placer en fin de fichier. 
	fichier_entete.open(CManip::NomFichierEntete("csv").c_str(),ios::out | ios::app);//ios::ate); //ios::ate : Permet de se placer en fin de fichier. 

	fichier_entete << stringChangementCSV << endl;
	
	fichier_entete.close();
}

