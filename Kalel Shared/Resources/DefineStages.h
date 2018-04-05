#pragma once

#define EXPERIMENT_TYPE_UNDEF			0
#define EXPERIMENT_TYPE_AUTO			1
#define EXPERIMENT_TYPE_MANUAL			2
#define EXPERIMENT_TYPE_MODIFY			3
#define EXPERIMENT_TYPE_SAMPLE_VACUUM	4
#define EXPERIMENT_TYPE_BOTTLE_VACUUM	5
#define EXPERIMENT_TYPE_BOTTLE_CHANGE	6
#define EXPERIMENT_TYPE_CONTINUOUS		7

#define STAGE_UNDEF						0
#define STAGE_TEMP						1										
#define STAGE_EQUILIBRATION				2
#define STAGE_ADSORPTION				3
#define	STAGE_DESORPTION				4
#define STAGE_CONTINUOUS_ADSORPTION		5
#define STAGE_VERIFICATIONS				6
#define	STAGE_END_AUTOMATIC				7
#define STAGE_MANUAL					8
#define STAGE_VACUUM_SAMPLE				9
#define STAGE_VACUUM_BOTTLE				10


#define STEP_VERIFICATIONS_SECURITY				11
#define STEP_VERIFICATIONS_VALVES				12
#define	STEP_VERIFICATIONS_PRESSURE				13
#define STEP_VERIFICATIONS_TEMPERATURE			14
#define STEP_VERIFICATIONS_COMPLETE				15
#define STEP_VERIFICATIONS_UNDEF				16


#define STEP_STATUS_UNDEF			0
#define STEP_STATUS_START			10			// Larger since some will be incremented
#define STEP_STATUS_INPROGRESS		20			// Larger since some will be incremented
#define	STEP_STATUS_END				30			// Larger since some will be incremented
#define STEP_STATUS_SUBSTEP			40			// Larger since some will be incremented

#define SUBSTEP_STATUS_START			100		// Larger since some will be incremented
#define SUBSTEP_STATUS_INPROGRESS		200		// Larger since some will be incremented
#define SUBSTEP_STATUS_INJECTION		300		// Larger since some will be incremented
#define SUBSTEP_STATUS_CHECK			400		// Larger since some will be incremented
#define SUBSTEP_STATUS_ABORT			500		// Larger since some will be incremented
#define SUBSTEP_STATUS_ADSORPTION		600		// Larger since some will be incremented
#define	SUBSTEP_STATUS_END				700		// Larger since some will be incremented
#define SUBSTEP_STATUS_REMOVAL			800		// Larger since some will be incremented
#define SUBSTEP_STATUS_DESORPTION		900		// Larger since some will be incremented


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
