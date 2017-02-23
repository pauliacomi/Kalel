#pragma once
#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_


#include "../USB/NI_USB_9211A.h"
#include "../../Parameters/Parametres.h"

class TemperatureInstruments :
	public NI_USB_9211A
{
protected:

public:
	TemperatureInstruments(void);
	~TemperatureInstruments(void);

	// Pass in the references to double variables to get the three temperatures
	bool Read(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece);

	// Pass in the references to float variables to get the three temperatures
	bool Read(float* Temperature_Calo, float* Temperature_Cage, float* Temperature_Piece);
	

	// Get calorimeter temperature
	double ReadCalo();

	// Get cage temperature
	double ReadCage();

	// Get room temperature
	double ReadPiece();

	// Get the usb port for temperature readings
	int PortUSB();
};

#endif