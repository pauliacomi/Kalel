//*****************************************************************************************************
//
//	This class is the implementation for the National Instruments Controller
//	used for controlling the valves and pump
//
//*****************************************************************************************************

#ifndef NI_USB_6008_H_
#define NI_USB_6008_H_
#pragma once

#include "NIDAQmx.h"
#include <string>

class NI_USB_6008
{
public:
	NI_USB_6008(int dev);
	~NI_USB_6008(void);
	
private:

	//*************************
	// Parameters for writing
	//*************************
	int DevNI_USB_6008;								// USB port
	uInt8 etatPort0[8] = { 0 };						// Array for keeping port 0 states
	uInt8 etatPort1[8] = { 0 };						// Array for keeping port 1 states

	std::string errorKeep;							// C++ string of error

	// Read/Write parameters
	static const uInt32 bufferSize	= 8;
	int32		samplesPerChannel	= 1;
	bool32		autostart			= true;
	float64     timeout				= 10.0f;

	// Read/write functions
	bool ReadPort0();
	bool ReadPort1();
	bool WritePort0();
	bool WritePort1();
	bool ReadDigital(char chan[], uInt8 w_data[]);
	bool WriteDigital(char chan[], uInt8 w_data[]);

public:
	
	int GetDevNI_USB_6008();						// Get USB port
	void SetDevNI_USB_6008(int dev);				// Set USB port

	// Functions to open / close a particular bit from a port or all bits
	bool SetPortCustom(int port, unsigned int customarray[8]);

	bool OuvrirPort0(int num);
	bool FermerPort0(int num);
	bool OuvrirPort1(int num);
	bool FermerPort1(int num);

	bool FermerPort0Tous();
	bool OuvrirPort0Tous();
	bool FermerPort1Tous();
	bool OuvrirPort1Tous();

	// Check for open/closed functions

	bool EstOuvertPort0(int num);
	bool EstFermePort0(int num);
	bool EstOuvertPort1(int num);
	bool EstFermePort1(int num);

	// Error retrieval

	void GetError(std::string* err);
};

#endif