#pragma once

/****************
EXPERIMENT TYPES
****************/

#define EXPERIMENT_TYPE_UNDEF			0
#define EXPERIMENT_TYPE_AUTO			1
#define EXPERIMENT_TYPE_MANUAL			2
#define EXPERIMENT_TYPE_MODIFY			3
#define EXPERIMENT_TYPE_SAMPLE_VACUUM	4
#define EXPERIMENT_TYPE_BOTTLE_VACUUM	5
#define EXPERIMENT_TYPE_BOTTLE_CHANGE	6
#define EXPERIMENT_TYPE_CONTINUOUS		7

/****************
STAGES
****************/

#define STAGE_UNDEF						0

#define STAGE_AUTO_VERIFICATIONS		1
#define STAGE_AUTO_EQUILIBRATION		2
#define STAGE_AUTO_RECORD				3
#define STAGE_AUTO_ADSORPTION			4
#define	STAGE_AUTO_DESORPTION			5
#define STAGE_AUTO_VACUUM_SAMPLE		6
#define	STAGE_AUTO_END					7

#define STAGE_CONT_VERIFICATIONS		1
#define STAGE_CONT_FLOWRATE				2
#define STAGE_CONT_DEADVOL				3
#define STAGE_CONT_VACUUM_BEFOREEX		4
#define STAGE_CONT_EQUILIBRATION		5
#define STAGE_CONT_ADSORPTION			6
#define STAGE_CONT_END_EQUILIBRATION	7
#define	STAGE_CONT_END					8

#define STAGE_MANUAL					1

#define STAGE_VACUUM_SAMPLE				1

#define STAGE_VACUUM_BOTTLE				1
#define STAGE_VACUUM_END				2

/****************
STEP STATUS
****************/

#define STEP_STATUS_UNDEF			0

#define STEP_STATUS_START			1	 
#define	STEP_STATUS_END				2
#define STEP_STATUS_UNDEF_END		3

#define STEP_VERIFICATIONS_SECURITY				4
#define STEP_VERIFICATIONS_VALVES				5
#define	STEP_VERIFICATIONS_PRESSURE				6
#define STEP_VERIFICATIONS_TEMPERATURE			7

#define STEP_STATUS_INPROGRESS					10			// Larger since some will be incremented

/****************
SUBSTEP STATUS
****************/

#define SUBSTEP_STATUS_UNDEF			0

#define SUBSTEP_STATUS_START			1
#define SUBSTEP_STATUS_INPROGRESS		200		// Larger since some will be incremented
#define SUBSTEP_STATUS_INJECTION		300		// Larger since some will be incremented
#define SUBSTEP_STATUS_CHECK			400		// Larger since some will be incremented
#define SUBSTEP_STATUS_ADSORPTION		600		// Larger since some will be incremented
#define	SUBSTEP_STATUS_END				700		// Larger since some will be incremented
#define SUBSTEP_STATUS_REMOVAL			800		// Larger since some will be incremented
#define SUBSTEP_STATUS_DESORPTION		900		// Larger since some will be incremented

#define SUBSTEP_STATUS_TO_PRESSURE		500		// Larger since some will be incremented


// Stage steps
enum struct Stop
{
	Error,
	UnderVacuum,
	Step,
	Dose,
	EmergencyHP,
	EmergenyT,
	Command,
	Undefined,
	Normal,
	Cancel,
	Complete,
	InProgress
};

#define CHOICE_NONE			0
#define CHOICE_YES			1
#define CHOICE_NO			2
#define CHOICE_WAIT			3
