#include "FileWriter.h"

#include <filesystem>

namespace fs = std::experimental::filesystem::v1;

// Resources
#include "../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../Kalel Shared/Resources/DefineStages.h"						// All stage, experiment type definitions are here


FileWriter::FileWriter()
{
}


FileWriter::~FileWriter()
{
}



/**********************************************************************
* Private function that allows for thread safe file writing
* Inputs:
*		const wstring &filename:	Filename to write to
*		const wstring &stream:		What to write
***********************************************************************/
void FileWriter::writeFile(const std::wstring &filename, const std::wstring & stream)
{	
	std::unique_lock<std::mutex> lock(fileLock);			// get mutex
	_wfopen_s(&f, filename.c_str(), L"w");
	if (!f)
		return;
	fprintf(f, "%ws", stream.c_str());
	fflush(f);
}


/**********************************************************************
* Write the first section of an entete and save it
* Inputs:
*		const ExperimentSettings &expSettings:	Reference to the experimentSettings which generates the entete
*		const MachineSettings &machSettings:	Reference to the MachineSettings which generates the entete
***********************************************************************/
bool FileWriter::EnteteCreate(const ExperimentSettings &expSettings, const MachineSettings &machSettings)
{
	std::wostringstream stream;

	stream << EnteteBase(false, expSettings.experimentType);
	stream << EnteteGeneral(false, expSettings.dataGeneral, machSettings.CaloName);

	if (expSettings.experimentType == EXPERIMENT_TYPE_AUTO) {
		stream << EnteteDivers(false, expSettings.dataDivers);
		stream << EnteteAdsorption(false, expSettings.dataAdsorption);
		stream << EnteteDesorption(false, expSettings.dataDesorption);
	}

	bool ret = false;
	writeFile(FileWriter::BuildFileName(L"txt", expSettings.dataGeneral, true, ret), stream.str());
	return ret;
}



/**********************************************************************
* Write the first section of an CSV entete and save it
* Inputs:
*		const ExperimentSettings &expSettings:	Reference to the experimentSettings which generates the entete
*		const MachineSettings &machSettings:	Reference to the MachineSettings which generates the entete
***********************************************************************/
bool FileWriter::EnteteCSVCreate(const ExperimentSettings &expSettings, const MachineSettings &machSettings)
{
	std::wostringstream stream;

	stream << EnteteBase(true, expSettings.experimentType);
	stream << EnteteGeneral(true, expSettings.dataGeneral, machSettings.CaloName);

	if (expSettings.experimentType == EXPERIMENT_TYPE_AUTO) {
		stream << EnteteDivers(true, expSettings.dataDivers);
		stream << EnteteAdsorption(true, expSettings.dataAdsorption);
		stream << EnteteDesorption(true, expSettings.dataDesorption);
	}

	bool ret = false;
	writeFile(FileWriter::BuildFileName(L"csv", expSettings.dataGeneral, true, ret), stream.str());
	return ret;
}



/**********************************************************************
* Opens the FileWriter file for the first time and stores its link in the fileStream std::ofstream
* Also writes columns in the CSV
* Inputs: 
*		const Donnees_General &general:		Reference to the general data to be checked
***********************************************************************/
bool FileWriter::FileMeasurementCreate(const Donnees_General &general)
{
	std::wostringstream stream;

	// Write column names
	stream << "N°mesure;";												// Experiment dose
	stream << "Temps(s);";												// Experiment time
	stream << "Calorimètre(W);";										// Calorimeter value
	stream << "Basse Pression(Bar);";									// Pressure low range
	stream << "Haute Pression(Bar);";									// Pressure high range
	stream << "T°C Calo;";												// Temperature calorimeter
	stream << "T°C Cage;";												// Temperature enclosure
	stream << "T°C pièce;";												// Temperature room
	stream << "Vanne 6";												// Valve open
	stream << std::endl;												// Next line

	bool ret = false;
	writeFile(FileWriter::BuildFileName(L"csv", general, false, ret), stream.str());
	return ret;
}



