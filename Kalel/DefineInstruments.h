#pragma once
#ifndef DEFINE_INSTRUMENT_H
#define DEFINE_INSTRUMENT_H

// type of instrument
#define INSTRUMENT_KEITHLEY		10
#define INSTRUMENT_MENSOR		11
#define INSTRUMENT_NONE			12

#define INSTRUMENT_UNDEF		-20
#define INSTRUMENT_INEXIST		-21

#define READ_LP					0
#define READ_HP					1

// functions
#define CALO_V1_BP_V2_KEITHLEY			100
#define CALO_V1_HP_V2_KEITHLEY			108
#define CALO_V1_KEITHLEY				102
#define INSTRUMENT_KEYTHLEY_HP_V2		104
#define INSTRUMENT_KEYTHLEY_LP_V2		103
#define INSTRUMENT_MENSOR_HP			200
#define INSTRUMENT_MENSOR_LP			201
#define FUNCTION_NONE					0

#define FUNCTION_UNDEF					-22
#define FUNCTION_INEXIST				-23


#define INSTRUMENT_KEYTHLEY_V1			300
#define INSTRUMENT_KEYTHLEY_V2			301
#define MENSOR_VOIE						302
#define VOIE_INDEF						-30

#define NB_OF_INSTRUMENTS				3
#define INDEX_INDEF						NB_OF_INSTRUMENTS
#define INDEX_AUCUN						0
#define INDEX_KEITHLEY					1
#define INDEX_MENSOR					2

// Controlled instruments

#define INSTRUMENT_VALVE	1001
#define INSTRUMENT_EV		1002
#define INSTRUMENT_PUMP		1003

// Some other stuff
#define PassageNul						0
#define PassageCOM1						1
#define PassageCOMs						2

#endif
