#include "../stdafx.h"
#include "FileWriter.h"

// Resources
#include "../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../Kalel Shared/Com Classes/ExperimentSettings.h"	
#include "../../Kalel Shared/Resources/DefineStages.h"						// All stage, experiment type definitions are here

// Synchronization classes
#include "../MessageHandler.h"												// Handles all the messages from this class to the client
#include "../../Kalel Shared/Com Classes/ExperimentData.h"

// FileWriter and manipulation classes
#include "../Backend/Wrapper Classes/Vannes.h"								// Controlling valves


FileWriter::FileWriter()
{
}


FileWriter::~FileWriter()
{
}


/**********************************************************************
* Write the first section of an entete and save it
* Inputs:
*		const ExperimentSettings &expSettings:	Reference to the experimentSettings which generates the entete
*		const MachineSettings &machSettings:	Reference to the MachineSettings which generates the entete
***********************************************************************/
void FileWriter::EnteteCreate(const ExperimentSettings &expSettings, const MachineSettings &machSettings)
{
	std::wofstream file;

	file.open(FileWriter::BuildFileName(L"txt", expSettings.dataGeneral, true).c_str(), std::ios_base::out);
	file.clear();
	file << EnteteBase(false, expSettings.experimentType);
	file << EnteteGeneral(false, expSettings.dataGeneral, machSettings.CaloName);

	if (expSettings.experimentType == EXPERIMENT_TYPE_AUTO) {
		file << EnteteDivers(false, expSettings.dataDivers);
		file << EnteteAdsorption(false, expSettings.dataAdsorption);
		file << EnteteDesorption(false, expSettings.dataDesorption);
	}

	file.close();
}



/**********************************************************************
* Write the first section of an CSV entete and save it
* Inputs:
*		const ExperimentSettings &expSettings:	Reference to the experimentSettings which generates the entete
*		const MachineSettings &machSettings:	Reference to the MachineSettings which generates the entete
***********************************************************************/
void FileWriter::EnteteCSVCreate(const ExperimentSettings &expSettings, const MachineSettings &machSettings)
{
	std::wofstream file;

	file.open(FileWriter::BuildFileName(L"csv", expSettings.dataGeneral, true).c_str(), std::ios_base::out);
	file.clear();
	file << EnteteBase(true, expSettings.experimentType);
	file << EnteteGeneral(true, expSettings.dataGeneral, machSettings.CaloName);

	if (expSettings.experimentType == EXPERIMENT_TYPE_AUTO) {
		file << EnteteDivers(true, expSettings.dataDivers);
		file << EnteteAdsorption(true, expSettings.dataAdsorption);
		file << EnteteDesorption(true, expSettings.dataDesorption);
	}
	file.close();
}



/**********************************************************************
* Opens the FileWriter file for the first time and stores its link in the fileStream std::ofstream
* Also writes columns in the CSV
* Inputs: 
*		const Donnees_General &general:		Reference to the general data to be checked
***********************************************************************/
void FileWriter::FileMeasurementOpen(const Donnees_General &general)
{
	// Create the file
	fileStream.open(BuildFileName(L"csv", general, false).c_str(), std::ios_base::out /*| ios::trunc*/);

	// Clear the file stream, pas le .csv, et on peut réitérer l'écriture en enlevant le caractère "fin de fichier"
	fileStream.clear();

	// Write column names
	fileStream << "N°mesure;";													// Experiment dose
	fileStream << "Temps(s);";													// Experiment time
	fileStream << "Calorimètre(W);";											// Calorimeter value
	fileStream << "Basse Pression(Bar);";										// Pressure low range
	fileStream << "Haute Pression(Bar);";										// Pressure high range
	fileStream << "T°C Calo;";													// Temperature calorimeter
	fileStream << "T°C Cage;";													// Temperature enclosure
	fileStream << "T°C pièce;";													// Temperature room
	fileStream << "Vanne 6";													// Valve open
	fileStream << std::endl;													// Next line
}



/**********************************************************************
* Closes the FileWriter file
* Inputs: none
***********************************************************************/
void FileWriter::FileMeasurementClose()
{
	fileStream.close();
}



/**********************************************************************
* Records a measurement
* Inputs:
*		Reference to the experimentSettings which generates the entete
*		bool valveOpen6: records if valve number 6 is open or not
***********************************************************************/
void FileWriter::FileMeasurementRecord(const ExperimentData &data, bool valveOpen6)
{
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo, "%.8E", data.resultCalorimeter);

	if (fileStream)
	{
		fileStream << data.experimentDose						<< ";";				// Experiment dose
		fileStream << data.timeElapsed							<< ";";				// Experiment time
		fileStream << char_resultat_calo						<< ";";				// Calorimeter value
		fileStream << data.pressureLow							<< ";";				// Pressure low range
		fileStream << data.pressureHigh							<< ";";				// Pressure high range
		fileStream << data.temperatureCalo						<< ";";				// Temperature calorimeter
		fileStream << data.temperatureCage						<< ";";				// Temperature enclosure
		fileStream << data.temperatureRoom						<< ";";				// Temperature room
		fileStream << valveOpen6								<< ";";				// Valve open
		fileStream << std::endl;													// Next line
	}
}



