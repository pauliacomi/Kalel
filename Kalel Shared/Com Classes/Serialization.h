#pragma once

#define STRINGIFY(Variable) (void(Variable),#Variable)

#include "../Netcode/json.hpp"
#include "../Com Classes/ExperimentData.h"
#include "../Com Classes/ExperimentSettings.h"
#include "../Com Classes/MachineSettings.h"
#include "../Com Classes/ControlInstrumentState.h"
#include "../unicodeConv.h"
#include "../timeHelpers.h"

#include <chrono>

inline void to_json(nlohmann::json &j, const MachineSettings &m);
inline void from_json(const nlohmann::json &j, MachineSettings &m);

inline void to_json(nlohmann::json &j, const ExperimentData &e);
inline void from_json(const nlohmann::json &j, ExperimentData &e);

inline void to_json(nlohmann::json &j, const ExperimentSettings &e);
inline void from_json(const nlohmann::json &j, ExperimentSettings &e);

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
//						timpoint <> JSON
//*************************************************************************************************************************

//namespace nlohmann {
//	template <>
//	struct adl_serializer<std::chrono::time_point<std::chrono::system_clock>> {
//		void to_json(nlohmann::json & j, const std::chrono::time_point<std::chrono::system_clock> & m)
//		{
//			j = nlohmann::json{ timeh::TimePointToString(m) };
//		}
//
//		void from_json(const nlohmann::json & j, std::chrono::time_point<std::chrono::system_clock> & m)
//		{
//			m = timeh::StringToTimePoint(j.get<std::string>());
//		}
//	};
//}

//*************************************************************************************************************************
//						MachineSettings <> JSON
//*************************************************************************************************************************

inline void to_json(nlohmann::json &j, const MachineSettings &m) {
		
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
	j[STRINGIFY(m.PortValves						)]	= m.PortValves								;
	j[STRINGIFY(m.PresenceTuyereSonique				)]	= m.PresenceTuyereSonique					;
	j[STRINGIFY(m.PressionLimiteVide				)]	= m.PressionLimiteVide						;
	j[STRINGIFY(m.PressionSecuriteBassePression		)]	= m.PressionSecuriteBassePression			;
	j[STRINGIFY(m.PressionSecuriteHautePression		)]	= m.PressionSecuriteHautePression			;
	j[STRINGIFY(m.SensitivityCalo					)]	= m.SensitivityCalo							;
	j[STRINGIFY(m.SensitivityLowPRange	)]	= m.SensitivityLowPRange			;
	j[STRINGIFY(m.SensitivityHighPRange	)]	= m.SensitivityHighPRange			;
	j[STRINGIFY(m.VolumeP6							)]	= m.VolumeP6								;
	j[STRINGIFY(m.VolumeRef							)]	= m.VolumeRef								;
	j[STRINGIFY(m.COMInstruments					)]	= m.COMInstruments							;
	j[STRINGIFY(m.FunctionInstruments				)]	= m.FunctionInstruments						;
	j[STRINGIFY(m.typeInstruments					)]	= m.typeInstruments							;

}

inline void from_json(const nlohmann::json &j, MachineSettings &m) {

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
	m.PortValves										=					j[STRINGIFY(m.PortValves								)];							
	m.PresenceTuyereSonique								=					j[STRINGIFY(m.PresenceTuyereSonique						)];				
	m.PressionLimiteVide								=					j[STRINGIFY(m.PressionLimiteVide						)];					
	m.PressionSecuriteBassePression						=					j[STRINGIFY(m.PressionSecuriteBassePression				)];		
	m.PressionSecuriteHautePression						=					j[STRINGIFY(m.PressionSecuriteHautePression				)];		
	m.SensitivityCalo									=					j[STRINGIFY(m.SensitivityCalo							)];					
	m.SensitivityLowPRange					=					j[STRINGIFY(m.SensitivityLowPRange			)];	
	m.SensitivityHighPRange					=					j[STRINGIFY(m.SensitivityHighPRange			)];	
	m.VolumeP6											=					j[STRINGIFY(m.VolumeP6									)];							
	m.VolumeRef											=					j[STRINGIFY(m.VolumeRef									)];							
	m.COMInstruments									=					j[STRINGIFY(m.COMInstruments							)].get<std::vector<int>>();						
	m.FunctionInstruments								=					j[STRINGIFY(m.FunctionInstruments						)].get<std::vector<int>>();
	m.typeInstruments									=					j[STRINGIFY(m.typeInstruments							)].get<std::vector<int>>();

}

