#pragma once

#ifndef DEFINE_STAGES_H
#define DEFINE_STAGES_H

// Used by Steps.rc



//#define STAGE_EQ_LINE				0
//#define STAGE_CONT_ADSORPTON		1


#define STAGE_UNDEF						100
#define STAGE_TEMP						101
#define	STAGE_UNDER_VACUUM				103

#define STAGE_EQUILIBRATION				104
#define STAGE_ADSORPTION				105
#define	STAGE_DESORPTION				106
#define STAGE_CONTINUOUS_ADSORPTION		107
#define STAGE_VERIFICATIONS				108
#define	STAGE_END_AUTOMATIC				109
#define STAGE_MANUAL					110


#define STEP_VERIFICATIONS_SECURITY				200
#define STEP_VERIFICATIONS_VALVES				201
#define	STEP_VERIFICATIONS_PRESSURE				202
#define STEP_VERIFICATIONS_TEMPERATURE			203
#define STEP_VERIFICATIONS_COMPLETE				204
#define STEP_VERIFICATIONS_UNDEF				205


#define STEP_STATUS_START			300
#define STEP_STATUS_INPROGRESS		301
#define	STEP_STATUS_END				302
#define STEP_STATUS_UNDEF			304

#define SUBSTEP_STATUS_START			400
#define SUBSTEP_STATUS_INJECTION		401
#define SUBSTEP_STATUS_INJECTION_CHK	402
#define SUBSTEP_STATUS_INJECTION_ABORT	403
#define SUBSTEP_STATUS_ADSORPTION		404
#define	SUBSTEP_STATUS_END				405


// Next default values for new objects
// 
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS
#define _APS_NEXT_RESOURCE_VALUE        101
#define _APS_NEXT_COMMAND_VALUE         40001
#define _APS_NEXT_CONTROL_VALUE         1001
#define _APS_NEXT_SYMED_VALUE           101
#endif
#endif

#endif
