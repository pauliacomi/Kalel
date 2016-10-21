#pragma once

#define STRINGIFY(var) (#var)

#include "../Netcode/json.hpp"
#include "../Com Classes/ExperimentData.h"
#include "../Com Classes/ExperimentSettings.h"
#include "../Com Classes/MachineSettings.h"
#include "../unicodeConv.h"

namespace serialization {
	
	static void serializeMachineSettingsToJSON(const MachineSettings &m, nlohmann::json &j) {
		
		j["CaloName"]										= UnicodeConv::ws2s(m.CaloName);
		j["CaloEntete"]										= UnicodeConv::ws2s(m.CaloEntete);
		j["CheminFichierGeneral"]							= UnicodeConv::ws2s(m.CheminFichierGeneral);
		j["ActivationSecurite"]								= m.ActivationSecurite;
		j["HighPressureToMeasure"]							= m.HighPressureToMeasure;
		j["LowPressureToMeasure"]							= m.LowPressureToMeasure;
		j["NumberInstruments"]								= m.NumberInstruments;
		j["PortKeithley"]									= m.PortKeithley;
		j["PortMensor"]										= m.PortMensor;
		j["PortTemperatures"]								= m.PortTemperatures;
		j["PortVannes"]										= m.PortVannes;
		j["PresenceTuyereSonique"]							= m.PresenceTuyereSonique;
		j["PressionLimiteVide"]								= m.PressionLimiteVide;
		j["PressionSecuriteBassePression"]					= m.PressionSecuriteBassePression;
		j["PressionSecuriteHautePression"]					= m.PressionSecuriteHautePression;
		j["SensibiliteCalo"]								= m.SensibiliteCalo;
		j["SensibiliteCapteurBassePression"]				= m.SensibiliteCapteurBassePression;
		j["SensibiliteCapteurHautePression"]				= m.SensibiliteCapteurHautePression;
		j["VolumeP6"]										= m.VolumeP6;
		j["VolumeRef"]										= m.VolumeRef;
		j["COMInstruments"]									= m.COMInstruments;
		j["FunctionInstruments"]							= m.FunctionInstruments;
		j["typeInstruments"]								= m.typeInstruments;

	}

	static void deserializeJSONtoMachineSettings(const nlohmann::json &j, MachineSettings &m) {

		m.CaloName											= UnicodeConv::s2ws(j["CaloName"]);
		m.CaloEntete										= UnicodeConv::s2ws(j["CaloEntete"]);
		m.CheminFichierGeneral								= UnicodeConv::s2ws(j["CheminFichierGeneral"]);
		m.ActivationSecurite								= j["ActivationSecurite"];				
		m.HighPressureToMeasure								= j["HighPressureToMeasure"];				
		m.LowPressureToMeasure								= j["LowPressureToMeasure"];				
		m.NumberInstruments									= j["NumberInstruments"];					
		m.PortKeithley										= j["PortKeithley"];						
		m.PortMensor										= j["PortMensor"];							
		m.PortTemperatures									= j["PortTemperatures"];					
		m.PortVannes										= j["PortVannes"];							
		m.PresenceTuyereSonique								= j["PresenceTuyereSonique"];				
		m.PressionLimiteVide								= j["PressionLimiteVide"];					
		m.PressionSecuriteBassePression						= j["PressionSecuriteBassePression"];		
		m.PressionSecuriteHautePression						= j["PressionSecuriteHautePression"];		
		m.SensibiliteCalo									= j["SensibiliteCalo"];					
		m.SensibiliteCapteurBassePression					= j["SensibiliteCapteurBassePression"];	
		m.SensibiliteCapteurHautePression					= j["SensibiliteCapteurHautePression"];	
		m.VolumeP6											= j["VolumeP6"];							
		m.VolumeRef											= j["VolumeRef"];							
		m.COMInstruments									= j["COMInstruments"].get<std::vector<int>>();						
		m.FunctionInstruments								= j["FunctionInstruments"].get<std::vector<int>>();
		m.typeInstruments									= j["typeInstruments"].get<std::vector<int>>();

	}

