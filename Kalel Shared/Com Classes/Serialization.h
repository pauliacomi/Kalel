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

inline void to_json(nlohmann::json &j, const Donnees_General &m);
inline void to_json(nlohmann::json &j, const Donnees_Divers &m);
inline void to_json(nlohmann::json &j, const Donnees_Doses &m);
inline void to_json(nlohmann::json &j, const Donnees_Desorption &m);

inline void from_json(const nlohmann::json &j, Donnees_General &m);
inline void from_json(const nlohmann::json &j, Donnees_Divers &m);
inline void from_json(const nlohmann::json &j, Donnees_Doses &m);
inline void from_json(const nlohmann::json &j, Donnees_Desorption &m);

inline void to_json(nlohmann::json &j, const gaz &m);
inline void to_json(nlohmann::json &j, const cellule &m);
inline void to_json(nlohmann::json &j, const experimentateur &m);

inline void from_json(const nlohmann::json &j, gaz &m);
inline void from_json(const nlohmann::json &j, cellule &m);
inline void from_json(const nlohmann::json &j, experimentateur &m);

inline void to_json(nlohmann::json &j, const ControlInstrumentState &e);
inline void from_json(const nlohmann::json &j, ControlInstrumentState &e);

//*************************************************************************************************************************
//						MachineSettings <> JSON
//*************************************************************************************************************************

void to_json(nlohmann::json &j, const MachineSettings &m) {
		
	j["CaloName"							]	= stringh::ws2s(m.CaloName)					;
	j["CaloEntete"							]	= stringh::ws2s(m.CaloEntete)				;
	j["CheminFichierGeneral"				]	= stringh::ws2s(m.CheminFichierGeneral)		;
	j["ActivationSecurite"					]	= m.ActivationSecurite						;
	j["PressionSecuriteBassePression"		]	= m.PressionSecuriteBassePression			;
	j["PressionSecuriteHautePression"		]	= m.PressionSecuriteHautePression			;
	j["PressionLimiteVide"					]	= m.PressionLimiteVide						;
	j["hasSonicNozzle"						]	= m.hasSonicNozzle					;
	j["VolumeP6"							]	= m.VolumeP6								;
	j["VolumeRef"							]	= m.VolumeRef								;

	nlohmann::json j1;
	for (const auto& kv : m.instruments) {
		j1[std::to_string(kv.first)]["type"								]	= kv.second.type				;
		j1[std::to_string(kv.first)]["port"								]	= kv.second.port				;
	}
	j["instruments"							]	= j1										;

	nlohmann::json j2;
	for (const auto& kv : m.readers) {
		j2[std::to_string(kv.first)]["type"								]	= kv.second.type				;
		j2[std::to_string(kv.first)]["identifier"						]	= kv.second.identifier			;
		j2[std::to_string(kv.first)]["sensitivity"						]	= kv.second.sensitivity			;
		j2[std::to_string(kv.first)]["channel"							]	= kv.second.channel				;
		j2[std::to_string(kv.first)]["instrument"						]	= kv.second.instrument			;
	}
	j["readers"								]	= j2										;

	nlohmann::json j3;
	for (const auto& kv : m.controllers) {
		j3[std::to_string(kv.first)]["type"								]	= kv.second.type				;
		j3[std::to_string(kv.first)]["identifier"						]	= kv.second.identifier			;
		j3[std::to_string(kv.first)]["sensitivity"						]	= kv.second.sensitivity			;
		j3[std::to_string(kv.first)]["channel"							]	= kv.second.channel				;
		j3[std::to_string(kv.first)]["instrument"						]	= kv.second.instrument			;
	}
	j["controllers"							]	= j3										;
}

