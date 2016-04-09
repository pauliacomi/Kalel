#include "StdAfx.h"
#include "Manip.h"




std::string CManip::NomFichier(std::string extension, bool entete)
{
	// Create buffer
	char fileNameBuffer[255];
	// Put path in buffer
	sprintf_s(fileNameBuffer,"%s",general.chemin.c_str());

	// Check for validity, if not, put the file in the C: drive
	if(!PathIsDirectory(_T(fileNameBuffer)))
	{
		ASSERT(0);
		sprintf_s(fileNameBuffer,"C:/");
	}
	else
	{
		// Check if the user field is empty
		if( strcmp(general.experimentateur.surnom.c_str(),"") )
		{
			sprintf_s(fileNameBuffer,"%s/Nouveau_Fichier",general.chemin.c_str());
		}
		else
		{
			// Check if the user directory exists, if not, create it
			sprintf_s(fileNameBuffer,"%s/%s",general.chemin.c_str(),general.experimentateur.surnom.c_str());
			if (!PathIsDirectory(_T(fileNameBuffer))) {
				CreateDirectory(_T(fileNameBuffer), NULL);
			}
			// Check if the sample directory exists
			sprintf_s(fileNameBuffer,"%s/%s/%s",general.chemin.c_str(),general.experimentateur.surnom.c_str(),
												general.nom_echantillon.c_str());
		}
		// Check if the full path exists, if not create it
		if (!PathIsDirectory(_T(fileNameBuffer))) {
			CreateDirectory(_T(fileNameBuffer), NULL);
		}
	}

	// Finally store the entire path including the file name and return it
	if (entete)
	{
		sprintf_s(fileNameBuffer, "%s/%s(entete).%s", fileNameBuffer, general.fichier.c_str(), extension.c_str());
	}
	else
	{
		sprintf_s(fileNameBuffer, "%s/%s.%s", fileNameBuffer, general.fichier.c_str(), extension.c_str());
	}
	string nom_fichier = fileNameBuffer;
	return nom_fichier;
}


void CManip::OuvertureFichierMesures()
{
	// Create the file
	fileStream.open(NomFichier("csv", false).c_str(), ios_base::out /*| ios::trunc*/);

	// Clear the file stream, pas le .csv, et on peut réitérer l'écriture en enlevant le caractère "fin de fichier"
	fileStream.clear();

	// Write column names
	fileStream << "N°mesure;Temps(s);Calorimètre(W);Basse Pression(Bar);Haute Pression(Bar);T°C Calo;T°C Cage;T°C pièce;Vanne 6" << endl;
}


void CManip::FermetureFichierMesures()
{
	fileStream.close();
}


string CManip::EnteteBase()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << "Experience Manuelle" << finl;
	chaine_char << EnteteGeneral();

	return chaine_char.str();
}

void CManip::EnregistrementFichierMesures()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo,"%.8E",resultat_calo);

	if(fileStream)
	{
		fileStream << numero_mesure << ";" << temps_manip << ";";
		fileStream << char_resultat_calo << ";" << resultat_bp << ";" << resultat_hp << ";";
		fileStream << TemperatureCalo << ";" << TemperatureCage << ";" << TemperaturePiece << ";";

		fileStream << EstOuvert_Vanne(6) << ";";

		fileStream << endl;
	}
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
	fichier_entete.open(CManip::NomFichier("txt", true).c_str(), ios_base::out /*| ios::trunc*/);

	fichier_entete.clear(); 

	fichier_entete << EnteteBase();

	fichier_entete.close();
}

void CManip::EcritureEnteteCSV()
{
	ofstream fichier_entete; 
	fichier_entete.open(CManip::NomFichier("csv", true).c_str(), ios_base::out /*| ios::trunc*/);
	fichier_entete.clear(); 

	fichier_entete << EnteteBaseCSV();

	fichier_entete.close();

}

