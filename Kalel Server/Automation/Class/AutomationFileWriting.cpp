#include "../../stdafx.h"
#include "Automation.h"



/**********************************************************************
* Write the first section of an entete and save it
* Inputs: none
***********************************************************************/
void Automation::EnteteCreate()
{
	std::wofstream file;

	file.open(Automation::BuildFileName(L"txt", true).c_str(), std::ios_base::out);
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
	std::wofstream file;

	file.open(Automation::BuildFileName(L"csv", true).c_str(), std::ios_base::out);
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
* Opens the measurement file for the first time and stores its link in the fileStream std::ofstream
* Also writes columns in the CSV
* Inputs: none
***********************************************************************/
void Automation::FileMeasurementOpen()
{
	// Create the file
	fileStream.open(BuildFileName(L"csv", false).c_str(), std::ios_base::out /*| ios::trunc*/);

	// Clear the file stream, pas le .csv, et on peut r�it�rer l'�criture en enlevant le caract�re "fin de fichier"
	fileStream.clear();

	// Write column names
	fileStream << "N�mesure;";													  // Experiment dose
	fileStream << "Temps(s);";													  // Experiment time
	fileStream << "Calorim�tre(W);";											  // Calorimeter value
	fileStream << "Basse Pression(Bar);";										  // Pressure low range
	fileStream << "Haute Pression(Bar);";										  // Pressure high range
	fileStream << "T�C Calo;";													  // Temperature calorimeter
	fileStream << "T�C Cage;";													  // Temperature enclosure
	fileStream << "T�C pi�ce;";													  // Temperature room
	fileStream << "Vanne 6";													  // Valve open
	fileStream << std::endl;															  // Next line
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
		fileStream << experimentLocalData.timeElapsed << ";";				   // Experiment time
		fileStream << char_resultat_calo << ";";								   // Calorimeter value
		fileStream << experimentLocalData.pressureLow << ";";					   // Pressure low range
		fileStream << experimentLocalData.pressureHigh << ";";					   // Pressure high range
		fileStream << experimentLocalData.temperatureCalo << ";";				   // Temperature calorimeter
		fileStream << experimentLocalData.temperatureCage << ";";				   // Temperature enclosure
		fileStream << experimentLocalData.temperatureRoom << ";";				   // Temperature room
		fileStream << EstOuvert_Vanne(6) << ";";								   // Valve open
		fileStream << std::endl;														   // Next line
	}
}



