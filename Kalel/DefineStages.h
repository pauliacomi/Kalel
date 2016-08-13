#pragma once

#ifndef DEFINE_STAGES_H
#define DEFINE_STAGES_H

// Used by Steps.rc


//#define STAGE_EQ_LINE				0
//#define STAGE_CONT_ADSORPTON		1


#define EXPERIMENT_TYPE_UNDEF			0
#define EXPERIMENT_TYPE_AUTO			1
#define EXPERIMENT_TYPE_MANUAL			2
#define EXPERIMENT_TYPE_MODIFY			3
#define EXPERIMENT_TYPE_SAMPLE_VACUUM	4
#define EXPERIMENT_TYPE_BOTTLE_VACUUM	5

#define STAGE_UNDEF						20101
#define STAGE_TEMP						20102
#define	STAGE_UNDER_VACUUM				20103
										
#define STAGE_EQUILIBRATION				20104
#define STAGE_ADSORPTION				20105
#define	STAGE_DESORPTION				20106
#define STAGE_CONTINUOUS_ADSORPTION		20107
#define STAGE_VERIFICATIONS				20108
#define	STAGE_END_AUTOMATIC				20109
#define STAGE_MANUAL					20110


#define STEP_VERIFICATIONS_SECURITY				20200
#define STEP_VERIFICATIONS_VALVES				20201
#define	STEP_VERIFICATIONS_PRESSURE				20202
#define STEP_VERIFICATIONS_TEMPERATURE			20203
#define STEP_VERIFICATIONS_COMPLETE				20204
#define STEP_VERIFICATIONS_UNDEF				20205


#define STEP_STATUS_START			20300
#define STEP_STATUS_INPROGRESS		20301
#define	STEP_STATUS_END				20400
#define STEP_STATUS_UNDEF			20401
#define STEP_STATUS_SUBSTEP			20405

#define SUBSTEP_STATUS_START			20500
#define SUBSTEP_STATUS_INPROGRESS		20501
#define SUBSTEP_STATUS_INJECTION		20601
#define SUBSTEP_STATUS_CHECK			20602
#define SUBSTEP_STATUS_ABORT			20603
#define SUBSTEP_STATUS_ADSORPTION		20604
#define	SUBSTEP_STATUS_END				20605


// Stage steps
#define PAUSE				20705
#define INACTIVE			20700
#define ACTIVE				20711
#define STOP				20706
#define STOP_IMMEDIAT		20701
#define STOP_UNDERVACCUM	20702
#define STOP_STEP			20703
#define STOP_DOSE			20704
#define STOP_EMERGENCY_HP	20707
#define STOP_EMERGENCY_TCH	20708
#define STOP_EMERGENCY_TCB	20709
#define STOP_COMMAND		20710
							  
#define STOP_UNDEF			20711
#define STOP_NORMAL			20712
#define STOP_CANCEL			20713
#define STOP_COMPLETE		20714
#define STOP_INPROGRESS		20715

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