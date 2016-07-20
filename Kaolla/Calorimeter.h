#pragma once

#include "Keithley.h"
#include "FluxConverter.h"
#include "Instruments.h"

class CCalorimeter
	:public Instruments
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

	void GetError(std::string * err);

	// Pass in the references to float variables to get the three temperatures
	bool Read(double* calorimeter, char* message);
};

