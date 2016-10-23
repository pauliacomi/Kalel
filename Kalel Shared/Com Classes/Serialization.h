#pragma once

#define STRINGIFY(Variable) (void(Variable),#Variable)

#include "../Netcode/json.hpp"
#include "../Com Classes/ExperimentData.h"
#include "../Com Classes/ExperimentSettings.h"
#include "../Com Classes/MachineSettings.h"
#include "../unicodeConv.h"

namespace serialization {

	static void serializeMachineSettingsToJSON(const MachineSettings &m, nlohmann::json &j);
	static void deserializeJSONtoMachineSettings(const nlohmann::json &j, MachineSettings &m);
	
	static void serializeExperimentDataToJSON(const ExperimentData &e, nlohmann::json &j); 
	static void deserializeJSONtoExperimentData(const nlohmann::json &j, ExperimentData &e);
	
	static void serializeExperimentSettingsToJSON(const ExperimentSettings &e, nlohmann::json &j);
	static void deserializeJSONtoExperimentSettings(const nlohmann::json &j, ExperimentSettings &e);

	static void serializeDataGeneralToJSON(const Donnees_General &m, nlohmann::json &j);
	static void serializeDataDiversToJSON(const Donnees_Divers &m, nlohmann::json &j);
	static void serializeDataAdsToJSON(const Donnees_Doses &m, nlohmann::json &j);
	static void serializeDataDesToJSON(const Donnees_Desorption &m, nlohmann::json &j);

	static void deserializeJSONToDataGeneral(const nlohmann::json &j, Donnees_General &m);
	static void deserializeJSONToDataDivers(const nlohmann::json &j, Donnees_Divers &m);
	static void deserializeJSONToDataAds(const nlohmann::json &j, Donnees_Doses &m);
	static void deserializeJSONToDataDes(const nlohmann::json &j, Donnees_Desorption &m);

	static void serializeGasToJSON(const gaz &m, nlohmann::json &j);
	static void serializeCellToJSON(const cellule &m, nlohmann::json &j);
	static void serializeUserToJSON(const experimentateur &m, nlohmann::json &j);

	static void deserializeJSONToGas(const nlohmann::json &j, gaz &m);
	static void deserializeJSONToCell(const nlohmann::json &j, cellule &m);
	static void deserializeJSONToUser(const nlohmann::json &j, experimentateur &m);


	//*************************************************************************************************************************
	//						MachineSettings <> JSON
	//*************************************************************************************************************************
	static void serializeMachineSettingsToJSON(const MachineSettings &m, nlohmann::json &j) {
		
		j[STRINGIFY(m.CaloName							)]	= UnicodeConv::ws2s(m.CaloName)				;
		j[STRINGIFY(m.CaloEntete						)]	= UnicodeConv::ws2s(m.CaloEntete)			;
		j[STRINGIFY(m.CheminFichierGeneral				)]	= UnicodeConv::ws2s(m.CheminFichierGeneral)	;
		j[STRINGIFY(m.ActivationSecurite				)]	= m.ActivationSecurite						;
		j[STRINGIFY(m.HighPressureToMeasure				)]	= m.HighPressureToMeasure					;
		j[STRINGIFY(m.LowPressureToMeasure				)]	= m.LowPressureToMeasure					;
		j[STRINGIFY(m.NumberInstruments					)]	= m.NumberInstruments						;
		j[STRINGIFY(m.PortKeithley						)]	= m.PortKeithley							;
		j[STRINGIFY(m.PortMensor						)]	= m.PortMensor								;
		j[STRINGIFY(m.PortTemperatures					)]	= m.PortTemperatures						;
		j[STRINGIFY(m.PortVannes						)]	= m.PortVannes								;
		j[STRINGIFY(m.PresenceTuyereSonique				)]	= m.PresenceTuyereSonique					;
		j[STRINGIFY(m.PressionLimiteVide				)]	= m.PressionLimiteVide						;
		j[STRINGIFY(m.PressionSecuriteBassePression		)]	= m.PressionSecuriteBassePression			;
		j[STRINGIFY(m.PressionSecuriteHautePression		)]	= m.PressionSecuriteHautePression			;
		j[STRINGIFY(m.SensibiliteCalo					)]	= m.SensibiliteCalo							;
		j[STRINGIFY(m.SensibiliteCapteurBassePression	)]	= m.SensibiliteCapteurBassePression			;
		j[STRINGIFY(m.SensibiliteCapteurHautePression	)]	= m.SensibiliteCapteurHautePression			;
		j[STRINGIFY(m.VolumeP6							)]	= m.VolumeP6								;
		j[STRINGIFY(m.VolumeRef							)]	= m.VolumeRef								;
		j[STRINGIFY(m.COMInstruments					)]	= m.COMInstruments							;
		j[STRINGIFY(m.FunctionInstruments				)]	= m.FunctionInstruments						;
		j[STRINGIFY(m.typeInstruments					)]	= m.typeInstruments							;

	}