/**********************************************************************
* Writes the base of the entete
* Inputs:
*        bool csv: Ask for a comma separated value format if true
*		 int experimentType: Type of experiment
***********************************************************************/
std::wstring FileWriter::EnteteBase(bool csv, int experimentType)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	text << "Experiment type" << divider;
	text << experimentType;
	text << std::endl;

	return text.str();
}



/**********************************************************************
* Writes the settings from the general tab
* Inputs:
*       bool csv: Ask for a comma separated value format if true
*		const Donnees_General &general:		Reference to the general data to be written
*		std::wstring caloName: name of the calorimeter
***********************************************************************/
std::wstring FileWriter::EnteteGeneral(bool csv, const Donnees_General &general, std::wstring caloName)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	text << L"Experimentateur"				<< divider		<< general.experimentateur.nom								<< std::endl;
	text << L"Date"							<< divider		<< general.date_experience									<< std::endl;
	text << L"Gaz"							<< divider		<< general.gaz.symbole										<< std::endl;
	text << L"Echantillon"					<< divider		<< general.nom_echantillon									<< std::endl;
	text << L"Masse"						<< divider		<< general.masse_echantillon		<< divider << L"g"		<< std::endl;
	text << L"Température de l'expérience"	<< divider		<< general.temperature_experience	<< divider << L"°C"		<< std::endl;
	text << L"Commentaires"					<< divider		<< general.commentaires										<< std::endl;
	text << L"Calorimètre"					<< divider		<< caloName.c_str()											<< std::endl;
	text																												<< std::endl;

	return text.str();
}



/**********************************************************************
* Writes the settings from the diverse tab
* Inputs:
*       bool csv: Ask for a comma separated value format if true
*		const Donnees_Divers &divers:	Reference to the diverse data to be written
***********************************************************************/
std::wstring FileWriter::EnteteDivers(bool csv, const Donnees_Divers &divers)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	text << L"Numéro de Cellule"			<< divider		<< divers.cellule.numero									<< std::endl;
	text << L"Volume du calo"				<< divider		<< divers.cellule.volume_calo		<< divider << "cm3"		<< std::endl;
	text << L"Volume total"					<< divider		<< divers.cellule.volume_total		<< divider << "cm3"		<< std::endl;
	text << L"Baseline time"				<< divider		<< divers.temps_ligne_base			<< divider << "min"		<< std::endl;
	text << L"Experiment end vacuum"		<< divider		<< divers.mise_sous_vide_fin_experience						<< std::endl;
	text << L"Vacuum time"					<< divider		<< divers.temps_vide				<< divider << "min"		<< std::endl;
	text																												<< std::endl;							

	return text.str();
}



/**********************************************************************
* Writes the adsorption steps
* Inputs:
*       bool csv: Ask for a comma separated value format if true
*		const std::vector<Donnees_Doses> &doses:	Reference to the adsorption data to be written
***********************************************************************/
std::wstring FileWriter::EnteteAdsorption(bool csv, const std::vector<Donnees_Doses> &doses)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	for (size_t i = 0; i < doses.size(); i++)
	{	
		text <<L"-----------------------------------------------------"																	<< std::endl;
		text <<L"Adsorption stage number"							<< divider	<< i + 1	 											<< std::endl;
		text <<L"Delta pression"									<< divider	<< doses[i].delta_pression		<< divider << "bar"		<< std::endl;
		text <<L"Temps de l'expérience dans le volume référenciel"	<< divider	<< doses[i].temps_volume		<< divider << "min"		<< std::endl;
		text <<L"Temps de l'expérience dans l'adsorption "			<< divider	<< doses[i].temps_adsorption	<< divider << "min"		<< std::endl;
		text <<L"Pression finale"									<< divider	<< doses[i].pression_finale		<< divider << "bar"		<< std::endl;
		text																															<< std::endl;	
	}

	return text.str();
}



