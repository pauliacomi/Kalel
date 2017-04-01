#ifndef DEFINE_INSTRUMENT_H
#define DEFINE_INSTRUMENT_H
#pragma once

// Connection types
#define CONNECTION_SERIAL		1
#define CONNECTION_USB			2

#define CONNECTION_SERIAL_TXT	"serial"
#define CONNECTION_USB_TXT		"usb"

// type of instrument
#define INSTRUMENT_KEITHLEY			10
#define INSTRUMENT_MENSOR			11
#define INSTRUMENT_NI_USB_6008		12
#define INSTRUMENT_NI_USB_9211A		13
#define INSTRUMENT_NONE				14
#define INSTRUMENT_UNDEF			-20

#define INSTRUMENT_KEITHLEY_TXT			"keithley"
#define INSTRUMENT_MENSOR_TXT			"mensor"
#define INSTRUMENT_NI_USB_6008_TXT		"NI_USB_6008"
#define INSTRUMENT_NI_USB_9211A_TXT		"NI_USB_9211A"
#define INSTRUMENT_NONE_TXT				"none"
#define INSTRUMENT_UNDEF_TXT			"undefined"

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

// channels
#define INSTRUMENT_KEYTHLEY_V1			300
#define INSTRUMENT_KEYTHLEY_V2			301
#define MENSOR_VOIE						302
#define VOIE_INDEF						-30


#define COM_UNDEF				-1
#define READ_LP					0
#define READ_HP					1





#define NB_OF_INSTRUMENTS				3
#define INDEX_INDEF						NB_OF_INSTRUMENTS
#define INDEX_AUCUN						0
#define INDEX_KEITHLEY					1
#define INDEX_MENSOR					2

// Controlled instruments

#define INSTRUMENT_VALVE				1
#define INSTRUMENT_EV					2
#define INSTRUMENT_PUMP					3

// Some other stuff
#define PassageNul						0
#define PassageCOM1						1
#define PassageCOMs						2


#endif