	static void deserializeJSONtoMachineSettings(const nlohmann::json &j, MachineSettings &m) {

		m.CaloName											= UnicodeConv::s2ws(j[STRINGIFY(m.CaloName									)]);
		m.CaloEntete										= UnicodeConv::s2ws(j[STRINGIFY(m.CaloEntete								)]);
		m.CheminFichierGeneral								= UnicodeConv::s2ws(j[STRINGIFY(m.CheminFichierGeneral						)]);
		m.ActivationSecurite								=					j[STRINGIFY(m.ActivationSecurite						)];				
		m.HighPressureToMeasure								=					j[STRINGIFY(m.HighPressureToMeasure						)];				
		m.LowPressureToMeasure								=					j[STRINGIFY(m.LowPressureToMeasure						)];				
		m.NumberInstruments									=					j[STRINGIFY(m.NumberInstruments							)];					
		m.PortKeithley										=					j[STRINGIFY(m.PortKeithley								)];						
		m.PortMensor										=					j[STRINGIFY(m.PortMensor								)];							
		m.PortTemperatures									=					j[STRINGIFY(m.PortTemperatures							)];					
		m.PortVannes										=					j[STRINGIFY(m.PortVannes								)];							
		m.PresenceTuyereSonique								=					j[STRINGIFY(m.PresenceTuyereSonique						)];				
		m.PressionLimiteVide								=					j[STRINGIFY(m.PressionLimiteVide						)];					
		m.PressionSecuriteBassePression						=					j[STRINGIFY(m.PressionSecuriteBassePression				)];		
		m.PressionSecuriteHautePression						=					j[STRINGIFY(m.PressionSecuriteHautePression				)];		
		m.SensibiliteCalo									=					j[STRINGIFY(m.SensibiliteCalo							)];					
		m.SensibiliteCapteurBassePression					=					j[STRINGIFY(m.SensibiliteCapteurBassePression			)];	
		m.SensibiliteCapteurHautePression					=					j[STRINGIFY(m.SensibiliteCapteurHautePression			)];	
		m.VolumeP6											=					j[STRINGIFY(m.VolumeP6									)];							
		m.VolumeRef											=					j[STRINGIFY(m.VolumeRef									)];							
		m.COMInstruments									=					j[STRINGIFY(m.COMInstruments							)].get<std::vector<int>>();						
		m.FunctionInstruments								=					j[STRINGIFY(m.FunctionInstruments						)].get<std::vector<int>>();
		m.typeInstruments									=					j[STRINGIFY(m.typeInstruments							)].get<std::vector<int>>();

	}

