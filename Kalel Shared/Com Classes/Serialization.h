#pragma once

#include "../Netcode/json.hpp"
#include "../Com Classes/ExperimentData.h"
#include "../Com Classes/ExperimentSettings.h"
#include "../Com Classes/ExperimentStatus.h"
#include "../Com Classes/MachineSettings.h"
#include "../Com Classes/ControlInstrumentState.h"
#include "../stringHelpers.h"

inline void to_json(nlohmann::json &j, const MachineSettings &m);
inline void from_json(const nlohmann::json &j, MachineSettings &m);

inline void to_json(nlohmann::json &j, const Instrument &m);
inline void from_json(const nlohmann::json &j, Instrument &m);
inline void to_json(nlohmann::json &j, const Reader &m);
inline void from_json(const nlohmann::json &j, Reader &m);
inline void to_json(nlohmann::json &j, const Controller &m);
inline void from_json(const nlohmann::json &j, Controller &m);

inline void to_json(nlohmann::json &j, const ExperimentData &e);
inline void from_json(const nlohmann::json &j, ExperimentData &e);

inline void to_json(nlohmann::json &j, const ExperimentSettings &e);
inline void from_json(const nlohmann::json &j, ExperimentSettings &e);

inline void to_json(nlohmann::json &j, const ExperimentStatus &e);
inline void from_json(const nlohmann::json &j, ExperimentStatus &e);

inline void to_json(nlohmann::json &j, const data_general &m);
inline void to_json(nlohmann::json &j, const data_other &m);
inline void to_json(nlohmann::json &j, const data_adsorption &m);
inline void to_json(nlohmann::json &j, const data_desorption &m);

inline void from_json(const nlohmann::json &j, data_general &m);
inline void from_json(const nlohmann::json &j, data_other &m);
inline void from_json(const nlohmann::json &j, data_adsorption &m);
inline void from_json(const nlohmann::json &j, data_desorption &m);

inline void to_json(nlohmann::json &j, const gas &m);
inline void to_json(nlohmann::json &j, const cell &m);
inline void to_json(nlohmann::json &j, const user &m);

inline void from_json(const nlohmann::json &j, gas &m);
inline void from_json(const nlohmann::json &j, cell &m);
inline void from_json(const nlohmann::json &j, user &m);


//*************************************************************************************************************************
//						MachineSettings <> JSON
//*************************************************************************************************************************

void to_json(nlohmann::json &j, const MachineSettings &m) {

	j["timestamp"] = timeh::TimePointToISOString(m.tp);

	j["CaloName"							]	= stringh::ws2s(m.CaloName)					;
	j["CaloPrefix"							]	= stringh::ws2s(m.CaloPrefix)				;
	j["DefaultPath"							]	= stringh::ws2s(m.DefaultPath)		;
	j["SafetyOn"							]	= m.SafetyOn								;

	j["hasSonicNozzle"						]	= m.hasSonicNozzle							;
	j["VolumeP6"							]	= m.VolumeP6								;
	j["VolumeRef"							]	= m.VolumeRef								;
	
	j["PressurePumpVacuum"					]	= m.PressurePumpVacuum						;
	j["PressureLimitVacuum"					]	= m.PressureLimitVacuum						;
												 											
	j["InjectionAttemptNumber"				]	= m.InjectionAttemptNumber					;
	j["InjectionMargin"						]	= m.InjectionMargin							;
	j["InjectionMultiplier"					]	= m.InjectionMultiplier						;
												
	j["TimeBetweenMeasurement"				]	= m.TimeBetweenMeasurement					;
	j["TimeBetweenRecording"				]	= m.TimeBetweenRecording					;
	j["TimeBetweenAutomation"				]	= m.TimeBetweenAutomation					;
	j["TimeWaitValves"						]	= m.TimeWaitValves							;
	j["TimeWaitValvesShort"					]	= m.TimeWaitValvesShort						;
	j["TimeWaitPump"						]	= m.TimeWaitPump							;
	j["TimeVacuumEmergency"					]	= m.TimeVacuumEmergency						;
	j["TimeVacuumBottle"					]	= m.TimeVacuumBottle						;
	j["TimeVacuumEndDefault"				]	= m.TimeVacuumEndDefault					;

	nlohmann::json j1;
	for (const auto& kv : m.instruments) {
		j1[std::to_string(kv.first)] = kv.second;
	}
	j["instruments"]	= j1;

	nlohmann::json j2;
	for (const auto& kv : m.readers) {
		j2[std::to_string(kv.first)] = kv.second;
	}
	j["readers"]		= j2;

	nlohmann::json j3;
	for (const auto& kv : m.controllers) {
		j3[std::to_string(kv.first)] = kv.second;
	}
	j["controllers"]	= j3;
}