inline void from_json(const nlohmann::json &j, MachineSettings &m) {

	m.CaloName											= stringh::s2ws(j["CaloName"							]);
	m.CaloEntete										= stringh::s2ws(j["CaloEntete"							]);
	m.CheminFichierGeneral								= stringh::s2ws(j["CheminFichierGeneral"				]);
	m.ActivationSecurite								=				j["ActivationSecurite"					];		
	m.PressionSecuriteBassePression						=				j["PressionSecuriteBassePression"		];
	m.PressionSecuriteHautePression						=				j["PressionSecuriteHautePression"		];
	m.PressionLimiteVide								=				j["PressionLimiteVide"					];
	m.hasSonicNozzle									=				j["hasSonicNozzle"						];
	m.VolumeP6											=				j["VolumeP6"							];
	m.VolumeRef											=				j["VolumeRef"							];

	nlohmann::json j1 = j["instruments"];
	for (nlohmann::json::iterator it = j1.begin(); it != j1.end(); ++it) {
		Instrument i;

		i.type = it.value()["type"];
		i.port = it.value()["port"];

		m.instruments.insert(std::make_pair(atoi(it.key().c_str()), i));
	}

	nlohmann::json j2 = j["readers"];
	for (nlohmann::json::iterator it = j2.begin(); it != j2.end(); ++it) {
		Reader i;

		i.type				= it.value()["type"			];
		i.identifier		= it.value()["identifier"	];
		i.sensitivity		= it.value()["sensitivity"	];
		i.channel			= it.value()["channel"		];
		i.instrument		= it.value()["instrument"	];

		m.readers.insert(std::make_pair(atoi(it.key().c_str()), i));
	}

	nlohmann::json j3 = j["controllers"];
	for (nlohmann::json::iterator it = j3.begin(); it != j3.end(); ++it) {
		Controller i;
		
		i.type				= it.value()["type"			];
		i.identifier		= it.value()["identifier"	];
		i.sensitivity		= it.value()["sensitivity"	];
		i.channel			= it.value()["channel"		];
		i.instrument		= it.value()["instrument"	];

		m.controllers.insert(std::make_pair(atoi(it.key().c_str()), i));
	}
}

//*************************************************************************************************************************
//						Instrument <> JSON
//*************************************************************************************************************************

inline void to_json(nlohmann::json &j, const Instrument &m) {
	

	}
inline void from_json(const nlohmann::json &j, Instrument &m) {
}

//*************************************************************************************************************************
//						Reader <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const Reader &m) {
}
inline void from_json(const nlohmann::json &j, Reader &m) {
	m.type												=					j["type"									];
	m.identifier										=					j["identifier"								];
	m.sensitivity										=					j["sensitivity"								];
	m.channel											=					j["channel"									];
	m.instrument										=					j["instrument"								];
}
//*************************************************************************************************************************
//						Controller <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const Controller &m) {
	j["type"							]	= m.type									;
	j["identifier"						]	= m.identifier								;
	j["sensitivity"						]	= m.sensitivity								;
	j["channel"							]	= m.channel									;
	j["instrument"						]	= m.instrument								;
}
inline void from_json(const nlohmann::json &j, Controller &m) {
	m.type												=					j["type"									];
	m.identifier										=					j["identifier"								];
	m.sensitivity										=					j["sensitivity"								];
	m.channel											=					j["channel"									];
	m.instrument										=					j["instrument"								];
}

//*************************************************************************************************************************
//						ExperimentData <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentData &e) {

	j[	"Cl"	]	= e.GetresultCalorimeter					();
	j[	"PL"	]	= e.GetpressureLow							();
	j[	"PH"	]	= e.GetpressureHigh							();
	j[	"TCl"	]	= e.GettemperatureCalo						();
	j[	"TCg"	]	= e.GettemperatureCage						();
	j[	"TR"	]	= e.GettemperatureRoom						();
}

inline void from_json(const nlohmann::json &j, ExperimentData &e) {

	e.SetresultCalorimeter									( j[	"Cl"	]);
	e.SetpressureLow										( j[	"PL"	]);
	e.SetpressureHigh										( j[	"PH"	]);
	e.SettemperatureCalo									( j[	"TCl"	]);
	e.SettemperatureCage									( j[	"TCg"	]);
	e.SettemperatureRoom									( j[	"TR"	]);

}


//*************************************************************************************************************************
//						ExperimentStatus <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentStatus &e) {

	j[	"EP"	]	= e.GetexperimentInProgress					();
	j[	"ER"	]	= e.GetexperimentRecording					();
	j[	"EW"	]	= e.GetexperimentWaiting					();
	j[	"ECR"	]	= e.GetexperimentCommandsRequested			();
	j[	"ES"	]	= e.GetexperimentStage						();
	j[	"VS"	]	= e.GetverificationStep						();
	j[	"SSs"	]	= e.GetexperimentStepStatus					();
	j[	"SSg"	]	= e.GetexperimentSubstepStage				();
	j[	"ED"	]	= e.GetexperimentDose						();
	j[	"EpS"	]	= e.GetexperimentPreviousStage				();
	j[	"tS"	]	= e.GettimeStart							();
	j[	"tE"	]	= e.GettimeElapsed							();
	j[	"tEq"	]	= e.GettimeToEquilibrate					();
	j[	"tEqC"	]	= e.GettimeToEquilibrateCurrent				();
	j[	"CIa"	]	= e.GetinjectionAttemptCounter				();
	j[	"CA"	]	= e.GetadsorptionCounter					();
	j[	"CD"	]	= e.GetdesorptionCounter					();
	j[	"PI"	]	= e.GetpressureInitial						();
	j[	"PF"	]	= e.GetpressureFinal						();
	j[	"PHo"	]	= e.GetpressureHighOld						();
}