/**********************************************************************
* Writes the base of the entete
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
std::wstring Automation::EnteteBase(bool csv)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

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
std::wstring Automation::EnteteGeneral(bool csv)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	text << L"Experimentateur"				<< divider		<< experimentLocalSettings.dataGeneral.experimentateur.nom								<< finl;
	text << L"Date"							<< divider		<< experimentLocalSettings.dataGeneral.date_experience									<< finl;
	text << L"Gaz"							<< divider		<< experimentLocalSettings.dataGeneral.gaz.symbole										<< finl;
	text << L"Echantillon"					<< divider		<< experimentLocalSettings.dataGeneral.nom_echantillon									<< finl;
	text << L"Masse"						<< divider		<< experimentLocalSettings.dataGeneral.masse_echantillon		<< divider << L"g"		<< finl;
	text << L"Temp�rature de l'exp�rience"	<< divider		<< experimentLocalSettings.dataGeneral.temperature_experience	<< divider << L"�C"		<< finl;
	text << L"Commentaires"					<< divider		<< experimentLocalSettings.dataGeneral.commentaires										<< finl;
	text << L"Calorim�tre"					<< divider		<< GetNomCalo()																			<< finl;
	text																																			<< finl;

	return text.str();
}



/**********************************************************************
* Writes the settings from the diverse tab
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
std::wstring Automation::EnteteDivers(bool csv)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	text << L"Num�ro de Cellule"			<< divider		<< experimentLocalSettings.dataDivers.cellule.numero									<< finl;
	text << L"Volume du calo"				<< divider		<< experimentLocalSettings.dataDivers.cellule.volume_calo		<< divider << "cm3"		<< finl;
	text << L"Volume total"					<< divider		<< experimentLocalSettings.dataDivers.cellule.volume_total		<< divider << "cm3"		<< finl;
	text << L"Baseline time"				<< divider		<< experimentLocalSettings.dataDivers.temps_ligne_base			<< divider << "min"		<< finl;
	text << L"Experiment end vacuum"		<< divider		<< experimentLocalSettings.dataDivers.mise_sous_vide_fin_experience						<< finl;
	text << L"Vacuum time"					<< divider		<< experimentLocalSettings.dataDivers.temps_vide				<< divider << "min"		<< finl;
	text																																			<< finl;							

	return text.str();
}



/**********************************************************************
* Writes the adsorption steps
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
std::wstring Automation::EnteteAdsorption(bool csv)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	for (size_t i = 0; i < experimentLocalSettings.dataAdsorption.size(); i++)
	{	
		text <<L"-----------------------------------------------------"																								<< finl;
		text <<L"Adsorption stage number"							<< divider	<< i + 1	 																		<< finl;
		text <<L"Delta pression"									<< divider	<< experimentLocalSettings.dataAdsorption[i].delta_pression		<< divider << "bar" << finl;
		text <<L"Temps de l'exp�rience dans le volume r�f�renciel"	<< divider	<< experimentLocalSettings.dataAdsorption[i].temps_volume		<< divider << "min" << finl;
		text <<L"Temps de l'exp�rience dans l'adsorption "			<< divider	<< experimentLocalSettings.dataAdsorption[i].temps_adsorption	<< divider << "min" << finl;
		text <<L"Pression finale"									<< divider	<< experimentLocalSettings.dataAdsorption[i].pression_finale	<< divider << "bar" << finl;
		text																																						<< finl;	
	}

	return text.str();
}



/**********************************************************************
* Writes the desorption steps
* Inputs:
*        bool csv: Ask for a comma separated value format if true
***********************************************************************/
std::wstring Automation::EnteteDesorption(bool csv)
{
	std::wstring divider;
	if (csv)
		divider = L";";
	else
		divider = L" : ";

	std::wostringstream text(L"", std::ios_base::app);

	for (size_t i = 0; i < experimentLocalSettings.dataDesorption.size(); i++)
	{	
		text <<L"-----------------------------------------------------"																								<< finl;
		text <<L"Desorption stage number"							<< divider	<< i + 1																			<< finl;
		text <<L"Delta pression"									<< divider	<< experimentLocalSettings.dataDesorption[i].delta_pression		<< divider << "bar" << finl;
		text <<L"Temps de l'exp�rience dans le volume r�f�renciel"	<< divider	<< experimentLocalSettings.dataDesorption[i].temps_volume		<< divider << "min" << finl;
		text <<L"Temps de l'exp�rience dans l'adsorption "			<< divider	<< experimentLocalSettings.dataDesorption[i].temps_desorption	<< divider << "min" << finl;
		text <<L"Pression finale"									<< divider	<< experimentLocalSettings.dataDesorption[i].pression_finale	<< divider << "bar" << finl;
		text																																						<< finl;	
	}

	return text.str();
}


