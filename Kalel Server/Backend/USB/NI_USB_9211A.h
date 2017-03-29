//*****************************************************************************************************
//
//	This class is the implementation for the National Instruments Controller
//	used for reading the temperatures through thermocouples
//
//*****************************************************************************************************

#ifndef NI_USB_9211A_H
#define NI_USB_9211A_H
#pragma once

#include "NIDAQmx.h"
#include <string>


class NI_USB_9211A
{
public:
	NI_USB_9211A(void);
	NI_USB_9211A(int dev);
	~NI_USB_9211A(void);


private:
	//*************************
	// Parameters for reading
	//*************************
	int DevNI_USB_9211A;								// USB port used

	//double TC0 = -25000;								// Store the data from the thermocouples
	//double TC1 = -25000;								// Store the data from the thermocouples
	//double TC2 = -25000;								// Store the data from the thermocouples
	//double TC3 = -25000;								// Store the data from the thermocouples

	std::string errorKeep;								// C++ string of error

    // Channel parameters
	char        chan[25]		= { '\0' };
    float64     min				= 0.0f;					// minimum expected value to measure
    float64     max				= 100.0f;				// maximum expected value to measure
	int			analog_start	= 0;
	int			analog_end		= 3;

    // Timing parameters
    char        source[13]		= "OnboardClock";
	uInt64      samplesPerChan	= 1;
    float64     sampleRate		= 4.0f;

    // Data read parameters
	static const uInt32 bufferSize	= 10;
	int32       pointsToRead		= 1;
    float64     timeout				= 10.0f;

	// Read function
	bool ReadThermocouple(float64 data[bufferSize]);	// Reads all data from device

	//*************************
	// Public functions
	//*************************
public:
	int GetDevNI_USB_9211A();					// Get USB port
	void SetDevNI_USB_9211A(int dev);			// Set USB port
	
	bool ReadAllThermocouples(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3);

	// Error retrieval
	void GetError(std::string* err);

	bool LectureThermocouple_de_0_a_2(double* Valeur0,double* Valeur1,double* Valeur2);
	bool LectureThermocouple_de_1_a_3(double* Valeur1,double* Valeur2,double* Valeur3);
	bool LectureThermocouple_0_1_3(double* Valeur0,double* Valeur1,double* Valeur3);
	bool LectureThermocouple_0_2_3(double* Valeur0,double* Valeur2,double* Valeur3);
	
	bool LectureThermocouple_0_1(double* Valeur0,double* Valeur1);
	bool LectureThermocouple_0_2(double* Valeur0,double* Valeur2);
	bool LectureThermocouple_0_3(double* Valeur0,double* Valeur3);
	bool LectureThermocouple_1_2(double* Valeur1,double* Valeur2);
	bool LectureThermocouple_1_3(double* Valeur1,double* Valeur3);
	bool LectureThermocouple_2_3(double* Valeur2,double* Valeur3);
	
	bool LectureThermocouple_0(double* Valeur0);
	bool LectureThermocouple_1(double* Valeur1);
	bool LectureThermocouple_2(double* Valeur2);
	bool LectureThermocouple_3(double* Valeur3);

};


#endif