//*************************************************************************************************************************
//						ExperimentData <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const ExperimentData &e) {

	j[	"EP"	]	= e.GetexperimentInProgress					();
	j[	"ER"	]	= e.GetexperimentRecording					();
	j[	"EW"	]	= e.GetexperimentWaiting					();
	j[	"ECR"	]	= e.GetexperimentCommandsRequested			();
	j[	"ES"	]	= e.GetexperimentStage						();
	j[	"VS"	]	= e.GetverificationStep						();
	j[	"SSs"	]	= e.GetexperimentStepStatus					();
	j[	"SSg"	]	= e.GetexperimentSubstepStage				();
	j[	"ED"	]	= e.GetexperimentDose						();
	j[	"GP"	]	= e.GetexperimentGraphPoints				();
	j[	"MM"	]	= e.GetexperimentPreviousStage				();
	j[	"TS"	]	= e.GetmeasurementsMade						();
	j[	"TE"	]	= e.GettimeStart							();
	j[	"TtE"	]	= e.GettimeElapsed							();
	j[	"TtEq"	]	= e.GettimeToEquilibrate					();
	j[	"TtEqC"	]	= e.GettimeToEquilibrateCurrent				();
	j[	"CIa"	]	= e.GetinjectionAttemptCounter				();
	j[	"CA"	]	= e.GetadsorptionCounter					();
	j[	"CD"	]	= e.GetdesorptionCounter					();
	j[	"PI"	]	= e.GetpressureInitial						();
	j[	"PF"	]	= e.GetpressureFinal						();
	j[	"PHo"	]	= e.GetpressureHighOld						();
	j[	"RCl"	]	= e.GetresultCalorimeter					();
	j[	"PL"	]	= e.GetpressureLow							();
	j[	"PH"	]	= e.GetpressureHigh							();
	j[	"TCl"	]	= e.GettemperatureCalo						();
	j[	"TCg"	]	= e.GettemperatureCage						();
	j[	"TR"	]	= e.GettemperatureRoom						();
}