/**********************************************************************
* Writes any settings changes into the entete files
* Inputs:
*        ExperimentSettings newSettings: The settings file to compare to the current one
*        bool csv: Ask to write to the comma separated value file if true
***********************************************************************/
void Automation::RecordDataChange(const ExperimentSettings& newSettings, bool csv)
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
	for (size_t i = 0; i < experimentLocalSettings.dataAdsorption.size(); i++)
	{
		if (experimentLocalSettings.dataAdsorption[i] != newSettings.dataAdsorption[i] )
		{

			text << std::endl << "-----------------------------------------------------" << finl;
			text << "Settings changed"												<< finl;
			text << "Step" << divider << experimentLocalData.adsorptionCounter		<< finl;
			text << "Dose" << divider << experimentLocalData.experimentDose			<< finl;

			if (experimentLocalSettings.dataAdsorption[i].delta_pression != newSettings.dataAdsorption[i].delta_pression)
			{
				text << "Delta pression par d�sorption"						<< divider << newSettings.dataAdsorption[i].delta_pression		<< divider << "bar" << finl;
			}

			if (experimentLocalSettings.dataAdsorption[i].temps_volume != newSettings.dataAdsorption[i].temps_volume)
			{
				text << "Temps de l'exp�rience dans le volume r�f�renciel"	<< divider << newSettings.dataAdsorption[i].temps_volume		<< divider << "min" << finl;
			}

			if (experimentLocalSettings.dataAdsorption[i].temps_adsorption != newSettings.dataAdsorption[i].temps_adsorption)
			{
				text << "Temps de l'exp�rience dans la d�sorption"			<< divider << newSettings.dataAdsorption[i].temps_adsorption	<< divider << "min" << finl;
			}

			if (experimentLocalSettings.dataAdsorption[i].pression_finale != newSettings.dataAdsorption[i].pression_finale)
			{
				text << "Pression finale"									<< divider << newSettings.dataAdsorption[i].pression_finale		<< divider << "bar" << finl;
			}
		}
	}

	// Check for changes in desorption
	for (size_t i = 0; i < experimentLocalSettings.dataDesorption.size(); i++)
	{
		if (experimentLocalSettings.dataDesorption[i] != newSettings.dataDesorption[i])
		{
			text << std::endl << "-----------------------------------------------------" << finl;
			text << "Settings changed"												<< finl;
			text << "Step" << divider << experimentLocalData.adsorptionCounter		<< finl;
			text << "Dose" << divider << experimentLocalData.experimentDose			<< finl;

			if (experimentLocalSettings.dataDesorption[i] != newSettings.dataDesorption[i])
			{
				if (experimentLocalSettings.dataDesorption[i].delta_pression != newSettings.dataDesorption[i].delta_pression)
				{
					text << "Delta pression par d�sorption"						<< divider << newSettings.dataDesorption[i].delta_pression		<< divider << "bar" << finl;
				}

				if (experimentLocalSettings.dataDesorption[i].temps_volume != newSettings.dataAdsorption[i].temps_volume)
				{
					text << "Temps de l'exp�rience dans le volume r�f�renciel"	<< divider << newSettings.dataDesorption[i].temps_volume		<< divider << "min" << finl;
				}

				if (experimentLocalSettings.dataDesorption[i].temps_desorption != newSettings.dataDesorption[i].temps_desorption)
				{
					text << "Temps de l'exp�rience dans la d�sorption"			<< divider << newSettings.dataDesorption[i].temps_desorption	<< divider << "min" << finl;
				}

				if (experimentLocalSettings.dataDesorption[i].pression_finale != newSettings.dataAdsorption[i].pression_finale)
				{
					text << "Pression finale"									<< divider << newSettings.dataDesorption[i].pression_finale		<< divider << "bar" << finl;
				}
			}
		}
	}

	// Get title
	std::wstring title;
	if (csv)
		title = BuildFileName(L"csv", true).c_str();
	else
		title = BuildFileName(L"txt", true).c_str();

	// Write to file
	std::ofstream file;
	file.open(title, std::ios::out | std::ios::app);
	file << text.str() << std::endl;
	file.close();

}