/**********************************************************************
* Records a measurement
* Inputs:
*		Reference to the experimentSettings which generates the entete
*		bool valveOpen6: records if valve number 6 is open or not
***********************************************************************/
void FileWriter::FileMeasurementRecord(const Donnees_General &general, const ExperimentData &data, const ExperimentStatus &status, bool valveOpen6)
{
	std::wostringstream stream;
	char char_resultat_calo[20];
	sprintf_s(char_resultat_calo, "%.8E", data.GetresultCalorimeter());

	if (stream)
	{
		stream << status.experimentDose					<< ";";				// Experiment dose
		stream << status.timeElapsed					<< ";";				// Experiment time
		stream << char_resultat_calo					<< ";";				// Calorimeter value
		stream << data.pressureLow						<< ";";				// Pressure low range
		stream << data.pressureHigh						<< ";";				// Pressure high range
		stream << data.temperatureCalo					<< ";";				// Temperature calorimeter
		stream << data.temperatureCage					<< ";";				// Temperature enclosure
		stream << data.temperatureRoom					<< ";";				// Temperature room
		stream << valveOpen6							<< ";";				// Valve open
		stream << std::endl;												// Next line
	}

	bool ret = false;
	writeFile(FileWriter::BuildFileName(L"csv", general, false, ret), stream.str());
	return;
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

	std::wostringstream text;

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

	std::wostringstream text;

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

	std::wostringstream text;

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

	std::wostringstream text;

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
void FileWriter::RecordDataChange(bool csv, const ExperimentSettings& newSettings, const ExperimentSettings& oldSettings, const ExperimentStatus &status, const ExperimentData& data)
{
	// Check if csv file is requested
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	// Create std::string stream
	std::wostringstream text;

	// Check for changes in adsorption
	for (size_t i = 0; i < oldSettings.dataAdsorption.size(); i++)
	{
		if (oldSettings.dataAdsorption[i] != newSettings.dataAdsorption[i])
		{

			text << std::endl << "-----------------------------------------------------"	<< std::endl;
			text << "Settings changed"														<< std::endl;
			text << "Step" << divider << status.adsorptionCounter							<< std::endl;
			text << "Dose" << divider << status.experimentDose								<< std::endl;

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
			text << "Step" << divider << status.adsorptionCounter							<< std::endl;
			text << "Dose" << divider << status.experimentDose								<< std::endl;

			
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

	// Get title
	std::wstring title;
	bool ret = false;
	if (csv)
		title = BuildFileName(L"csv", newSettings.dataGeneral, true, ret).c_str();
	else
		title = BuildFileName(L"txt", newSettings.dataGeneral, true, ret).c_str();

	// Write to file
	writeFile(title, text.str());
}



/**********************************************************************
* Returns the full path and title of the file to be written
* Inputs:
*       std::wstring extension: Extension you want the file to have
*		const Donnees_General &general:		Reference to the general data to be checked
*       bool entete: specify true to get the entete std::string or false for the regular file
***********************************************************************/
std::wstring FileWriter::BuildFileName(std::wstring extension, const Donnees_General &general, bool entete, bool error)
{
	// Create buffer
	wchar_t fileNameBuffer[255];

	// Put path in buffer
	wprintf_s(fileNameBuffer, "%s", general.chemin.c_str());

	// Check for validity, if not, put the file in the C: drive and return an error
	error = false;
	if (!fs::is_directory(fileNameBuffer))
	{
		error = true;
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
		if (!fs::create_directory(fileNameBuffer)) {
			fs::create_directory(fileNameBuffer);
		}
		// Check if the sample directory exists
		wprintf_s(fileNameBuffer, "%s/%s/%s", general.chemin.c_str(), general.experimentateur.surnom.c_str(),
			general.nom_echantillon.c_str());
	}

	// Check if the full path exists, if not create it
	if (!fs::create_directory(fileNameBuffer)) {
		fs::create_directory(fileNameBuffer);
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
//	std::wostringstream chaine_char("", std::ios_base::app);
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
//	std::wostringstream chaine_char("", std::ios_base::app);
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