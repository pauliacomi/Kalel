#pragma once

// Instruments
#define NONE						0
#define INSTRUMENT_UNDEF			100
#define INSTRUMENT_KEITHLEY			1
#define INSTRUMENT_MENSOR			2
#define INSTRUMENT_NI_USB_9211A		3
#define INSTRUMENT_NI_USB_6008		4

// Readers 
#define READER_PRESSURE				16
#define READER_TEMPERATURE			32
#define READER_CALO					48

// Reader identifiers 
#define PRESSURE_LP					(READER_PRESSURE + 1   )
#define PRESSURE_HP					(READER_PRESSURE + 2   )
#define TEMPERATURE_CALO			(READER_TEMPERATURE + 1)
#define TEMPERATURE_CAGE			(READER_TEMPERATURE + 2)
#define TEMPERATURE_ROOM			(READER_TEMPERATURE + 3)
#define CALO						(READER_CALO + 1	   )

// Controllers
#define CONTROLLER_VALVE			16
#define CONTROLLER_EV				32
#define CONTROLLER_PUMP				48

#define VALVE_1		   1
#define VALVE_2		   2
#define VALVE_3		   3
#define VALVE_4		   4
#define VALVE_5		   5
#define VALVE_6		   6
#define VALVE_7		   7
#define VALVE_8		   8
#define EV_1		   1
#define EV_2		   2
#define PUMP		   1

// Controller identifiers

#define ID_VALVE_1						(CONTROLLER_VALVE + VALVE_1)
#define ID_VALVE_2						(CONTROLLER_VALVE + VALVE_2)
#define ID_VALVE_3						(CONTROLLER_VALVE + VALVE_3)
#define ID_VALVE_4						(CONTROLLER_VALVE + VALVE_4)
#define ID_VALVE_5						(CONTROLLER_VALVE + VALVE_5)
#define ID_VALVE_6						(CONTROLLER_VALVE + VALVE_6)
#define ID_VALVE_7						(CONTROLLER_VALVE + VALVE_7)
#define ID_VALVE_8						(CONTROLLER_VALVE + VALVE_8)

#define ID_EV_1							(CONTROLLER_EV + EV_1)
#define ID_EV_2							(CONTROLLER_EV + EV_2)

#define ID_PUMP							(CONTROLLER_PUMP + PUMP)
