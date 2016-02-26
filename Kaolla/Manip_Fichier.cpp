#include "StdAfx.h"
#include "Manip.h"


using namespace std;

ofstream fichier_mesure_manuelle;


string CManip::NomFichier(string extention)
{


	// Version 2

	char nom_fichier_char[255];
	sprintf_s(nom_fichier_char,"%s",general.chemin.c_str());

	if(!PathIsDirectory(_T(nom_fichier_char)))
	{
		//mettre erreur
		// Pour l'instant : 
		sprintf_s(nom_fichier_char,"C:/");
	}
	else
	{
		if(general.experimentateur.surnom.c_str() == "")
		{
			sprintf_s(nom_fichier_char,"%s/Nouveau_Fichier",general.chemin.c_str());
			if(!PathIsDirectory(_T(nom_fichier_char)))
				CreateDirectory(_T(nom_fichier_char),NULL);
		}
		else
		{
			sprintf_s(nom_fichier_char,"%s/%s",general.chemin.c_str(),general.experimentateur.surnom.c_str());
			if(!PathIsDirectory(_T(nom_fichier_char)))
				CreateDirectory(_T(nom_fichier_char),NULL);
			sprintf_s(nom_fichier_char,"%s/%s/%s",general.chemin.c_str(),general.experimentateur.surnom.c_str(),
												general.nom_echantillon.c_str());
			if(!PathIsDirectory(_T(nom_fichier_char)))
				CreateDirectory(_T(nom_fichier_char),NULL);
		}
	}
	sprintf_s(nom_fichier_char,"%s/%s.%s",nom_fichier_char,general.fichier.c_str(),extention.c_str());
	string nom_fichier = nom_fichier_char;
	return nom_fichier;
}

string CManip::NomFichierEntete(string extention)
{
	char nom_fichier_entete_char[255];
	sprintf_s(nom_fichier_entete_char,"%s",general.chemin.c_str());

	if(!PathIsDirectory(_T(nom_fichier_entete_char)))
	{
		//mettre erreur
		// Pour l'instant : 
		sprintf_s(nom_fichier_entete_char,"C:/");
	}
	else
	{
		if(general.experimentateur.surnom.c_str() == "")
		{
			sprintf_s(nom_fichier_entete_char,"%s/Nouveau_Fichier",general.chemin.c_str());
			if(!PathIsDirectory(_T(nom_fichier_entete_char)))
				CreateDirectory(_T(nom_fichier_entete_char),NULL);
		}
		else
		{
			sprintf_s(nom_fichier_entete_char,"%s/%s",general.chemin.c_str(),general.experimentateur.surnom.c_str());
			if(!PathIsDirectory(_T(nom_fichier_entete_char)))
				CreateDirectory(_T(nom_fichier_entete_char),NULL);
			sprintf_s(nom_fichier_entete_char,"%s/%s/%s",general.chemin.c_str(),general.experimentateur.surnom.c_str(),
												general.nom_echantillon.c_str());
			if(!PathIsDirectory(_T(nom_fichier_entete_char)))
				CreateDirectory(_T(nom_fichier_entete_char),NULL);
		}
	}
	sprintf_s(nom_fichier_entete_char,"%s/%s(entete).%s",nom_fichier_entete_char,general.fichier.c_str(),extention.c_str());
	string nom_fichier_entete = nom_fichier_entete_char;
	return nom_fichier_entete;
}



void CManip::EnregistrementFichierMesures()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo,"%.8E",resultat_calo);

	if(fichier_mesure_manuelle)
	{
		fichier_mesure_manuelle << numero_mesure << ";" << temps_manip << ";";
		fichier_mesure_manuelle << char_resultat_calo << ";" << resultat_bp << ";" << resultat_hp << ";";
		//fichier_mesure_manuelle << TemperatureCalo << ";" << TemperatureCage << ";" << TemperaturePiece << ";" << endl;
		fichier_mesure_manuelle << TemperatureCalo << ";" << TemperatureCage << ";" << TemperaturePiece << ";";

		fichier_mesure_manuelle << EstOuvert_Vanne(6) << ";";

		fichier_mesure_manuelle << endl;
	}
}




