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
	int portUSB;											// USB port used

    // Channel parameters
    const float64 t_min				= -200.0f;				// minimum expected value to measure
    const float64 t_max				= 100.0f;				// maximum expected value to measure
	const int chan_min				= 0;					// minimum channel number
	const int chan_max				= 7;					// maximum channel number

    // Data read parameters
	const uInt64 samplesPerChan		= 1;					// datapoints to read per channel
	static const uInt32 maxBufSize	= 8;					// max size of read buffer
    const float64 timeout			= 2.0f;					// seconds until timeout

	// Read function
	bool ReadAnalog(char * chan, float64 * data);			// Generic read function

	//*************************
	// Public functions
	//*************************
public:

	void SetComPort(int dev) override;		// Set USB port
	int GetComPort() override;				// Get USB port
	
	double Read(unsigned int channel) override;
	bool ReadAllChannels(double* readArray);
};