inline void from_json(const nlohmann::json &j, ExperimentStatus &e) {

	e.SetexperimentInProgress								( j[	"EP"	]);
	e.SetexperimentRecording								( j[	"ER"	]);
	e.SetexperimentWaiting									( j[	"EW"	]);
	e.SetexperimentCommandsRequested						( j[	"ECR"	]);
	e.SetexperimentStage									( j[	"ES"	]);
	e.SetverificationStep									( j[	"VS"	]);
	e.SetexperimentStepStatus								( j[	"SSs"	]);
	e.SetexperimentSubstepStage								( j[	"SSg"	]);	  
	e.SetexperimentDose										( j[	"ED"	]);	  
	e.SetexperimentPreviousStage							( j[	"EpS"	]);
	e.SettimeStart											( j[	"tS"	]);
	e.SettimeElapsed										( j[	"tE"	]);
	e.SettimeToEquilibrate									( j[	"tEq"	]);
	e.SettimeToEquilibrateCurrent							( j[	"tEqC"	]);
	e.SetinjectionAttemptCounter							( j[	"CIa"	]);
	e.SetadsorptionCounter									( j[	"CA"	]);
	e.SetdesorptionCounter									( j[	"CD"	]);
	e.SetpressureInitial									( j[	"PI"	]);
	e.SetpressureFinal										( j[	"PF"	]);
	e.SetpressureHighOld									( j[	"PHo"	]);
}


//*************************************************************************************************************************
//						ExperimentSettings <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentSettings &e) {
		
	j["experimentType"]						= e.experimentType;

	// general data
	to_json(j["general"], e.dataGeneral);

	// divers data
	to_json(j["divers"], e.dataDivers);

	// adsorption data
	for (size_t i = 0; i < e.dataAdsorption.size(); i++)
	{
		to_json(j["ads"][i], e.dataAdsorption[i]);
	}
		
	// desorption data
	for (size_t i = 0; i < e.dataDesorption.size(); i++)
	{
		to_json(j["des"][i], e.dataDesorption[i]);
	}
}

inline void from_json(const nlohmann::json &j, ExperimentSettings &e) {

	e.experimentType						= j["experimentType"];

	// general data
	from_json(j["general"], e.dataGeneral);

	// divers data
	from_json(j["divers"], e.dataDivers);

	// adsorption data
	for (size_t i = 0; i < e.dataAdsorption.size(); i++)
	{
		from_json(j["ads"][i], e.dataAdsorption[i]);
	}
		
	// desorption data
	for (size_t i = 0; i < e.dataDesorption.size(); i++)
	{
		from_json(j["des"][i], e.dataDesorption[i]);
	}
}


//*************************************************************************************************************************
//						ExperimentData (General+Divers+Ads+Des) <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const Donnees_General &m) {

	to_json(j["user"]	, m.experimentateur	);
	to_json(j["gas"]	, m.gaz				);

	j["path"			]					= stringh::ws2s(m.chemin					);
	j["comments"		]					= stringh::ws2s(m.commentaires				);
	j["experimentDate"	]					= stringh::ws2s(m.date_experience			);
	j["file"			]					= stringh::ws2s(m.fichier					);
	j["sampleMass"		]					=					m.masse_echantillon			;
	j["sampleName"		]					= stringh::ws2s(m.nom_echantillon			);
	j["temperature"		]					=					m.temperature_experience	;
}

void from_json(const nlohmann::json & j, Donnees_General & m)
{
	from_json(j["user"]		,	m.experimentateur);
	from_json( j["gas"]		,	m.gaz);

	m.chemin								= stringh::s2ws(j["path"					]);
	m.commentaires							= stringh::s2ws(j["comments"				]);
	m.date_experience						= stringh::s2ws(j["experimentDate"			]);
	m.fichier								= stringh::s2ws(j["file"					]);
	m.masse_echantillon						=					j["sampleMass"				];		 
	m.nom_echantillon						= stringh::s2ws(j["sampleName"				]);
	m.temperature_experience				=					j["temperature"				];		
}																	



inline void to_json(nlohmann::json &j, const Donnees_Divers &m) {

	to_json(j["cell"], m.cellule);

	j["endUnderVacuum"]						= m.mise_sous_vide_fin_experience	;
	j["baselineTime"]						= m.temps_ligne_base				;
	j["vacuumTime"]							= m.temps_vide						;
}

