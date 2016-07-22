#include "StdAfx.h"
#include "Automation.h"




/**********************************************************************
* Write the first section of an entete and save it
* Inputs: none
***********************************************************************/
void Automation::EnteteCreate()
{
	ofstream file;

	file.open(Automation::BuildFileName("txt", true).c_str(), ios_base::out);
	file.clear();
	file << EnteteBase(false);
	file << EnteteGeneral(false);

	if (experimentLocalSettings.experimentType == EXPERIMENT_TYPE_AUTO) {
		file << EnteteDivers(false);
		file << EnteteAdsorption(false);
		file << EnteteDesorption(false);
	}

	file.close();
}



/**********************************************************************
* Write the first section of an CSV entete and save it
* Inputs: none
***********************************************************************/
void Automation::EnteteCSVCreate()
{
	ofstream file;

	file.open(Automation::BuildFileName("csv", true).c_str(), ios_base::out);
	file.clear();
	file << EnteteBase(true);
	file << EnteteGeneral(true);
	if (experimentLocalSettings.experimentType == EXPERIMENT_TYPE_AUTO) {
		file << EnteteDivers(true);
		file << EnteteAdsorption(true);
		file << EnteteDesorption(true);
	}
	file.close();
}



/**********************************************************************
* Opens the measurement file for the first time and stores its link in the fileStream ofstream
* Also writes columns in the CSV
* Inputs: none
***********************************************************************/
void Automation::FileMeasurementOpen()
{
	// Create the file
	fileStream.open(BuildFileName("csv", false).c_str(), ios_base::out /*| ios::trunc*/);

	// Clear the file stream, pas le .csv, et on peut réitérer l'écriture en enlevant le caractère "fin de fichier"
	fileStream.clear();

	// Write column names
	fileStream << "N°mesure;";													  // Experiment dose
	fileStream << "Temps(s);";													  // Experiment time
	fileStream << "Calorimètre(W);";											  // Calorimeter value
	fileStream << "Basse Pression(Bar);";										  // Pressure low range
	fileStream << "Haute Pression(Bar);";										  // Pressure high range
	fileStream << "T°C Calo;";													  // Temperature calorimeter
	fileStream << "T°C Cage;";													  // Temperature enclosure
	fileStream << "T°C pièce;";													  // Temperature room
	fileStream << "Vanne 6";													  // Valve open
	fileStream << endl;															  // Next line
}



/**********************************************************************
* Closes the measurement file
* Inputs: none
***********************************************************************/
void Automation::FileMeasurementClose()
{
	fileStream.close();
}



/**********************************************************************
* Records a measurement
* Inputs: none
***********************************************************************/
void Automation::FileMeasurementRecord()
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo, "%.8E", experimentLocalData.resultCalorimeter);

	if (fileStream)
	{
		fileStream << experimentLocalData.experimentDose << ";";				   // Experiment dose
		fileStream << experimentLocalData.experimentTime << ";";				   // Experiment time
		fileStream << char_resultat_calo << ";";								   // Calorimeter value
		fileStream << experimentLocalData.pressureLow << ";";					   // Pressure low range
		fileStream << experimentLocalData.pressureHigh << ";";					   // Pressure high range
		fileStream << experimentLocalData.temperatureCalo << ";";				   // Temperature calorimeter
		fileStream << experimentLocalData.temperatureCage << ";";				   // Temperature enclosure
		fileStream << experimentLocalData.temperatureRoom << ";";				   // Temperature room
		fileStream << EstOuvert_Vanne(6) << ";";								   // Valve open
		fileStream << endl;														   // Next line
	}
}