	static void serializeExperimentDataToJSON(const ExperimentData &e, nlohmann::json &j) {

		j[	STRINGIFY(experimentInProgress			)]		= e.experimentInProgress					;
		j[	STRINGIFY(experimentRecording			)]		= e.experimentRecording						;
		j[	STRINGIFY(experimentWaiting				)]		= e.experimentWaiting						;
		j[	STRINGIFY(experimentCommandsRequested	)]		= e.experimentCommandsRequested				;
		j[	STRINGIFY(experimentStage				)]		= e.experimentStage							;
		j[	STRINGIFY(verificationStep				)]		= e.verificationStep						;
		j[	STRINGIFY(experimentStepStatus			)]		= e.experimentStepStatus					;
		j[	STRINGIFY(experimentSubstepStage		)]		= e.experimentSubstepStage					;
		j[	STRINGIFY(experimentDose				)]		= e.experimentDose							;
		j[	STRINGIFY(experimentGraphPoints			)]		= e.experimentGraphPoints					;
		j[	STRINGIFY(experimentPreviousStage		)]		= e.experimentPreviousStage					;
		j[	STRINGIFY(measurementsMade				)]		= e.measurementsMade						;
		j[	STRINGIFY(timeElapsed					)]		= e.timeElapsed								;
		j[	STRINGIFY(timeStart						)]		= e.timeStart								;
		j[	STRINGIFY(timeToEquilibrate				)]		= e.timeToEquilibrate						;
		j[	STRINGIFY(timeToEquilibrateCurrent		)]		= e.timeToEquilibrateCurrent				;
		j[	STRINGIFY(injectionAttemptCounter		)]		= e.injectionAttemptCounter					;
		j[	STRINGIFY(adsorptionCounter				)]		= e.adsorptionCounter						;
		j[	STRINGIFY(desorptionCounter				)]		= e.desorptionCounter						;
		j[	STRINGIFY(pressureInitial				)]		= e.pressureInitial							;
		j[	STRINGIFY(pressureFinal					)]		= e.pressureFinal							;
		j[	STRINGIFY(pressureHighOld				)]		= e.pressureHighOld							;
		j[	STRINGIFY(resultCalorimeter				)]		= e.resultCalorimeter						;
		j[	STRINGIFY(pressureLow					)]		= e.pressureLow								;
		j[	STRINGIFY(pressureHigh					)]		= e.pressureHigh							;
		j[	STRINGIFY(temperatureCalo				)]		= e.temperatureCalo							;
		j[	STRINGIFY(temperatureCage				)]		= e.temperatureCage							;
		j[	STRINGIFY(temperatureRoom				)]		= e.temperatureRoom							;
	}

	static void deserializeJSONtoExperimentData(const nlohmann::json &j, ExperimentData &e) {

		e.experimentInProgress								= j[	STRINGIFY(experimentInProgress			)];
		e.experimentRecording								= j[	STRINGIFY(experimentRecording			)];
		e.experimentWaiting									= j[	STRINGIFY(experimentWaiting				)];
		e.experimentCommandsRequested						= j[	STRINGIFY(experimentCommandsRequested	)];
		e.experimentStage									= j[	STRINGIFY(experimentStage				)];
		e.verificationStep									= j[	STRINGIFY(verificationStep				)];
		e.experimentStepStatus								= j[	STRINGIFY(experimentStepStatus			)];
		e.experimentSubstepStage							= j[	STRINGIFY(experimentSubstepStage		)];
		e.experimentDose									= j[	STRINGIFY(experimentDose				)];
		e.experimentGraphPoints								= j[	STRINGIFY(experimentGraphPoints			)];
		e.experimentPreviousStage							= j[	STRINGIFY(experimentPreviousStage		)];
		e.measurementsMade									= j[	STRINGIFY(measurementsMade				)];
		e.timeElapsed										= j[	STRINGIFY(timeElapsed					)];
		e.timeStart											= j[	STRINGIFY(timeStart						)];
		e.timeToEquilibrate									= j[	STRINGIFY(timeToEquilibrate				)];
		e.timeToEquilibrateCurrent							= j[	STRINGIFY(timeToEquilibrateCurrent		)];
		e.injectionAttemptCounter							= j[	STRINGIFY(injectionAttemptCounter		)];
		e.adsorptionCounter									= j[	STRINGIFY(adsorptionCounter				)];
		e.desorptionCounter									= j[	STRINGIFY(desorptionCounter				)];
		e.pressureInitial									= j[	STRINGIFY(pressureInitial				)];
		e.pressureFinal										= j[	STRINGIFY(pressureFinal					)];
		e.pressureHighOld									= j[	STRINGIFY(pressureHighOld				)];
		e.resultCalorimeter									= j[	STRINGIFY(resultCalorimeter				)];
		e.pressureLow										= j[	STRINGIFY(pressureLow					)];
		e.pressureHigh										= j[	STRINGIFY(pressureHigh					)];
		e.temperatureCalo									= j[	STRINGIFY(temperatureCalo				)];
		e.temperatureCage									= j[	STRINGIFY(temperatureCage				)];
		e.temperatureRoom									= j[	STRINGIFY(temperatureRoom				)];

	}

