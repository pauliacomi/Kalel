#pragma once


#include <fstream>
#include <sstream>

class FileWriter
{
public:
	FileWriter();
	~FileWriter();

	std::ofstream fileStream;							// The file stream is stored in this variable

	/**********************************************************************************************************************************
	*
	// File Writing Functions
	*
	***********************************************************************************************************************************/

public:

	/**********************************************************************
	* Write the first section of an entete and save it
	* Inputs: 
	*		const ExperimentSettings &expSettings:	Reference to the experimentSettings which generates the entete
	*		const MachineSettings &machSettings:	Reference to the MachineSettings which generates the entete
	* Outputs:
	*		bool: Path undefined error
	***********************************************************************/
	bool EnteteCreate(const ExperimentSettings &expSettings, const MachineSettings &machSettings);


	/**********************************************************************
	* Write the first section of an CSV entete and save it
	* Inputs: 
	*		const ExperimentSettings &expSettings:	Reference to the experimentSettings which generates the entete
	*		const MachineSettings &machSettings:	Reference to the MachineSettings which generates the entete
	* Outputs:
	*		bool: Path undefined error
	***********************************************************************/
	bool EnteteCSVCreate(const ExperimentSettings &expSettings, const MachineSettings &machSettings);


	/**********************************************************************
	* Opens the measurement file for the first time and stores its link in the fileStream ofstream
	* Also writes columns in the CSV
	* Inputs:
	*		const Donnees_General &general:		Reference to the general data to be checked
	* Outputs:
	*		bool: Path undefined error
	***********************************************************************/
	bool FileMeasurementOpen(const Donnees_General &general);


	/**********************************************************************
	* Closes the measurement file
	* Inputs: none
	***********************************************************************/
	void FileMeasurementClose();


	/**********************************************************************
	* Records a measurement
	* Inputs:
	*		Reference to the experimentSettings which generates the entete
	*		bool valveOpen6: records if valve number 6 is open or not
	***********************************************************************/
	void FileMeasurementRecord(const ExperimentData &data, bool valveOpen6);


	/**********************************************************************
	* Writes the base of the entete
	* Inputs:
	*        bool csv: Ask for a comma separated value format if true
	*		 int experimentType: Type of experiment
	***********************************************************************/
	std::wstring EnteteBase(bool csv, int experimentType);


	/**********************************************************************
	* Writes the settings from the general tab
	* Inputs:
	*       bool csv: Ask for a comma separated value format if true
	*		const Donnees_General &general:		Reference to the general data to be written
	*		std::wstring caloName: name of the calorimeter
	***********************************************************************/
	std::wstring EnteteGeneral(bool csv, const Donnees_General &general, std::wstring caloName);


	/**********************************************************************
	* Writes the settings from the diverse tab
	* Inputs:
	*       bool csv: Ask for a comma separated value format if true
	*		const Donnees_Divers &divers:	Reference to the diverse data to be written
	***********************************************************************/
	std::wstring EnteteDivers(bool csv, const Donnees_Divers &divers);


	/**********************************************************************
	* Writes the adsorption steps
	* Inputs:
	*       bool csv: Ask for a comma separated value format if true
	*		const std::vector<Donnees_Doses> &doses:	Reference to the adsorption data to be written
	***********************************************************************/
	std::wstring EnteteAdsorption(bool csv, const std::vector<Donnees_Doses> &doses);


	/**********************************************************************
	* Writes the desorption steps
	* Inputs:
	*       bool csv: Ask for a comma separated value format if true
	*		const std::vector<Donnees_Desorption> &desorption:	Reference to the desorpiton data to be written
	***********************************************************************/
	std::wstring EnteteDesorption(bool csv, const std::vector<Donnees_Desorption> &desorption);


	/**********************************************************************
	* Writes any settings changes into the entete files
	* Inputs:
	*        bool csv:								Ask to write to the comma separated value file if true
	*        const ExperimentSettings& newSettings:	ExperimentSettings newSettings: The settings file to compare to the current one
	*        const ExperimentSettings& oldSettings:	ExperimentSettings newSettings: The old settings file to compare to
	*        const ExperimentData& newSettings:		ExperimentSettings newSettings: The current experiment state
	***********************************************************************/
	void RecordDataChange(bool csv, const ExperimentSettings& newSettings, const ExperimentSettings& oldSettings, const ExperimentData& data);


	/**********************************************************************
	* Returns the full path and title of the file to be written
	* Inputs:
	*       string extension: Extension you want the file to have
	*		const Donnees_General &general:		Reference to the general data to be checked
	*       bool entete: specify true to get the entete string or false for the regular file
	*       bool entete: error return value for undefined path
	***********************************************************************/
	std::wstring BuildFileName(std::wstring extension, const Donnees_General &general, bool entete, bool error);
};