inline void from_json(const nlohmann::json &j, MachineSettings &m) {

	m.tp = timeh::ISOStringToTimePoint(j["timestamp"]);

	m.CaloName											= stringh::s2ws(j["CaloName"							]);
	m.CaloPrefix										= stringh::s2ws(j["CaloPrefix"							]);
	m.DefaultPath										= stringh::s2ws(j["DefaultPath"							]);
	m.SafetyOn											=				j["SafetyOn"							];	
	
	m.hasSonicNozzle									=				j["hasSonicNozzle"						];
	m.VolumeP6											=				j["VolumeP6"							];
	m.VolumeRef											=				j["VolumeRef"							];

	m.PressurePumpVacuum								=				j["PressurePumpVacuum"					];
	m.PressureLimitVacuum								=				j["PressureLimitVacuum"					];

	m.InjectionAttemptNumber							=				j["InjectionAttemptNumber"				];
	m.InjectionMargin									=				j["InjectionMargin"						];
	m.InjectionMultiplier								=				j["InjectionMultiplier"					];

	m.TimeBetweenMeasurement							=				j["TimeBetweenMeasurement"				];
	m.TimeBetweenRecording								=				j["TimeBetweenRecording"				];
	m.TimeBetweenAutomation								=				j["TimeBetweenAutomation"				];
	m.TimeWaitValves									=				j["TimeWaitValves"						];
	m.TimeWaitValvesShort								=				j["TimeWaitValvesShort"					];
	m.TimeWaitPump										=				j["TimeWaitPump"						];
	m.TimeVacuumEmergency								=				j["TimeVacuumEmergency"					];
	m.TimeVacuumBottle									=				j["TimeVacuumBottle"					];
	m.TimeVacuumEndDefault								=				j["TimeVacuumEndDefault"				];

	nlohmann::json j1 = j["instruments"];
	for (nlohmann::json::iterator it = j1.begin(); it != j1.end(); ++it) {

		m.instruments.emplace(atoi(it.key().c_str()), it.value());
	}

	nlohmann::json j2 = j["readers"];
	for (nlohmann::json::iterator it = j2.begin(); it != j2.end(); ++it) {

		m.readers.emplace(atoi(it.key().c_str()), it.value());
	}

	nlohmann::json j3 = j["controllers"];
	for (nlohmann::json::iterator it = j3.begin(); it != j3.end(); ++it) {

		m.controllers.emplace(atoi(it.key().c_str()), it.value());
	}
}

//*************************************************************************************************************************
//						Instrument <> JSON
//*************************************************************************************************************************

inline void to_json(nlohmann::json &j, const Instrument &m) {
	j["type"]		=				  m.type		;
	j["portType"]	=	stringh::ws2s(m.port_type)	;
	j["port"]		=				  m.port		;
	}
inline void from_json(const nlohmann::json &j, Instrument &m) {
	m.type			=					  j["type"]		;
	m.port_type		=		stringh::s2ws(j["portType"]);
	m.port			=					  j["port"]		;
}

//*************************************************************************************************************************
//						Reader <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const Reader &m) {
	j["type"									]		= m.type														;
	j["identifier"								]		= m.identifier													;
	j["sensitivity"								]		= m.sensitivity													;
	j["safe_min"								]		= m.safe_min													;
	j["safe_max"								]		= m.safe_max													;
	j["channel"									]		= m.channel														;
	j["instrument"								]		= m.instrument													;
}

inline void from_json(const nlohmann::json &j, Reader &m) {
	m.type												=					j["type"									];
	m.identifier										=					j["identifier"								];
	m.sensitivity										=					j["sensitivity"								];
	m.safe_min											=					j["safe_min"								];
	m.safe_max											=					j["safe_max"								];
	m.channel											=					j["channel"									];
	m.instrument										=					j["instrument"								];
}
//*************************************************************************************************************************
//						Controller <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const Controller &m) {
	j["type"							]	= m.type									;
	j["identifier"						]	= m.identifier								;
	j["instrument"						]	= m.instrument								;
	j["channel"							]	= m.channel									;
	j["subchannel"						]	= m.subchannel								;
}

inline void from_json(const nlohmann::json &j, Controller &m) {
	m.type												=					j["type"									];
	m.identifier										=					j["identifier"								];
	m.instrument										=					j["instrument"								];
	m.channel											=					j["channel"									];
	m.subchannel										=					j["subchannel"								];
}

//*************************************************************************************************************************
//						ExperimentData <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentData &e) {

	j[	"Cl"	]	= e.resultCalorimeter					;
	j[	"PL"	]	= e.pressureLow							;
	j[	"PH"	]	= e.pressureHigh						;
	j[	"TCl"	]	= e.temperatureCalo						;
	j[	"TCg"	]	= e.temperatureCage						;
	j[	"TR"	]	= e.temperatureRoom						;
}

