#include "StdAfx.h"
#include "Automation.h"


std::string Automation::NomFichier(std::string extension, bool entete)
{
	// Create buffer
	char fileNameBuffer[255];
	// Put path in buffer
	sprintf_s(fileNameBuffer, "%s", experimentLocalData.dataGeneral.chemin.c_str());

	// Check for validity, if not, put the file in the C: drive
	if (!PathIsDirectory(_T(fileNameBuffer)))
	{
		ASSERT(0);
		sprintf_s(fileNameBuffer, "C:/");
	}
	else
	{
		// Check if the user field is empty
		if (strcmp(experimentLocalData.dataGeneral.experimentateur.surnom.c_str(), ""))
		{
			sprintf_s(fileNameBuffer, "%s/Nouveau_Fichier", experimentLocalData.dataGeneral.chemin.c_str());
		}
		else
		{
			// Check if the user directory exists, if not, create it
			sprintf_s(fileNameBuffer, "%s/%s", experimentLocalData.dataGeneral.chemin.c_str(), experimentLocalData.dataGeneral.experimentateur.surnom.c_str());
			if (!PathIsDirectory(_T(fileNameBuffer))) {
				CreateDirectory(_T(fileNameBuffer), NULL);
			}
			// Check if the sample directory exists
			sprintf_s(fileNameBuffer, "%s/%s/%s", experimentLocalData.dataGeneral.chemin.c_str(), experimentLocalData.dataGeneral.experimentateur.surnom.c_str(),
				experimentLocalData.dataGeneral.nom_echantillon.c_str());
		}
		// Check if the full path exists, if not create it
		if (!PathIsDirectory(_T(fileNameBuffer))) {
			CreateDirectory(_T(fileNameBuffer), NULL);
		}
	}

	// Finally store the entire path including the file name and return it
	if (entete)
	{
		sprintf_s(fileNameBuffer, "%s/%s(entete).%s", fileNameBuffer, experimentLocalData.dataGeneral.fichier.c_str(), extension.c_str());
	}
	else
	{
		sprintf_s(fileNameBuffer, "%s/%s.%s", fileNameBuffer, experimentLocalData.dataGeneral.fichier.c_str(), extension.c_str());
	}
	string nom_fichier = fileNameBuffer;
	return nom_fichier;
}


void Automation::FileMeasurementOpen()
{
	// Create the file
	fileStream.open(NomFichier("csv", false).c_str(), ios_base::out /*| ios::trunc*/);

	// Clear the file stream, pas le .csv, et on peut réitérer l'écriture en enlevant le caractère "fin de fichier"
	fileStream.clear();

	// Write column names
	fileStream << "N°mesure;Temps(s);Calorimètre(W);Basse Pression(Bar);Haute Pression(Bar);T°C Calo;T°C Cage;T°C pièce;Vanne 6" << endl;
}


void Automation::FileMeasurementClose()
{
	fileStream.close();
}


string Automation::EnteteBase()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << "Experience Manuelle" << finl;
	chaine_char << EnteteGeneral();

	return chaine_char.str();
}

void Automation::EnregistrementFichierMesures()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo, "%.8E", experimentLocalData.resultCalorimeter);

	if (fileStream)
	{
		fileStream << experimentLocalData.experimentDose << ";" << experimentLocalData.experimentTime << ";";
		fileStream << char_resultat_calo << ";" << experimentLocalData.pressureLow << ";" << experimentLocalData.pressureHigh << ";";
		fileStream << experimentLocalData.temperatureCalo << ";" << experimentLocalData.temperatureCage << ";" << experimentLocalData.temperatureRoom << ";";

		fileStream << EstOuvert_Vanne(6) << ";";

		fileStream << endl;
	}
}



string Automation::EnteteBaseCSV()
{
	ostringstream chaine_char("", ios_base::app);

	// Ecriture des noms des colonnes
	chaine_char << "Experience Manuelle" << endl;
	chaine_char << EnteteGeneralCSV();

	return chaine_char.str();
}

string Automation::EnteteGeneral()
{
	ostringstream chaine_char("", ios_base::app);

	chaine_char << "Experimentateur : " << experimentLocalData.dataGeneral.experimentateur.nom << finl;
	chaine_char << "Date : " << experimentLocalData.dataGeneral.date_experience << finl;
	chaine_char << "Gaz : " << experimentLocalData.dataGeneral.gaz.symbole << finl;
	chaine_char << "Echantillon : " << experimentLocalData.dataGeneral.nom_echantillon << finl;
	chaine_char << "Masse : " << experimentLocalData.dataGeneral.masse_echantillon << " g" << finl;
	chaine_char << "Température de l'expérience : " << experimentLocalData.dataGeneral.temperature_experience << " °C" << finl;
	chaine_char << "Commentaires : " << experimentLocalData.dataGeneral.commentaires << finl;
	chaine_char << "Calorimètre : " << GetNomCalo() << finl;

	return chaine_char.str();
}

string Automation::EnteteGeneralCSV()
{
	ostringstream chaine_char("", ios_base::app);

	// Ecriture des noms des colonnes
	chaine_char << "Experimentateur; " << experimentLocalData.dataGeneral.experimentateur.nom << endl;
	chaine_char << "Date;" << experimentLocalData.dataGeneral.date_experience << endl;
	chaine_char << "Gaz;" << experimentLocalData.dataGeneral.gaz.symbole << endl;
	chaine_char << "Echantillon;" << experimentLocalData.dataGeneral.nom_echantillon << endl;
	chaine_char << "Masse;" << experimentLocalData.dataGeneral.masse_echantillon << ";g" << endl;
	chaine_char << "Température de l'expérience;" << experimentLocalData.dataGeneral.temperature_experience << ";°C" << endl;
	chaine_char << "Commentaires;" << experimentLocalData.dataGeneral.commentaires << endl;
	chaine_char << "Calorimètre;" << GetNomCalo() << endl;

	return chaine_char.str();
}


void Automation::EcritureEntete()
{
	ofstream fichier_entete;
	fichier_entete.open(Automation::NomFichier("txt", true).c_str(), ios_base::out /*| ios::trunc*/);

	fichier_entete.clear();

	fichier_entete << EnteteBase();

	fichier_entete.close();
}

void Automation::EcritureEnteteCSV()
{
	ofstream fichier_entete;
	fichier_entete.open(Automation::NomFichier("csv", true).c_str(), ios_base::out /*| ios::trunc*/);
	fichier_entete.clear();

	fichier_entete << EnteteBaseCSV();

	fichier_entete.close();
}

void Automation::RecordDataChange()
{
	ostringstream char_changement("", ios_base::app);
	ostringstream char_changementCSV("", ios_base::app);

	char_changement << endl << "-----------------------------------------------------" << endl;
	char_changementCSV << endl << "-----------------------------------------------------" << endl;

	/*if (moyennes_doses.a_effectuer)
	{

		char_changement << "Rajout de l'étape : " << << endl;
		char_changementCSV << "Rajout de l'étape : "<< << endl;

		char_changement << EnteteMoyennesDoses();
		char_changementCSV << EnteteMoyennesDosesCSV();

	}
	else
	{
		char_changement << "Suppression de l'étape : " << << endl;
		char_changementCSV << "Suppression de l'étape :" << << endl;
	}

	string strChangement = char_changement.str();
	string strChangementCSV = char_changementCSV.str();

	RajoutFichierEntete(strChangement);
	RajoutFichierEnteteCSV(strChangementCSV);*/
}