/**********************************************************************
* Writes the base of the entete
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
string Automation::EnteteBase(bool csv)
{
	string divider;
	if (csv)
		divider = ";";
	else
		divider = " : ";

	ostringstream text("", ios_base::app);

	text << "Experiment type" << divider;
	text << experimentLocalSettings.experimentType;
	text << finl;

	return text.str();
}



/**********************************************************************
* Writes the settings from the general tab
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
string Automation::EnteteGeneral(bool csv)
{
	string divider;
	if (csv)
		divider = ";";
	else
		divider = " : ";

	ostringstream text("", ios_base::app);

	text << "Experimentateur"				<< divider		<< experimentLocalSettings.dataGeneral.experimentateur.nom								<< finl;
	text << "Date"							<< divider		<< experimentLocalSettings.dataGeneral.date_experience									<< finl;
	text << "Gaz"							<< divider		<< experimentLocalSettings.dataGeneral.gaz.symbole										<< finl;
	text << "Echantillon"					<< divider		<< experimentLocalSettings.dataGeneral.nom_echantillon									<< finl;
	text << "Masse"							<< divider		<< experimentLocalSettings.dataGeneral.masse_echantillon		<< divider << "g"		<< finl;
	text << "Température de l'expérience"	<< divider		<< experimentLocalSettings.dataGeneral.temperature_experience	<< divider << "°C"		<< finl;
	text << "Commentaires"					<< divider		<< experimentLocalSettings.dataGeneral.commentaires										<< finl;
	text << "Calorimètre"					<< divider		<< GetNomCalo()																			<< finl;
	text																																			<< finl;

	return text.str();
}



/**********************************************************************
* Writes the settings from the diverse tab
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
string Automation::EnteteDivers(bool csv)
{
	string divider;
	if (csv)
		divider = ";";
	else
		divider = " : ";

	ostringstream text("", ios_base::app);

	text << "Numéro de Cellule"				<< divider		<< experimentLocalSettings.dataDivers.cellule.numero									<< finl;
	text << "Volume du calo"				<< divider		<< experimentLocalSettings.dataDivers.cellule.volume_calo		<< divider << "cm3"		<< finl;
	text << "Volume total"					<< divider		<< experimentLocalSettings.dataDivers.cellule.volume_total		<< divider << "cm3"		<< finl;
	text << "Baseline time"					<< divider		<< experimentLocalSettings.dataDivers.temps_ligne_base			<< divider << "min"		<< finl;
	text << "Experiment end vacuum"			<< divider		<< experimentLocalSettings.dataDivers.mise_sous_vide_fin_experience						<< finl;
	text << "Vacuum time"					<< divider		<< experimentLocalSettings.dataDivers.temps_vide				<< divider << "min"		<< finl;
	text																																			<< finl;							

	return text.str();
}



/**********************************************************************
* Writes the adsorption steps
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
string Automation::EnteteAdsorption(bool csv)
{
	string divider;
	if (csv)
		divider = ";";
	else
		divider = " : ";

	ostringstream text("", ios_base::app);

	for (size_t i = 0; i < experimentLocalSettings.dataAdsorption.size; i++)
	{	
		text << "-----------------------------------------------------"																								<< finl;
		text << "Adsorption stage number"							<< divider	<< i + 1	 																		<< finl;
		text << "Delta pression"									<< divider	<< experimentLocalSettings.dataAdsorption[i].delta_pression		<< divider << "bar" << finl;
		text << "Temps de l'expérience dans le volume référenciel"	<< divider	<< experimentLocalSettings.dataAdsorption[i].temps_volume		<< divider << "min" << finl;
		text << "Temps de l'expérience dans l'adsorption "			<< divider	<< experimentLocalSettings.dataAdsorption[i].temps_adsorption	<< divider << "min" << finl;
		text << "Pression finale"									<< divider	<< experimentLocalSettings.dataAdsorption[i].pression_finale	<< divider << "bar" << finl;
		text																																						<< finl;	
	}

	return text.str();
}



/**********************************************************************
* Writes the desorption steps
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
string Automation::EnteteDesorption(bool csv)
{
	string divider;
	if (csv)
		divider = ";";
	else
		divider = " : ";

	ostringstream text("", ios_base::app);

	for (size_t i = 0; i < experimentLocalSettings.dataDesorption.size; i++)
	{	
		text << "-----------------------------------------------------"																								<< finl;
		text << "Desorption stage number"							<< divider	<< i + 1																			<< finl;
		text << "Delta pression"									<< divider	<< experimentLocalSettings.dataDesorption[i].delta_pression		<< divider << "bar" << finl;
		text << "Temps de l'expérience dans le volume référenciel"	<< divider	<< experimentLocalSettings.dataDesorption[i].temps_volume		<< divider << "min" << finl;
		text << "Temps de l'expérience dans l'adsorption "			<< divider	<< experimentLocalSettings.dataDesorption[i].temps_desorption	<< divider << "min" << finl;
		text << "Pression finale"									<< divider	<< experimentLocalSettings.dataDesorption[i].pression_finale	<< divider << "bar" << finl;
		text << "Dernière étape à partie du vide"					<< divider	<< experimentLocalSettings.dataDesorption[i].derniere_etape							<< finl;
		text																																						<< finl;	
	}

	return text.str();
}



void Automation::RecordDataChange(ExperimentSettings newSettings)
{
	ostringstream char_changement("", ios_base::app);

	char_changement << endl << "-----------------------------------------------------" << endl;

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



/**********************************************************************
* Returns the full path and title of the file to be written
* Inputs:
*        string extension: Extension you want the file to have
*        bool entete: specify true to get the entete string or false for the regular file
***********************************************************************/
std::string Automation::BuildFileName(std::string extension, bool entete)
{
	// Create buffer
	char fileNameBuffer[255];

	// Put path in buffer
	sprintf_s(fileNameBuffer, "%s", experimentLocalSettings.dataGeneral.chemin.c_str());

	// Check for validity, if not, put the file in the C: drive
	if (!PathIsDirectory(_T(fileNameBuffer)))
	{
		messageHandler.DisplayMessageBox(ERROR_PATHUNDEF, MB_ICONWARNING | MB_OK, false);
		sprintf_s(fileNameBuffer, "C:/");
	}

	// Check if the user field is empty
	if (strcmp(experimentLocalSettings.dataGeneral.experimentateur.surnom.c_str(), ""))		//??
	{
		sprintf_s(fileNameBuffer, "%s/Nouveau_Fichier", experimentLocalSettings.dataGeneral.chemin.c_str());
	}
	else
	{
		// Check if the user directory exists, if not, create it
		sprintf_s(fileNameBuffer, "%s/%s", experimentLocalSettings.dataGeneral.chemin.c_str(), experimentLocalSettings.dataGeneral.experimentateur.surnom.c_str());
		if (!PathIsDirectory(_T(fileNameBuffer))) {
			CreateDirectory(_T(fileNameBuffer), NULL);
		}
		// Check if the sample directory exists
		sprintf_s(fileNameBuffer, "%s/%s/%s", experimentLocalSettings.dataGeneral.chemin.c_str(), experimentLocalSettings.dataGeneral.experimentateur.surnom.c_str(),
			experimentLocalSettings.dataGeneral.nom_echantillon.c_str());
	}

	// Check if the full path exists, if not create it
	if (!PathIsDirectory(_T(fileNameBuffer))) {
		CreateDirectory(_T(fileNameBuffer), NULL);
	}

	// Finally store the entire path including the file name and return it
	if (entete)
	{
		sprintf_s(fileNameBuffer, "%s/%s(entete).%s", fileNameBuffer, experimentLocalSettings.dataGeneral.fichier.c_str(), extension.c_str());
	}
	else
	{
		sprintf_s(fileNameBuffer, "%s/%s.%s", fileNameBuffer, experimentLocalSettings.dataGeneral.fichier.c_str(), extension.c_str());
	}

	// Now return generated string
	string nom_fichier = fileNameBuffer;
	return nom_fichier;
}