inline void from_json(const nlohmann::json &j, ExperimentData &e) {

	e.resultCalorimeter										= j[	"Cl"	];
	e.pressureLow											= j[	"PL"	];
	e.pressureHigh											= j[	"PH"	];
	e.temperatureCalo										= j[	"TCl"	];
	e.temperatureCage										= j[	"TCg"	];
	e.temperatureRoom										= j[	"TR"	];

}


//*************************************************************************************************************************
//						ExperimentStatus <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentStatus &e) {

	j["timestamp"] = timeh::TimePointToISOString(e.tp);

	j[	"EP"	]	= e.experimentInProgress				.load();
	j[	"ER"	]	= e.experimentRecording					.load();
	j[	"EW"	]	= e.experimentWaiting					.load();
	j[	"ECR"	]	= e.experimentCommandsRequested			.load();
	j[	"ES"	]	= e.experimentStage						.load();
	j[	"VS"	]	= e.verificationStep					.load();
	j[	"SSs"	]	= e.experimentStepStatus				.load();
	j[	"SSg"	]	= e.experimentSubstepStage				.load();
	j[	"ED"	]	= e.experimentDose						.load();
	j[	"EpS"	]	= e.experimentPreviousStage				.load();
	j[	"tS"	]	= e.timeStart							.load();
	j[	"tEq"	]	= e.timeToEquilibrate					.load();
	j[	"tEqS"	]	= e.timeEquilibrationStart				.load();
	j[	"CIa"	]	= e.injectionAttemptCounter				.load();
	j[	"CA"	]	= e.adsorptionCounter					.load();
	j[	"CD"	]	= e.desorptionCounter					.load();
	j[	"PI"	]	= e.pressureInitial						.load();
	j[	"PF"	]	= e.pressureFinal						.load();
	j[	"PHo"	]	= e.pressureHighOld						.load();
}

inline void from_json(const nlohmann::json &j, ExperimentStatus &e) {

	e.tp = timeh::ISOStringToTimePoint(j["timestamp"]);

	e.experimentInProgress								.store(j[	"EP"	]);
	e.experimentRecording								.store(j[	"ER"	]);
	e.experimentWaiting									.store(j[	"EW"	]);
	e.experimentCommandsRequested						.store(j[	"ECR"	]);
	
	e.experimentStage									.store(j[	"ES"	]);
	e.experimentPreviousStage							.store(j[	"EpS"	]);
	e.verificationStep									.store(j[	"VS"	]);
	e.experimentStepStatus								.store(j[	"SSs"	]);
	e.experimentSubstepStage							.store(j[	"SSg"	]);
	
	e.experimentDose									.store(j[	"ED"	]);
	e.injectionAttemptCounter							.store(j[	"CIa"	]);
	e.adsorptionCounter									.store(j[	"CA"	]);
	e.desorptionCounter									.store(j[	"CD"	]);

	e.timeStart											.store(j[	"tS"	]);
	e.timeToEquilibrate									.store(j[	"tEq"	]);
	e.timeEquilibrationStart							.store(j[	"tEqS"	]);

	e.pressureInitial									.store(j[	"PI"	]);
	e.pressureFinal										.store(j[	"PF"	]);
	e.pressureHighOld									.store(j[	"PHo"	]);
}


//*************************************************************************************************************************
//						ExperimentSettings <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentSettings &e) {

	j["timestamp"] = timeh::TimePointToISOString(e.tp);

	j["experimentType"]						= e.experimentType;

	// general data
	j["general"]							= e.dataGeneral;

	// divers data
	j["divers"]								= e.dataDivers;

	// adsorption data
	j["ads"]								= e.dataAdsorption;
		
	// desorption data
	j["des"]								= e.dataDesorption;
}

inline void from_json(const nlohmann::json &j, ExperimentSettings &e) {

	e.tp = timeh::ISOStringToTimePoint(j["timestamp"]);

	e.experimentType						= j["experimentType"];

	// general data
	e.dataGeneral							= j["general"];

	// divers data
	e.dataDivers							= j["divers"];

	// adsorption data
	e.dataAdsorption						= j["ads"].get<std::vector<data_adsorption>>();

	// desorption data
	e.dataDesorption						= j["des"].get<std::vector<data_desorption>>();;
}


//*************************************************************************************************************************
//						ExperimentData (General+Divers+Ads+Des) <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const data_general &m) {

	
	j["user"]								= m.user;
	j["gas"]								= m.gas;

	j["path"			]					= stringh::ws2s(m.chemin					);
	j["comments"		]					= stringh::ws2s(m.commentaires				);
	j["experimentDate"	]					= stringh::ws2s(m.date_experience			);
	j["file"			]					= stringh::ws2s(m.fichier					);
	j["sampleMass"		]					=				m.masse_echantillon			;
	j["sampleName"		]					= stringh::ws2s(m.nom_echantillon			);
	j["temperature"		]					=					m.temperature_experience;
	j["temperatureInitialCheck"]			=					m.temperature_range_initial_check;
}