	//*************************************************************************************************************************
	//						ExperimentData <> JSON
	//*************************************************************************************************************************
	static void serializeExperimentDataToJSON(const ExperimentData &e, nlohmann::json &j) {

		j[	STRINGIFY(e.experimentInProgress			)]	= e.experimentInProgress					;
		j[	STRINGIFY(e.experimentRecording				)]	= e.experimentRecording						;
		j[	STRINGIFY(e.experimentWaiting				)]	= e.experimentWaiting						;
		j[	STRINGIFY(e.experimentCommandsRequested		)]	= e.experimentCommandsRequested				;
		j[	STRINGIFY(e.experimentStage					)]	= e.experimentStage							;
		j[	STRINGIFY(e.verificationStep				)]	= e.verificationStep						;
		j[	STRINGIFY(e.experimentStepStatus			)]	= e.experimentStepStatus					;
		j[	STRINGIFY(e.experimentSubstepStage			)]	= e.experimentSubstepStage					;
		j[	STRINGIFY(e.experimentDose					)]	= e.experimentDose							;
		j[	STRINGIFY(e.experimentGraphPoints			)]	= e.experimentGraphPoints					;
		j[	STRINGIFY(e.experimentPreviousStage			)]	= e.experimentPreviousStage					;
		j[	STRINGIFY(e.measurementsMade				)]	= e.measurementsMade						;
		j[	STRINGIFY(e.timeElapsed						)]	= e.timeElapsed								;
		j[	STRINGIFY(e.timeStart						)]	= e.timeStart								;
		j[	STRINGIFY(e.timeToEquilibrate				)]	= e.timeToEquilibrate						;
		j[	STRINGIFY(e.timeToEquilibrateCurrent		)]	= e.timeToEquilibrateCurrent				;
		j[	STRINGIFY(e.injectionAttemptCounter			)]	= e.injectionAttemptCounter					;
		j[	STRINGIFY(e.adsorptionCounter				)]	= e.adsorptionCounter						;
		j[	STRINGIFY(e.desorptionCounter				)]	= e.desorptionCounter						;
		j[	STRINGIFY(e.pressureInitial					)]	= e.pressureInitial							;
		j[	STRINGIFY(e.pressureFinal					)]	= e.pressureFinal							;
		j[	STRINGIFY(e.pressureHighOld					)]	= e.pressureHighOld							;
		j[	STRINGIFY(e.resultCalorimeter				)]	= e.resultCalorimeter						;
		j[	STRINGIFY(e.pressureLow						)]	= e.pressureLow								;
		j[	STRINGIFY(e.pressureHigh					)]	= e.pressureHigh							;
		j[	STRINGIFY(e.temperatureCalo					)]	= e.temperatureCalo							;
		j[	STRINGIFY(e.temperatureCage					)]	= e.temperatureCage							;
		j[	STRINGIFY(e.temperatureRoom					)]	= e.temperatureRoom							;
	}

	static void deserializeJSONtoExperimentData(const nlohmann::json &j, ExperimentData &e) {

		e.experimentInProgress								= j[	STRINGIFY(e.experimentInProgress			)];
		e.experimentRecording								= j[	STRINGIFY(e.experimentRecording				)];
		e.experimentWaiting									= j[	STRINGIFY(e.experimentWaiting				)];
		e.experimentCommandsRequested						= j[	STRINGIFY(e.experimentCommandsRequested		)];
		e.experimentStage									= j[	STRINGIFY(e.experimentStage					)];
		e.verificationStep									= j[	STRINGIFY(e.verificationStep				)];
		e.experimentStepStatus								= j[	STRINGIFY(e.experimentStepStatus			)];
		e.experimentSubstepStage							= j[	STRINGIFY(e.experimentSubstepStage			)];
		e.experimentDose									= j[	STRINGIFY(e.experimentDose					)];
		e.experimentGraphPoints								= j[	STRINGIFY(e.experimentGraphPoints			)];
		e.experimentPreviousStage							= j[	STRINGIFY(e.experimentPreviousStage			)];
		e.measurementsMade									= j[	STRINGIFY(e.measurementsMade				)];
		e.timeElapsed										= j[	STRINGIFY(e.timeElapsed						)];
		e.timeStart											= j[	STRINGIFY(e.timeStart						)];
		e.timeToEquilibrate									= j[	STRINGIFY(e.timeToEquilibrate				)];
		e.timeToEquilibrateCurrent							= j[	STRINGIFY(e.timeToEquilibrateCurrent		)];
		e.injectionAttemptCounter							= j[	STRINGIFY(e.injectionAttemptCounter			)];
		e.adsorptionCounter									= j[	STRINGIFY(e.adsorptionCounter				)];
		e.desorptionCounter									= j[	STRINGIFY(e.desorptionCounter				)];
		e.pressureInitial									= j[	STRINGIFY(e.pressureInitial					)];
		e.pressureFinal										= j[	STRINGIFY(e.pressureFinal					)];
		e.pressureHighOld									= j[	STRINGIFY(e.pressureHighOld					)];
		e.resultCalorimeter									= j[	STRINGIFY(e.resultCalorimeter				)];
		e.pressureLow										= j[	STRINGIFY(e.pressureLow						)];
		e.pressureHigh										= j[	STRINGIFY(e.pressureHigh					)];
		e.temperatureCalo									= j[	STRINGIFY(e.temperatureCalo					)];
		e.temperatureCage									= j[	STRINGIFY(e.temperatureCage					)];
		e.temperatureRoom									= j[	STRINGIFY(e.temperatureRoom					)];

	}