/**********************************************************************
* Writes the desorption steps
* Inputs:
*       bool csv: Ask for a comma separated value format if true
*		const std::vector<Donnees_Desorption> &desorption:	Reference to the desorpiton data to be written
***********************************************************************/
std::wstring FileWriter::EnteteDesorption(bool csv, const std::vector<Donnees_Desorption> &desorption)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	for (size_t i = 0; i < desorption.size(); i++)
	{	
		text <<L"-----------------------------------------------------"																		<< std::endl;
		text <<L"Desorption stage number"							<< divider	<< i + 1													<< std::endl;
		text <<L"Delta pression"									<< divider	<< desorption[i].delta_pression		<< divider << "bar"		<< std::endl;
		text <<L"Temps de l'expérience dans le volume référenciel"	<< divider	<< desorption[i].temps_volume		<< divider << "min"		<< std::endl;
		text <<L"Temps de l'expérience dans l'adsorption "			<< divider	<< desorption[i].temps_desorption	<< divider << "min"		<< std::endl;
		text <<L"Pression finale"									<< divider	<< desorption[i].pression_finale	<< divider << "bar"		<< std::endl;
		text																																<< std::endl;	
	}

	return text.str();
}


/**********************************************************************
* Writes any settings changes into the entete files
* Inputs:
*        bool csv:								Ask to write to the comma separated value file if true
*        const ExperimentSettings& newSettings:	ExperimentSettings newSettings: The settings file to compare to the current one
*        const ExperimentSettings& oldSettings:	ExperimentSettings newSettings: The old settings file to compare to
*        const ExperimentData& newSettings:		ExperimentSettings newSettings: The current experiment state
***********************************************************************/
void FileWriter::RecordDataChange(bool csv, const ExperimentSettings& newSettings, const ExperimentSettings& oldSettings, const ExperimentData& data)
{
	// Check if csv file is requested
	std::string divider;
	if (csv)
		divider = ";";
	else
		divider = " : ";

	// Create std::string stream
	std::ostringstream text("", std::ios_base::app);

	// Check for changes in adsorption
	for (size_t i = 0; i < oldSettings.dataAdsorption.size(); i++)
	{
		if (oldSettings.dataAdsorption[i] != newSettings.dataAdsorption[i])
		{

			text << std::endl << "-----------------------------------------------------"	<< std::endl;
			text << "Settings changed"														<< std::endl;
			text << "Step" << divider << data.adsorptionCounter								<< std::endl;
			text << "Dose" << divider << data.experimentDose								<< std::endl;

			if (oldSettings.dataAdsorption[i].delta_pression != newSettings.dataAdsorption[i].delta_pression)
			{
				text << "Delta pression par désorption"						<< divider << newSettings.dataAdsorption[i].delta_pression		<< divider << "bar" << std::endl;
			}

			if (oldSettings.dataAdsorption[i].temps_volume != newSettings.dataAdsorption[i].temps_volume)
			{
				text << "Temps de l'expérience dans le volume référenciel"	<< divider << newSettings.dataAdsorption[i].temps_volume		<< divider << "min" << std::endl;
			}

			if (oldSettings.dataAdsorption[i].temps_adsorption != newSettings.dataAdsorption[i].temps_adsorption)
			{
				text << "Temps de l'expérience dans la désorption"			<< divider << newSettings.dataAdsorption[i].temps_adsorption	<< divider << "min" << std::endl;
			}

			if (oldSettings.dataAdsorption[i].pression_finale != newSettings.dataAdsorption[i].pression_finale)
			{
				text << "Pression finale"									<< divider << newSettings.dataAdsorption[i].pression_finale		<< divider << "bar" << std::endl;
			}
		}
	}

	// Check for changes in desorption
	for (size_t i = 0; i < oldSettings.dataDesorption.size(); i++)
	{
		if (oldSettings.dataDesorption[i] != newSettings.dataDesorption[i])
		{
			text << std::endl << "-----------------------------------------------------"	<< std::endl;
			text << "Settings changed"														<< std::endl;
			text << "Step" << divider << data.adsorptionCounter								<< std::endl;
			text << "Dose" << divider << data.experimentDose								<< std::endl;

			if (oldSettings.dataDesorption[i] != newSettings.dataDesorption[i])
			{
				if (oldSettings.dataDesorption[i].delta_pression != newSettings.dataDesorption[i].delta_pression)
				{
					text << "Delta pression par désorption"						<< divider << newSettings.dataDesorption[i].delta_pression		<< divider << "bar" << std::endl;
				}

				if (oldSettings.dataDesorption[i].temps_volume != newSettings.dataAdsorption[i].temps_volume)
				{
					text << "Temps de l'expérience dans le volume référenciel"	<< divider << newSettings.dataDesorption[i].temps_volume		<< divider << "min" << std::endl;
				}

				if (oldSettings.dataDesorption[i].temps_desorption != newSettings.dataDesorption[i].temps_desorption)
				{
					text << "Temps de l'expérience dans la désorption"			<< divider << newSettings.dataDesorption[i].temps_desorption	<< divider << "min" << std::endl;
				}

				if (oldSettings.dataDesorption[i].pression_finale != newSettings.dataAdsorption[i].pression_finale)
				{
					text << "Pression finale"									<< divider << newSettings.dataDesorption[i].pression_finale		<< divider << "bar" << std::endl;
				}
			}
		}
	}

	// Get title
	std::wstring title;
	if (csv)
		title = BuildFileName(L"csv", newSettings.dataGeneral, true).c_str();
	else
		title = BuildFileName(L"txt", newSettings.dataGeneral, true).c_str();

	// Write to file
	std::ofstream file;
	file.open(title, std::ios::out | std::ios::app);
	file << text.str() << std::endl;
	file.close();

}



