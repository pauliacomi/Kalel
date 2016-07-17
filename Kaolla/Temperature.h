#pragma once
#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers, useless, because automatically defined in MFC
#include <stdio.h>
#include <tchar.h>

#include "NI_USB_9211A.h"
#include "Parametres.h"


class CTemperature :
	public NI_USB_9211A
{
protected:

public:
	CTemperature(void);
	~CTemperature(void);

	// Pass in the references to double variables to get the three temperatures
	void Temperature(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece);

	// Pass in the references to float variables to get the three temperatures
	void Temperature(float* Temperature_Calo, float* Temperature_Cage, float* Temperature_Piece);
	

	// Get calorimeter temperature
	double TemperatureCalo();

	// Get cage temperature
	double TemperatureCage();

	// Get room temperature
	double TemperaturePiece();

	// Get the usb port for temperature readings
	int PortUSB();
};

#endif