	static void serializeExperimentSettingsToJSON(const ExperimentSettings &m, nlohmann::json &j) {
		
		j["experimentType"]						= m.experimentType;

		// general data
		serializeDataGeneralToJSON(m.dataGeneral, j["general"]);

		// divers data
		serializeDataDiversToJSON(m.dataDivers, j["divers"]);

		// adsorption data
		for (size_t i = 0; i < m.dataAdsorption.size(); i++)
		{
			serializeDataAdsToJSON(m.dataAdsorption[i], j["ads"][i]);
		}
		
		// desorption data
		for (size_t i = 0; i < m.dataDesorption.size(); i++)
		{
			serializeDataDesToJSON(m.dataDesorption[i], j["des"][i]);
		}
	}

	static void serializeDataGeneralToJSON(const Donnees_General &m, nlohmann::json &j) {

		j["path"]								= m.chemin;
		j["comments"]							= m.commentaires;
		j["experimentDate"]						= m.date_experience;
		serializeUserToJSON(					  m.experimentateur, j["user"]);
		serializeGasToJSON(						  m.gaz, j["gas"]);
		j["file"]								= m.fichier;
		j["sampleMass"]							= m.masse_echantillon;
		j["sampleName"]							= m.nom_echantillon;
		j["temperature"]						= m.temperature_experience;
	}


	static void serializeDataDiversToJSON(const Donnees_Divers &m, nlohmann::json &j) {

		serializeCellToJSON(				m.cellule, j["cell"]);
		j["endUnderVacuum"]					= m.mise_sous_vide_fin_experience;
		j["baselineTime"]					= m.temps_ligne_base;
		j["vacuumTime"]						= m.temps_vide;
	}


	static void serializeDataAdsToJSON(const Donnees_Doses &m, nlohmann::json &j) {
		j["dP"]								= m.delta_pression;
		j["Pfinal"]							= m.pression_finale;
		j["Tads"]							= m.temps_adsorption;
		j["Tref"]							= m.temps_volume;
	}


	static void serializeDataDesToJSON(const Donnees_Desorption &m, nlohmann::json &j) {
		j["dP"]								= m.delta_pression;
		j["Pfinal"]							= m.pression_finale;
		j["Tdes"]							= m.temps_desorption;
		j["Tref"]							= m.temps_volume;
	}


	static void serializeGasToJSON(const gaz &m, nlohmann::json &j) {
		j["masse_moleculaire"		]								= m.masse_moleculaire			;
		j["nom"						]								= m.nom							;
		j["omega"					]								= m.omega						;
		j["pression_critique"		]								= m.pression_critique			;
		j["symbole"					]								= m.symbole						;
		j["temperature_critique"	]								= m.temperature_critique		;
		j["temperature_ebullition"	]								= m.temperature_ebullition		;
	}


	static void serializeUserToJSON(const experimentateur &m, nlohmann::json &j) {
		
		j["nom"		]								= m.nom				;
		j["surnom"	]								= m.surnom			;
	}


	static void serializeCellToJSON(const cellule &m, nlohmann::json &j) {
		
		j["number"		]							= m.numero				;
		j["caloVolume"	]							= m.volume_calo			;
		j["totalVolume"	]							= m.volume_total		;
	}
	
}