inline void from_json(const nlohmann::json &j, ExperimentData &e) {

	e.SetexperimentInProgress								( j[	"EP"	]);
	e.SetexperimentRecording								( j[	"ER"	]);
	e.SetexperimentWaiting									( j[	"EW"	]);
	e.SetexperimentCommandsRequested						( j[	"ECR"	]);
	e.SetexperimentStage									( j[	"ES"	]);
	e.SetverificationStep									( j[	"VS"	]);
	e.SetexperimentStepStatus								( j[	"SSs"	]);
	e.SetexperimentSubstepStage								( j[	"SSg"	]);	  
	e.SetexperimentDose										( j[	"ED"	]);	  
	e.SetexperimentGraphPoints								( j[	"GP"	]);
	e.SetexperimentPreviousStage							( j[	"MM"	]);
	e.SetmeasurementsMade									( j[	"TS"	]);
	e.SettimeStart											( j[	"TE"	]);
	e.SettimeElapsed										( j[	"TtE"	]);
	e.SettimeToEquilibrate									( j[	"TtEq"	]);
	e.SettimeToEquilibrateCurrent							( j[	"TtEqC"	]);
	e.SetinjectionAttemptCounter							( j[	"CIa"	]);
	e.SetadsorptionCounter									( j[	"CA"	]);
	e.SetdesorptionCounter									( j[	"CD"	]);
	e.SetpressureInitial									( j[	"PI"	]);
	e.SetpressureFinal										( j[	"PF"	]);
	e.SetpressureHighOld									( j[	"PHo"	]);
	e.SetresultCalorimeter									( j[	"RCl"	]);
	e.SetpressureLow										( j[	"PL"	]);
	e.SetpressureHigh										( j[	"PH"	]);
	e.SettemperatureCalo									( j[	"TCl"	]);
	e.SettemperatureCage									( j[	"TCg"	]);
	e.SettemperatureRoom									( j[	"TR"	]);

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
//						Data (General+Divers+Ads+Des) <> JSON
//*************************************************************************************************************************
inline void to_json(nlohmann::json &j, const Donnees_General &m) {

	to_json(j["user"]	, m.experimentateur	);
	to_json(j["gas"]	, m.gaz				);

	j["path"			]					= UnicodeConv::ws2s(m.chemin					);
	j["comments"		]					= UnicodeConv::ws2s(m.commentaires				);
	j["experimentDate"	]					= UnicodeConv::ws2s(m.date_experience			);
	j["file"			]					= UnicodeConv::ws2s(m.fichier					);
	j["sampleMass"		]					=					m.masse_echantillon			;
	j["sampleName"		]					= UnicodeConv::ws2s(m.nom_echantillon			);
	j["temperature"		]					=					m.temperature_experience	;
}

void from_json(const nlohmann::json & j, Donnees_General & m)
{
	from_json(j["user"]		,	m.experimentateur);
	from_json( j["gas"]		,	m.gaz);

	m.chemin								= UnicodeConv::s2ws(j["path"					]);
	m.commentaires							= UnicodeConv::s2ws(j["comments"				]);
	m.date_experience						= UnicodeConv::s2ws(j["experimentDate"			]);
	m.fichier								= UnicodeConv::s2ws(j["file"					]);
	m.masse_echantillon						=					j["sampleMass"				];		 
	m.nom_echantillon						= UnicodeConv::s2ws(j["sampleName"				]);
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
	j["nom"						]			= UnicodeConv::ws2s(m.nom							);
	j["omega"					]			=					m.omega							;
	j["pression_critique"		]			=					m.pression_critique				;
	j["symbole"					]			= UnicodeConv::ws2s(m.symbole						);
	j["temperature_critique"	]			=					m.temperature_critique			;
	j["temperature_ebullition"	]			=					m.temperature_ebullition		;
}

inline void from_json(const nlohmann::json & j, gaz & m)
{
	m.masse_moleculaire						=					j["masse_moleculaire"			];
	m.nom									= UnicodeConv::s2ws(j["nom"							]);
	m.omega									=					j["omega"						];
	m.pression_critique						=					j["pression_critique"			];
	m.symbole								= UnicodeConv::s2ws(j["symbole"						]);
	m.temperature_critique					=					j["temperature_critique"		];
	m.temperature_ebullition				=					j["temperature_ebullition"		];
}


inline void to_json(nlohmann::json &j, const experimentateur &m) 
{
	j["nom"]								= UnicodeConv::ws2s(m.nom			);
	j["surnom"]								= UnicodeConv::ws2s(m.surnom		);
}

inline void from_json(const nlohmann::json & j, experimentateur & m)
{
	m.nom									= UnicodeConv::s2ws(j["nom"			]);	
	m.surnom								= UnicodeConv::s2ws(j["surnom"		]);	
}


inline void to_json(nlohmann::json &j, const cellule &m) 
{
	j["number"		]						= UnicodeConv::ws2s(m.numero		);
	j["caloVolume"	]						=					m.volume_calo	;
	j["totalVolume"	]						=					m.volume_total	;
}

inline void from_json(const nlohmann::json & j, cellule & m)
{
	m.numero								= UnicodeConv::s2ws(j["number"		]);
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
