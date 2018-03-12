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
	j["hasSonicNozzle"						]	= m.hasSonicNozzle							;
	j["VolumeP6"							]	= m.VolumeP6								;
	j["VolumeRef"							]	= m.VolumeRef								;

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
	j["channel"									]		= m.channel														;
	j["instrument"								]		= m.instrument													;
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
	j[	"tEq"	]	= e.GettimeToEquilibrate					();
	j[	"tEqS"	]	= e.GettimeEquilibrateStart					();
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
	e.SettimeToEquilibrate									( j[	"tEq"	]);
	e.SettimeEquilibrateStart								( j[	"tEqS"	]);
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
inline void to_json(nlohmann::json &j, const data_general &m) {

	to_json(j["user"]	, m.user	);
	to_json(j["gas"]	, m.gas				);

	j["path"			]					= stringh::ws2s(m.chemin					);
	j["comments"		]					= stringh::ws2s(m.commentaires				);
	j["experimentDate"	]					= stringh::ws2s(m.date_experience			);
	j["file"			]					= stringh::ws2s(m.fichier					);
	j["sampleMass"		]					=					m.masse_echantillon			;
	j["sampleName"		]					= stringh::ws2s(m.nom_echantillon			);
	j["temperature"		]					=					m.temperature_experience	;
}

void from_json(const nlohmann::json & j, data_general & m)
{
	from_json(j["user"]		,	m.user);
	from_json( j["gas"]		,	m.gas);

	m.chemin								= stringh::s2ws(j["path"					]);
	m.commentaires							= stringh::s2ws(j["comments"				]);
	m.date_experience						= stringh::s2ws(j["experimentDate"			]);
	m.fichier								= stringh::s2ws(j["file"					]);
	m.masse_echantillon						=					j["sampleMass"				];		 
	m.nom_echantillon						= stringh::s2ws(j["sampleName"				]);
	m.temperature_experience				=					j["temperature"				];		
}																	



inline void to_json(nlohmann::json &j, const data_other &m) {

	to_json(j["cell"], m.cell);

	j["endUnderVacuum"]						= m.mise_sous_vide_fin_experience	;
	j["baselineTime"]						= m.temps_ligne_base				;
	j["vacuumTime"]							= m.temps_vide						;
}

inline void from_json(const nlohmann::json & j, data_other & m)
{
	from_json(j["cell"]			, m.cell);

	m.mise_sous_vide_fin_experience			= j["endUnderVacuum"				];
	m.temps_ligne_base						= j["baselineTime"					];
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
