#pragma once
#ifndef DEFINE_H
#define DEFINE_H

#include "Define_Stages.h"
#include "Define_Experiment_Type.h"
#include "Define_Instrument.h"

#define GREY_OUT FALSE
#define UN_GREY_OUT TRUE

#define T_BETWEEN_RECORD		4		//seconds
#define T_BETWEEN_MEASURE		500		//miliseconds
#define TEMP_SECURITY 1					//degreee celsius

#define ERROR_MESURE -300000

#define PassageNul  0
#define PassageCOM1 1
#define PassageCOMs 2

// Stage steps
#define INACTIVE			100
#define STOP_IMMEDIAT		101
#define STOP_UNDERVACCUM	102
#define STOP_STEP			103
#define STOP_DOSE			104
#define PAUSE				105
#define STOP				106
#define STOP_EMERGENCY_HP	107
#define STOP_EMERGENCY_TCH	108
#define STOP_EMERGENCY_TCB	109
#define STOP_COMMAND		110
#define ACTIVE				111

#endif
