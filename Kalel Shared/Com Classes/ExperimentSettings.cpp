#include "../Forcelib.h"

#include "ExperimentSettings.h"

#include "../timeHelpers.h"
#include "../Resources/DefineStages.h"
#include "../Resources/DefineText.h"

ExperimentSettings::ExperimentSettings() 
{
	ResetData();							// Do initialisation of all variables
}

ExperimentSettings::ExperimentSettings(const ExperimentSettings & rhs)
{
	Replace(rhs);							// Copy values from reference
}

ExperimentSettings & ExperimentSettings::operator=(const ExperimentSettings & p) {
	if (this != &p) {  // make sure not same object

		Replace(p);

		tp = timeh::NowTime();
	}
	return *this;    // Return ref for multiple assignment
}

void ExperimentSettings::ResetData()
{
	tp = timeh::NowTime();

	experimentType = EXPERIMENT_TYPE_UNDEF;

	// General
	dataGeneral.chemin = L"";
	dataGeneral.commentaires = L"";
	dataGeneral.date_experience = L"";
	dataGeneral.user.nom = L"";
	dataGeneral.user.surnom = L"";
	dataGeneral.fichier = L"";
	dataGeneral.gas.symbole = L"";
	dataGeneral.gas.masse_moleculaire = 0;
	dataGeneral.gas.pression_critique = 0;
	dataGeneral.gas.temperature_critique = 0;
	dataGeneral.gas.temperature_ebullition = 0;
	dataGeneral.masse_echantillon = 1.0f;
	dataGeneral.nom_echantillon = L"Sample";
	dataGeneral.temperature_experience = 30;

	// Divers
	dataDivers.cell.numero = L"";
	dataDivers.cell.volume_calo = 0;
	dataDivers.cell.volume_total = 0;
	dataDivers.mise_sous_vide_fin_experience = false;
	dataDivers.time_baseline = 15;
	dataDivers.temps_vide = 90;

}



void ExperimentSettings::Replace(const ExperimentSettings & rhs)
{
	tp = rhs.tp;

	experimentType = rhs.experimentType;

	dataGeneral = rhs.dataGeneral;
	dataDivers = rhs.dataDivers;
	dataAdsorption = rhs.dataAdsorption;
	dataDesorption = rhs.dataDesorption;
}