	//*************************************************************************************************************************
	//						ExperimentSettings <> JSON
	//*************************************************************************************************************************
	static void serializeExperimentSettingsToJSON(const ExperimentSettings &e, nlohmann::json &j) {
		
		j["experimentType"]						= e.experimentType;

		// general data
		serializeDataGeneralToJSON(e.dataGeneral, j["general"]);

		// divers data
		serializeDataDiversToJSON(e.dataDivers, j["divers"]);

		// adsorption data
		for (size_t i = 0; i < e.dataAdsorption.size(); i++)
		{
			serializeDataAdsToJSON(e.dataAdsorption[i], j["ads"][i]);
		}
		
		// desorption data
		for (size_t i = 0; i < e.dataDesorption.size(); i++)
		{
			serializeDataDesToJSON(e.dataDesorption[i], j["des"][i]);
		}
	}

	static void deserializeJSONtoExperimentSettings(const nlohmann::json &j, ExperimentSettings &e) {

		e.experimentType						= j["experimentType"];

		// general data
		deserializeJSONToDataGeneral(j["general"], e.dataGeneral);

		// divers data
		deserializeJSONToDataDivers(j["divers"], e.dataDivers);

		// adsorption data
		for (size_t i = 0; i < e.dataAdsorption.size(); i++)
		{
			deserializeJSONToDataAds(j["ads"][i], e.dataAdsorption[i]);
		}
		
		// desorption data
		for (size_t i = 0; i < e.dataDesorption.size(); i++)
		{
			deserializeJSONToDataDes(j["des"][i], e.dataDesorption[i]);
		}
	}


	//*************************************************************************************************************************
	//						Data (General+Divers+Ads+Des) <> JSON
	//*************************************************************************************************************************
	static void serializeDataGeneralToJSON(const Donnees_General &m, nlohmann::json &j) {

		serializeUserToJSON(m.experimentateur, j["user"]);
		serializeGasToJSON(	m.gaz,				j["gas"]);

		j["path"			]					= UnicodeConv::ws2s(m.chemin					);
		j["comments"		]					= UnicodeConv::ws2s(m.commentaires				);
		j["experimentDate"	]					= UnicodeConv::ws2s(m.date_experience			);
		j["file"			]					= UnicodeConv::ws2s(m.fichier					);
		j["sampleMass"		]					=					m.masse_echantillon			;
		j["sampleName"		]					= UnicodeConv::ws2s(m.nom_echantillon			);
		j["temperature"		]					=					m.temperature_experience	;
	}

	void deserializeJSONToDataGeneral(const nlohmann::json & j, Donnees_General & m)
	{
		deserializeJSONToUser(j["user"]		,	m.experimentateur);
		deserializeJSONToGas( j["gas"]		,	m.gaz);

		m.chemin								= UnicodeConv::s2ws(j["path"					]);
		m.commentaires							= UnicodeConv::s2ws(j["comments"				]);
		m.date_experience						= UnicodeConv::s2ws(j["experimentDate"			]);
		m.fichier								= UnicodeConv::s2ws(j["file"					]);
		m.masse_echantillon						=					j["sampleMass"				];		 
		m.nom_echantillon						= UnicodeConv::s2ws(j["sampleName"				]);
		m.temperature_experience				=					j["temperature"				];		
	}																	