void from_json(const nlohmann::json & j, data_general & m)
{
	m.user									= j["user"];
	m.gas									= j["gas"];

	m.chemin								= stringh::s2ws(j["path"					]);
	m.commentaires							= stringh::s2ws(j["comments"				]);
	m.date_experience						= stringh::s2ws(j["experimentDate"			]);
	m.fichier								= stringh::s2ws(j["file"					]);
	m.masse_echantillon						=				j["sampleMass"				];		 
	m.nom_echantillon						= stringh::s2ws(j["sampleName"				]);
	m.temperature_experience				=				j["temperature"				];	
	m.temperature_range_initial_check		=				j["temperatureInitialCheck"	];	
}																	



inline void to_json(nlohmann::json &j, const data_other &m) {

	j["cell"]								= m.cell							;
	j["endUnderVacuum"]						= m.mise_sous_vide_fin_experience	;
	j["baselineTime"]						= m.time_baseline					;
	j["vacuumTime"]							= m.temps_vide						;
}

inline void from_json(const nlohmann::json & j, data_other & m)
{
	m.cell									= j["cell"							];
	m.mise_sous_vide_fin_experience			= j["endUnderVacuum"				];
	m.time_baseline							= j["baselineTime"					];
	m.temps_vide							= j["vacuumTime"					];
}



inline void to_json(nlohmann::json &j, const data_adsorption &m) {
	j["dP"]									= m.delta_pression						;
	j["Pfinal"]								= m.pression_finale						;
	j["Tads"]								= m.temps_adsorption					;
	j["Tref"]								= m.temps_volume						;
}
	
inline void from_json(const nlohmann::json & j, data_adsorption & m)
{
	m.delta_pression						= j["dP"								];
	m.pression_finale						= j["Pfinal"							];
	m.temps_adsorption						= j["Tads"								];
	m.temps_volume							= j["Tref"								];
}



inline void to_json(nlohmann::json &j, const data_desorption &m) {
	j["dP"]									= m.delta_pression						;
	j["Pfinal"]								= m.pression_finale						;
	j["Tdes"]								= m.temps_desorption					;
	j["Tref"]								= m.temps_volume						;
}

inline void from_json(const nlohmann::json & j, data_desorption & m)
{
	m.delta_pression						= j["dP"								];
	m.pression_finale						= j["Pfinal"							];
	m.temps_desorption						= j["Tdes"								];
	m.temps_volume							= j["Tref"								];
}



//*************************************************************************************************************************
//						gas/user/cell <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const gas &m) 
{
	j["masse_moleculaire"		]			=					m.masse_moleculaire				;
	j["nom"						]			= stringh::ws2s(m.nom							);
	j["omega"					]			=					m.omega							;
	j["pression_critique"		]			=					m.pression_critique				;
	j["symbole"					]			= stringh::ws2s(m.symbole						);
	j["temperature_critique"	]			=					m.temperature_critique			;
	j["temperature_ebullition"	]			=					m.temperature_ebullition		;
}

inline void from_json(const nlohmann::json & j, gas & m)
{
	m.masse_moleculaire						=					j["masse_moleculaire"			];
	m.nom									= stringh::s2ws(j["nom"							]);
	m.omega									=					j["omega"						];
	m.pression_critique						=					j["pression_critique"			];
	m.symbole								= stringh::s2ws(j["symbole"						]);
	m.temperature_critique					=					j["temperature_critique"		];
	m.temperature_ebullition				=					j["temperature_ebullition"		];
}


inline void to_json(nlohmann::json &j, const user &m) 
{
	j["nom"]								= stringh::ws2s(m.nom			);
	j["surnom"]								= stringh::ws2s(m.surnom		);
}

inline void from_json(const nlohmann::json & j, user & m)
{
	m.nom									= stringh::s2ws(j["nom"			]);	
	m.surnom								= stringh::s2ws(j["surnom"		]);	
}


inline void to_json(nlohmann::json &j, const cell &m) 
{
	j["number"		]						= stringh::ws2s(m.numero		);
	j["caloVolume"	]						=					m.volume_calo	;
	j["totalVolume"	]						=					m.volume_total	;
}

inline void from_json(const nlohmann::json & j, cell & m)
{
	m.numero								= stringh::s2ws(j["number"		]);
	m.volume_calo							=					j["caloVolume"	];
	m.volume_total							=					j["totalVolume"	];
}