void CManip::OuvertureFichierMesures()
{
	//pDoc2 = m_KaollaView->GetDocument();

	fichier_mesure_manuelle.open(NomFichier("csv").c_str(), ios_base::out /*| ios::trunc*/);
	
	// vider le ofstream fichier, pas le .csv, et on peut réitérer l'écriture en enlevant le caractère "fin de fichier"
	fichier_mesure_manuelle.clear(); 
	// Ecriture des noms des colonnes
	//fichier_mesure_manuelle << "N°mesure;Temps(s);Calorimètre(W);Basse Pression(Bar);Haute Pression(Bar);T°C Calo;T°C Cage;T°C pièce" << endl;
	fichier_mesure_manuelle << "N°mesure;Temps(s);Calorimètre(W);Basse Pression(Bar);Haute Pression(Bar);T°C Calo;T°C Cage;T°C pièce;Vanne 6" << endl;
}


void CManip::FermetureFichierMesures()
{
	fichier_mesure_manuelle.close();
}

string CManip::EnteteBase()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << "Experience Manuelle" << finl;
	chaine_char << EnteteGeneral();

	return chaine_char.str();
}

string CManip::EnteteBaseCSV()
{
	ostringstream chaine_char("", ios_base::app);

	// Ecriture des noms des colonnes
	chaine_char << "Experience Manuelle" << endl;
	chaine_char << EnteteGeneralCSV();

	return chaine_char.str();
}

string CManip::EnteteGeneral()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << "Experimentateur : " << general.experimentateur.nom << finl;
	chaine_char << "Date : " << general.date_experience << finl;
	chaine_char << "Gaz : " << general.gaz.symbole << finl;
	chaine_char << "Echantillon : " << general.nom_echantillon << finl;
	chaine_char << "Masse : " << general.masse_echantillon << " g" << finl;
	chaine_char << "Température de l'expérience : " << general.temperature_experience << " °C" << finl;
	chaine_char << "Commentaires : " << general.commentaires << finl;
	chaine_char << "Calorimètre : " << GetNomCalo() << finl;

	return chaine_char.str();	
}

string CManip::EnteteGeneralCSV()
{
	ostringstream chaine_char("", ios_base::app);

	// Ecriture des noms des colonnes
	chaine_char << "Experimentateur; " << general.experimentateur.nom << endl;
	chaine_char << "Date;" << general.date_experience << endl;
	chaine_char << "Gaz;" << general.gaz.symbole << endl;
	chaine_char << "Echantillon;" << general.nom_echantillon << endl;
	chaine_char << "Masse;" << general.masse_echantillon << ";g" << endl;
	chaine_char << "Température de l'expérience;" << general.temperature_experience << ";°C" << endl;
	chaine_char << "Commentaires;" << general.commentaires << endl;
	chaine_char << "Calorimètre;" << GetNomCalo() << endl;

	return chaine_char.str();
}


void CManip::EcritureEntete()
{
	ofstream fichier_entete; 
	//fichier_entete.open(CManip::NomFichier("txt").c_str(), ios::out | ios::trunc);
	fichier_entete.open(CManip::NomFichierEntete("txt").c_str(), ios_base::out /*| ios::trunc*/);

	fichier_entete.clear(); 

	fichier_entete << EnteteBase();

	fichier_entete.close();
}

void CManip::EcritureEnteteCSV()
{
	ofstream fichier_entete; 
	fichier_entete.open(CManip::NomFichierEntete("csv").c_str(), ios_base::out /*| ios::trunc*/);
	fichier_entete.clear(); 

	fichier_entete << EnteteBaseCSV();

	fichier_entete.close();

}