	static void serializeDataDiversToJSON(const Donnees_Divers &m, nlohmann::json &j) {

		serializeCellToJSON(m.cellule			, j["cell"]);

		j["endUnderVacuum"]						= m.mise_sous_vide_fin_experience	;
		j["baselineTime"]						= m.temps_ligne_base				;
		j["vacuumTime"]							= m.temps_vide						;
	}

	static void deserializeJSONToDataDivers(const nlohmann::json & j, Donnees_Divers & m)
	{
		deserializeJSONToCell(j["cell"]			, m.cellule);

		m.mise_sous_vide_fin_experience			= j["endUnderVacuum"				];
		m.temps_ligne_base						= j["baselineTime"					];
		m.temps_vide							= j["vacuumTime"					];
	}



	static void serializeDataAdsToJSON(const Donnees_Doses &m, nlohmann::json &j) {
		j["dP"]								= m.delta_pression						;
		j["Pfinal"]							= m.pression_finale						;
		j["Tads"]							= m.temps_adsorption					;
		j["Tref"]							= m.temps_volume						;
	}
	
	static void deserializeJSONToDataAds(const nlohmann::json & j, Donnees_Doses & m)
	{
		m.delta_pression					= j["dP"								];
		m.pression_finale					= j["Pfinal"							];
		m.temps_adsorption					= j["Tads"								];
		m.temps_volume						= j["Tref"								];
	}



	static void serializeDataDesToJSON(const Donnees_Desorption &m, nlohmann::json &j) {
		j["dP"]								= m.delta_pression						;
		j["Pfinal"]							= m.pression_finale						;
		j["Tdes"]							= m.temps_desorption					;
		j["Tref"]							= m.temps_volume						;
	}

	static void deserializeJSONToDataDes(const nlohmann::json & j, Donnees_Desorption & m)
	{
		m.delta_pression					= j["dP"								];
		m.pression_finale					= j["Pfinal"							];
		m.temps_desorption					= j["Tdes"								];
		m.temps_volume						= j["Tref"								];
	}



	//*************************************************************************************************************************
	//						gas/user/cell <> JSON
	//*************************************************************************************************************************
	static void serializeGasToJSON(const gaz &m, nlohmann::json &j) 
	{
		j["masse_moleculaire"		]		=					m.masse_moleculaire				;
		j["nom"						]		= UnicodeConv::ws2s(m.nom							);
		j["omega"					]		=					m.omega							;
		j["pression_critique"		]		=					m.pression_critique				;
		j["symbole"					]		= UnicodeConv::ws2s(m.symbole						);
		j["temperature_critique"	]		=					m.temperature_critique			;
		j["temperature_ebullition"	]		=					m.temperature_ebullition		;
	}

	static void deserializeJSONToGas(const nlohmann::json & j, gaz & m)
	{
		m.masse_moleculaire					=					j["masse_moleculaire"			];
		m.nom								= UnicodeConv::s2ws(j["nom"							]);
		m.omega								=					j["omega"						];
		m.pression_critique					=					j["pression_critique"			];
		m.symbole							= UnicodeConv::s2ws(j["symbole"						]);
		m.temperature_critique				=					j["temperature_critique"		];
		m.temperature_ebullition			=					j["temperature_ebullition"		];
	}


	static void serializeUserToJSON(const experimentateur &m, nlohmann::json &j) 
	{
		j["nom"]							= UnicodeConv::ws2s(m.nom			);
		j["surnom"]							= UnicodeConv::ws2s(m.surnom		);
	}

	static void deserializeJSONToUser(const nlohmann::json & j, experimentateur & m)
	{
		m.nom								= UnicodeConv::s2ws(j["nom"			]);	
		m.surnom							= UnicodeConv::s2ws(j["surnom"		]);	
	}


	static void serializeCellToJSON(const cellule &m, nlohmann::json &j) 
	{
		j["number"		]					= UnicodeConv::ws2s(m.numero		);
		j["caloVolume"	]					=					m.volume_calo	;
		j["totalVolume"	]					=					m.volume_total	;
	}

	static void deserializeJSONToCell(const nlohmann::json & j, cellule & m)
	{
		m.numero							= UnicodeConv::s2ws(j["number"		]);
		m.volume_calo						=					j["caloVolume"	];
		m.volume_total						=					j["totalVolume"	];
	}
}