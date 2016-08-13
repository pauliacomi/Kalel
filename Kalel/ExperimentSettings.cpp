#include "StdAfx.h"
#include "ExperimentSettings.h"

#include "DefineStages.h"
#include "CommonFunctions.h"
#include "Parametres.h"
#include "StringTable.h"

ExperimentSettings::ExperimentSettings()
{
	ExperimentSettings(0, 0);
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
	GUIhandle = NULL;
	experimentType = EXPERIMENT_TYPE_UNDEF;
	
	// Initialisation of settings
	CommonFunctions cF;
	CString temp;

	// General
	dataGeneral.chemin = _T(GetCheminFichierGeneral().c_str());
	dataGeneral.commentaires = "";
	dataGeneral.date_experience = cF.getDate().GetBuffer();
	dataGeneral.experimentateur.nom = "";
	dataGeneral.experimentateur.surnom = "";

	temp.Format(TEXT_NEWFILETEXT, cF.getDateUnderline());
	dataGeneral.fichier = temp.GetBuffer();
	dataGeneral.gaz.nom = "";
	dataGeneral.gaz.symbole = "";
	dataGeneral.gaz.masse_moleculaire = 0;
	dataGeneral.gaz.pression_critique = 0;
	dataGeneral.gaz.temperature_critique = 0;
	dataGeneral.gaz.temperature_ebullition = 0;
	dataGeneral.masse_echantillon = 1.0f;

	temp.Format(TEXT_SAMPLE);
	dataGeneral.nom_echantillon = temp;
	dataGeneral.temperature_experience = 30;

	// Divers
	dataDivers.cellule.numero = "";
	dataDivers.cellule.volume_calo = 0;
	dataDivers.cellule.volume_total = 0;
	dataDivers.mise_sous_vide_fin_experience = false;
	dataDivers.temps_ligne_base = 15;
	dataDivers.temps_vide = 90;

	// Adsorption
	for (size_t i = 0; i < initialAdsorptions; i++)
	{
		Donnees_Doses adsorptionStep;

		adsorptionStep.delta_pression = 1.0f;
		adsorptionStep.pression_finale = 5.0f;
		adsorptionStep.temps_adsorption = 90;
		adsorptionStep.temps_volume = 15;

		dataAdsorption.push_back(adsorptionStep);
	}

	// Desorption
	for (size_t i = 0; i < initialDesorptions; i++)
	{
		Donnees_Desorption desorptionStep;

		desorptionStep.delta_pression = 1.0f;
		desorptionStep.pression_finale = 5.0f;
		desorptionStep.temps_desorption = 90;
		desorptionStep.temps_volume = 15;

		dataDesorption.push_back(desorptionStep);
	}
}

ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings * p) {
	if (this != p) {  // make sure not same object

		GUIhandle = p->GUIhandle;
		experimentType = p->experimentType;

		dataGeneral = p->dataGeneral;
		dataDivers = p->dataDivers;
		dataAdsorption = p->dataAdsorption;
		dataDesorption = p->dataDesorption;

	}
	return *this;    // Return ref for multiple assignment
}