/**********************************************************************
* Returns the full path and title of the file to be written
* Inputs:
*       std::wstring extension: Extension you want the file to have
*		const Donnees_General &general:		Reference to the general data to be checked
*       bool entete: specify true to get the entete std::string or false for the regular file
***********************************************************************/
std::wstring FileWriter::BuildFileName(std::wstring extension, const Donnees_General &general, bool entete)
{
	// Create buffer
	wchar_t fileNameBuffer[255];

	// Put path in buffer
	wprintf_s(fileNameBuffer, "%s", general.chemin.c_str());

	// Check for validity, if not, put the file in the C: drive
	if (!PathIsDirectory(fileNameBuffer))
	{
		messageHandler->DisplayMessageBox(ERROR_PATHUNDEF, MB_ICONERROR | MB_OK, false);
		wprintf_s(fileNameBuffer, "C:/");
	}

	// Check if the user field is empty
	if (general.experimentateur.surnom.empty())
	{
		wprintf_s(fileNameBuffer, "%s/Nouveau_Fichier", general.chemin.c_str());
	}
	else
	{
		// Check if the user directory exists, if not, create it
		wprintf_s(fileNameBuffer, "%s/%s", general.chemin.c_str(), general.experimentateur.surnom.c_str());
		if (!PathIsDirectory(fileNameBuffer)) {
			CreateDirectory(fileNameBuffer, NULL);
		}
		// Check if the sample directory exists
		wprintf_s(fileNameBuffer, "%s/%s/%s", general.chemin.c_str(), general.experimentateur.surnom.c_str(),
			general.nom_echantillon.c_str());
	}

	// Check if the full path exists, if not create it
	if (!PathIsDirectory(fileNameBuffer)) {
		CreateDirectory(fileNameBuffer, NULL);
	}

	// Finally store the entire path including the file name and return it
	if (entete)
	{
		wprintf_s(fileNameBuffer, "%s/%s(entete).%s", fileNameBuffer, general.fichier.c_str(), extension.c_str());
	}
	else
	{
		wprintf_s(fileNameBuffer, "%s/%s.%s", fileNameBuffer, general.fichier.c_str(), extension.c_str());
	}

	// Now return generated std::string
	return fileNameBuffer;
}



//std::string CManip_AutoGaz::EnteteAdsorptionContinue()
//{
//	std::ostringstream chaine_char("", std::ios_base::app);
//
//	chaine_char << std::endl << "-----------------------------------------------------" << std::endl;
//	chaine_char << "Application de l'adsorption continue" << std::endl;
//	chaine_char << "Temps d'étalonnage débit : " << adsorption_continue.temps_etalonnage_debit << " min" << std::endl;
//	chaine_char << "Temps d'étalonnage du volume intermédiaire : " << adsorption_continue.temps_etalonnage_volume_inter << " min" << std::endl;
//	chaine_char << "Temps d'équilibre continue : " << adsorption_continue.temps_equilibre_continue << " min" << std::endl;
//	chaine_char << "Temps final d'équilibre : " << adsorption_continue.temps_equilibre_continue << " min" << std::endl;
//	chaine_char << "Pression finale : " << adsorption_continue.pression_finale_adsorption_continue << " bar" << std::endl;
//
//	return chaine_char.str();
//}
//
//std::string CManip_AutoGaz::EnteteAdsorptionContinueCSV()
//{
//	std::ostringstream chaine_char("", std::ios_base::app);
//
//	chaine_char << std::endl << "-----------------------------------------------------" << std::endl;
//	chaine_char << "Application de l'adsorption continue" << std::endl;
//	chaine_char << "Temps d'étalonnage débit;" << adsorption_continue.temps_etalonnage_debit << ";min" << std::endl;
//	chaine_char << "Temps d'étalonnage du volume intermédiaire;" << adsorption_continue.temps_etalonnage_volume_inter << ";min" << std::endl;
//	chaine_char << "Temps d'équilibre continue;" << adsorption_continue.temps_equilibre_continue << ";min" << std::endl;
//	chaine_char << "Temps final d'équilibre;" << adsorption_continue.temps_equilibre_continue << ";min" << std::endl;
//	chaine_char << "Pression finale;" << adsorption_continue.pression_finale_adsorption_continue << ";bar" << std::endl;
//
//	return chaine_char.str();
//}