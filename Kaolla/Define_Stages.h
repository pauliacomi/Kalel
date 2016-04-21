#pragma once
#ifndef DEFINE_STAGES_H
#define DEFINE_STAGES_H

// les �tapes
#define STAGE_UNDEF				100
#define STAGE_TEMP				101
#define STAGE_EQ_LINE			0
#define STAGE_CONT_ADSORPTON	1
#define	STAGE_SMALL_DOSES		2
#define STAGE_MEDIUM_DOSES		3
#define STAGE_BIG_DOSES			4
#define	STAGE_DESORPTION		5
#define	STAGE_UNDER_VACUUM		6


#define STEP_EQUILIBRATION				0
#define STEP_ADSORPTION					1
#define	STEP_DESORPTION					2
#define STEP_CONTINUOUS_ADSORPTION		3
#define STEP_VERIFICATIONS				4
#define	STEP_END_AUTOMATIC				6

#define STEP_VERIFICATIONS_SECURITY				0
#define STEP_VERIFICATIONS_VALVES				1
#define	STEP_VERIFICATIONS_PRESSURE				2
#define STEP_VERIFICATIONS_TEMPERATURE			3
#define STEP_VERIFICATIONS_COMPLETE				4


#define STEP_STATUS_START			0
#define STEP_STATUS_INPROGRESS		1
#define	STEP_STATUS_END				2
#endif