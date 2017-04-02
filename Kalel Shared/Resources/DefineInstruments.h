#ifndef DEFINE_INSTRUMENT_H
#define DEFINE_INSTRUMENT_H
#pragma once

// type of instrument
#define INSTRUMENT_KEITHLEY			10
#define INSTRUMENT_MENSOR			11
#define INSTRUMENT_NI_USB_6008		12
#define INSTRUMENT_NI_USB_9211A		13
#define INSTRUMENT_NONE				14
#define INSTRUMENT_UNDEF			0

// channels
#define INSTRUMENT_KEYTHLEY_V1			300
#define INSTRUMENT_KEYTHLEY_V2			301
#define MENSOR_VOIE						302
#define VOIE_INDEF						-30


// Readers 
#define READER_PRESSURE				0
#define READER_TEMPERATURE			1
#define READER_CALO					2

// identifiers 
#define PRESSURE_LP					0
#define PRESSURE_HP					1
#define CALO						1
#define TEMPERATURE_CALO			0
#define TEMPERATURE_CAGE			1
#define TEMPERATURE_ROOM			1

// Controllers
#define CONTROLLER_VALVES			0

// Controlled instruments
#define INSTRUMENT_VALVE				1
#define INSTRUMENT_EV					2
#define INSTRUMENT_PUMP					3




#define NB_OF_INSTRUMENTS				3
#define INDEX_INDEF						NB_OF_INSTRUMENTS
#define INDEX_AUCUN						0
#define INDEX_KEITHLEY					1
#define INDEX_MENSOR					2


// Some other stuff
#define PassageNul						0
#define PassageCOM1						1
#define PassageCOMs						2

#define COM_UNDEF				-1

#endif