inline void from_json(const nlohmann::json & j, Donnees_Divers & m)
{
	from_json(j["cell"]			, m.cellule);

	m.mise_sous_vide_fin_experience			= j["endUnderVacuum"				];
	m.temps_ligne_base						= j["baselineTime"					];
	m.temps_vide							= j["vacuumTime"					];
}



inline void to_json(nlohmann::json &j, const Donnees_Doses &m) {
	j["dP"]									= m.delta_pression						;
	j["Pfinal"]								= m.pression_finale						;
	j["Tads"]								= m.temps_adsorption					;
	j["Tref"]								= m.temps_volume						;
}
	
inline void from_json(const nlohmann::json & j, Donnees_Doses & m)
{
	m.delta_pression						= j["dP"								];
	m.pression_finale						= j["Pfinal"							];
	m.temps_adsorption						= j["Tads"								];
	m.temps_volume							= j["Tref"								];
}



inline void to_json(nlohmann::json &j, const Donnees_Desorption &m) {
	j["dP"]									= m.delta_pression						;
	j["Pfinal"]								= m.pression_finale						;
	j["Tdes"]								= m.temps_desorption					;
	j["Tref"]								= m.temps_volume						;
}

inline void from_json(const nlohmann::json & j, Donnees_Desorption & m)
{
	m.delta_pression						= j["dP"								];
	m.pression_finale						= j["Pfinal"							];
	m.temps_desorption						= j["Tdes"								];
	m.temps_volume							= j["Tref"								];
}



//*************************************************************************************************************************
//						gas/user/cell <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const gaz &m) 
{
	j["masse_moleculaire"		]			=					m.masse_moleculaire				;
	j["nom"						]			= stringh::ws2s(m.nom							);
	j["omega"					]			=					m.omega							;
	j["pression_critique"		]			=					m.pression_critique				;
	j["symbole"					]			= stringh::ws2s(m.symbole						);
	j["temperature_critique"	]			=					m.temperature_critique			;
	j["temperature_ebullition"	]			=					m.temperature_ebullition		;
}

inline void from_json(const nlohmann::json & j, gaz & m)
{
	m.masse_moleculaire						=					j["masse_moleculaire"			];
	m.nom									= stringh::s2ws(j["nom"							]);
	m.omega									=					j["omega"						];
	m.pression_critique						=					j["pression_critique"			];
	m.symbole								= stringh::s2ws(j["symbole"						]);
	m.temperature_critique					=					j["temperature_critique"		];
	m.temperature_ebullition				=					j["temperature_ebullition"		];
}


inline void to_json(nlohmann::json &j, const experimentateur &m) 
{
	j["nom"]								= stringh::ws2s(m.nom			);
	j["surnom"]								= stringh::ws2s(m.surnom		);
}

inline void from_json(const nlohmann::json & j, experimentateur & m)
{
	m.nom									= stringh::s2ws(j["nom"			]);	
	m.surnom								= stringh::s2ws(j["surnom"		]);	
}


inline void to_json(nlohmann::json &j, const cellule &m) 
{
	j["number"		]						= stringh::ws2s(m.numero		);
	j["caloVolume"	]						=					m.volume_calo	;
	j["totalVolume"	]						=					m.volume_total	;
}

inline void from_json(const nlohmann::json & j, cellule & m)
{
	m.numero								= stringh::s2ws(j["number"		]);
	m.volume_calo							=					j["caloVolume"	];
	m.volume_total							=					j["totalVolume"	];
}



//*************************************************************************************************************************
//						ControlInstrumentState <> JSON
//*************************************************************************************************************************



inline void to_json(nlohmann::json & j, const ControlInstrumentState & e)
{
	for (size_t i = 0; i < e.valves.size(); i++)
	{
		j["valve"][i] = e.valves[i];
	}
	for (size_t i = 0; i < e.EVs.size(); i++)
	{
		j["ev"][i] = e.EVs[i];
	}
	for (size_t i = 0; i < e.pumps.size(); i++)
	{
		j["pump"][i] = e.pumps[i];
	}
}

inline void from_json(const nlohmann::json & j, ControlInstrumentState & e)
{
	auto j1 = j["valve"].get<std::vector<bool>>();
	for (auto i = 0; i < j1.size(); ++i)
	{
		e.valves[i]											=	j1[i]	;
	}
		
	j1 = j["ev"].get<std::vector<bool>>();
	for (auto i = 0; i < j1.size(); ++i)
	{
		e.EVs[i]											=	j1[i]	;
	}

	j1 = j["pump"].get<std::vector<bool>>();
	for (auto i = 0; i < j1.size(); ++i)
	{
		e.pumps[i]											=	j1[i]	;
	}
}
