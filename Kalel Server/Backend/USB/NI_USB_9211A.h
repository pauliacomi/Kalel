//*****************************************************************************************************
//
//	This class is the implementation for the National Instruments Controller
//	used for reading the temperatures through thermocouples
//
//*****************************************************************************************************

#pragma once

#include "NIDAQmx.h"
#include "../Wrapper Classes/InstrumentInterface.h"

class NI_USB_9211A : InstrumentInterface
{
public:
	NI_USB_9211A(int dev);
	~NI_USB_9211A(void);

private:
	//*************************
	// Parameters for reading
	//*************************
	int portUSB;										// USB port used

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
	bool ReadAll(float64 data[bufferSize]);				// Reads all data from device

	//*************************
	// Public functions
	//*************************
public:

	void SetComPort(int dev) override;		// Set USB port
	int GetComPort() override;				// Get USB port
	
	bool ReadAllChannels(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3);

	double Read(unsigned int channel) override;
};
