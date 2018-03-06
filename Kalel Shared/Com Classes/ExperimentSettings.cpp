#include "../Forcelib.h"
#include "ExperimentSettings.h"

#include "../Resources/DefineStages.h"
#include "../Resources/DefineText.h"

ExperimentSettings::ExperimentSettings() 
	: ExperimentSettings(0, 0)
{	
}

ExperimentSettings::ExperimentSettings(int initialAdsorptions, int initialDesorptions)
{
	// Data initialisation
	ResetData(initialAdsorptions, initialDesorptions);
}


ExperimentSettings::~ExperimentSettings(void)
{
}

void ExperimentSettings::ResetData()
{
	ResetData(0,0);
}

void ExperimentSettings::ResetData(int initialAdsorptions, int initialDesorptions)
{
	//GUIhandle = NULL;
	experimentType = EXPERIMENT_TYPE_UNDEF;

	// General
	dataGeneral.chemin = L"";
	dataGeneral.commentaires = L"";
	dataGeneral.date_experience = L"";
	dataGeneral.user.nom = L"";
	dataGeneral.user.surnom = L"";
	dataGeneral.fichier =									L"";		//std::to_wstring(TEXT_NEWFILETEXT) + cF.getDateUnderline();
	dataGeneral.gas.symbole = L"";
	dataGeneral.gas.masse_moleculaire = 0;
	dataGeneral.gas.pression_critique = 0;
	dataGeneral.gas.temperature_critique = 0;
	dataGeneral.gas.temperature_ebullition = 0;
	dataGeneral.masse_echantillon = 1.0f;
	dataGeneral.nom_echantillon =							L"";		//TEXT_SAMPLE;
	dataGeneral.temperature_experience = 30;

	// Divers
	dataDivers.cell.numero = L"";
	dataDivers.cell.volume_calo = 0;
	dataDivers.cell.volume_total = 0;
	dataDivers.mise_sous_vide_fin_experience = false;
	dataDivers.temps_ligne_base = 15;
	dataDivers.temps_vide = 90;

	// Adsorption
	for (int i = 0; i < initialAdsorptions; i++)
	{
		data_adsorption adsorptionStep;

		adsorptionStep.delta_pression = 1.0f;
		adsorptionStep.pression_finale = 5.0f;
		adsorptionStep.temps_adsorption = 90;
		adsorptionStep.temps_volume = 15;

		dataAdsorption.push_back(adsorptionStep);
	}

	// Desorption
	for (int i = 0; i < initialDesorptions; i++)
	{
		data_desorption desorptionStep;

		desorptionStep.delta_pression = 1.0f;
		desorptionStep.pression_finale = 5.0f;
		desorptionStep.temps_desorption = 90;
		desorptionStep.temps_volume = 15;

		dataDesorption.push_back(desorptionStep);
	}
}

ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p) {
	if (this != p) {  // make sure not same object

		experimentType = p->experimentType;

		dataGeneral = p->dataGeneral;
		dataDivers = p->dataDivers;
		dataAdsorption = p->dataAdsorption;
		dataDesorption = p->dataDesorption;

	}
	return *this;    // Return ref for multiple assignment
}