//string CManip_AutoGaz::EnteteAdsorptionContinue()
//{
//	ostringstream chaine_char("", ios_base::app);
//
//	chaine_char << finl << "-----------------------------------------------------" << finl;
//	chaine_char << "Application de l'adsorption continue" << finl;
//	chaine_char << "Temps d'étalonnage débit : " << adsorption_continue.temps_etalonnage_debit << " min" << finl;
//	chaine_char << "Temps d'étalonnage du volume intermédiaire : " << adsorption_continue.temps_etalonnage_volume_inter << " min" << finl;
//	chaine_char << "Temps d'équilibre continue : " << adsorption_continue.temps_equilibre_continue << " min" << finl;
//	chaine_char << "Temps final d'équilibre : " << adsorption_continue.temps_equilibre_continue << " min" << finl;
//	chaine_char << "Pression finale : " << adsorption_continue.pression_finale_adsorption_continue << " bar" << finl;
//
//	return chaine_char.str();
//}
//
//string CManip_AutoGaz::EnteteAdsorptionContinueCSV()
//{
//	ostringstream chaine_char("", ios_base::app);
//
//	chaine_char << endl << "-----------------------------------------------------" << endl;
//	chaine_char << "Application de l'adsorption continue" << endl;
//	chaine_char << "Temps d'étalonnage débit;" << adsorption_continue.temps_etalonnage_debit << ";min" << endl;
//	chaine_char << "Temps d'étalonnage du volume intermédiaire;" << adsorption_continue.temps_etalonnage_volume_inter << ";min" << endl;
//	chaine_char << "Temps d'équilibre continue;" << adsorption_continue.temps_equilibre_continue << ";min" << endl;
//	chaine_char << "Temps final d'équilibre;" << adsorption_continue.temps_equilibre_continue << ";min" << endl;
//	chaine_char << "Pression finale;" << adsorption_continue.pression_finale_adsorption_continue << ";bar" << endl;
//
//	return chaine_char.str();
//}