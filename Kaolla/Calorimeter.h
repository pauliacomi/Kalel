#pragma once

#include "Keithley.h"
#include "FluxConverter.h"

class CCalorimeter
{
public:
	CCalorimeter();
	~CCalorimeter();

private:
	int COM;
	int index;
	int chanToRead;
	Keithley * calorimeterDevice;
	FluxConverter converter;

public:

	// Pass in the references to double variables to get the three temperatures
	bool Read(double* calorimeter);

	// Pass in the references to float variables to get the three temperatures
	bool Read(double* calorimeter, char* message);
};