/**********************************************************************
* Returns the full path and title of the file to be written
* Inputs:
*        std::string extension: Extension you want the file to have
*        bool entete: specify true to get the entete std::string or false for the regular file
***********************************************************************/
std::wstring Automation::BuildFileName(std::wstring extension, bool entete)
{
	// Create buffer
	wchar_t fileNameBuffer[255];

	// Put path in buffer
	wprintf_s(fileNameBuffer, "%s", experimentLocalSettings.dataGeneral.chemin.c_str());

	// Check for validity, if not, put the file in the C: drive
	if (!PathIsDirectory(fileNameBuffer))
	{
		messageHandler.DisplayMessageBox(ERROR_PATHUNDEF, MB_ICONERROR | MB_OK, false);
		wprintf_s(fileNameBuffer, "C:/");
	}

	// Check if the user field is empty
	if (experimentLocalSettings.dataGeneral.experimentateur.surnom.empty())
	{
		wprintf_s(fileNameBuffer, "%s/Nouveau_Fichier", experimentLocalSettings.dataGeneral.chemin.c_str());
	}
	else
	{
		// Check if the user directory exists, if not, create it
		wprintf_s(fileNameBuffer, "%s/%s", experimentLocalSettings.dataGeneral.chemin.c_str(), experimentLocalSettings.dataGeneral.experimentateur.surnom.c_str());
		if (!PathIsDirectory(fileNameBuffer)) {
			CreateDirectory(fileNameBuffer, NULL);
		}
		// Check if the sample directory exists
		wprintf_s(fileNameBuffer, "%s/%s/%s", experimentLocalSettings.dataGeneral.chemin.c_str(), experimentLocalSettings.dataGeneral.experimentateur.surnom.c_str(),
			experimentLocalSettings.dataGeneral.nom_echantillon.c_str());
	}

	// Check if the full path exists, if not create it
	if (!PathIsDirectory(fileNameBuffer)) {
		CreateDirectory(fileNameBuffer, NULL);
	}

	// Finally store the entire path including the file name and return it
	if (entete)
	{
		wprintf_s(fileNameBuffer, "%s/%s(entete).%s", fileNameBuffer, experimentLocalSettings.dataGeneral.fichier.c_str(), extension.c_str());
	}
	else
	{
		wprintf_s(fileNameBuffer, "%s/%s.%s", fileNameBuffer, experimentLocalSettings.dataGeneral.fichier.c_str(), extension.c_str());
	}

	// Now return generated std::string
	return fileNameBuffer;
}



//std::string CManip_AutoGaz::EnteteAdsorptionContinue()
//{
//	std::ostringstream chaine_char("", std::ios_base::app);
//
//	chaine_char << finl << "-----------------------------------------------------" << finl;
//	chaine_char << "Application de l'adsorption continue" << finl;
//	chaine_char << "Temps d'�talonnage d�bit : " << adsorption_continue.temps_etalonnage_debit << " min" << finl;
//	chaine_char << "Temps d'�talonnage du volume interm�diaire : " << adsorption_continue.temps_etalonnage_volume_inter << " min" << finl;
//	chaine_char << "Temps d'�quilibre continue : " << adsorption_continue.temps_equilibre_continue << " min" << finl;
//	chaine_char << "Temps final d'�quilibre : " << adsorption_continue.temps_equilibre_continue << " min" << finl;
//	chaine_char << "Pression finale : " << adsorption_continue.pression_finale_adsorption_continue << " bar" << finl;
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
//	chaine_char << "Temps d'�talonnage d�bit;" << adsorption_continue.temps_etalonnage_debit << ";min" << std::endl;
//	chaine_char << "Temps d'�talonnage du volume interm�diaire;" << adsorption_continue.temps_etalonnage_volume_inter << ";min" << std::endl;
//	chaine_char << "Temps d'�quilibre continue;" << adsorption_continue.temps_equilibre_continue << ";min" << std::endl;
//	chaine_char << "Temps final d'�quilibre;" << adsorption_continue.temps_equilibre_continue << ";min" << std::endl;
//	chaine_char << "Pression finale;" << adsorption_continue.pression_finale_adsorption_continue << ";bar" << std::endl;
//
//	return chaine_char.str();
//}