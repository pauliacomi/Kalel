#ifndef DEFINE_INSTRUMENT_H
#define DEFINE_INSTRUMENT_H
#pragma once

#define NONE						0

// type of instrument
#define INSTRUMENT_KEITHLEY			1
#define INSTRUMENT_MENSOR			2
#define INSTRUMENT_NI_USB_9211A		3
#define INSTRUMENT_NI_USB_6008		4
#define INSTRUMENT_UNDEF			10

// Readers 
#define READER_PRESSURE				1
#define READER_TEMPERATURE			2
#define READER_CALO					3

// identifiers 
#define PRESSURE_LP					1
#define PRESSURE_HP					2
#define TEMPERATURE_CALO			1
#define TEMPERATURE_CAGE			2
#define TEMPERATURE_ROOM			3
#define CALO						1

// Controllers
#define CONTROLLER_VALVES			1
#define ID_CONTROLLER_VALVES		1

// Controlled instruments
#define INSTRUMENT_VALVE			1
#define INSTRUMENT_EV				2
#define INSTRUMENT_PUMP				3

#endif
