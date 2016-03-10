// 
//

#pragma once
#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

#include "NI_USB_9211A.h"


class CTemperature :
	public NI_USB_9211A
{
protected:

public:
	CTemperature(void);
	CTemperature(int dev);
	~CTemperature(void);

	void Temperature(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece);
	void Temperature(float* Temperature_Calo, float* Temperature_Cage, float* Temperature_Piece);
	double TemperatureCalo();
	double TemperatureCage();
	double TemperaturePiece();
	int PortUSBTemperature();
};

#endif