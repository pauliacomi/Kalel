#pragma once

#include "Keithley.h"
#include "Mensor.h"
#include "FluxConverter.h"

class CPressure
{
public:
	CPressure();
	~CPressure();

private:
	int type_LowRange;
	int type_HighRange;
	int COM_LowRange;
	int COM_HighRange;
	int index_LowRange;
	int index_HighRange;
	int chanToRead_LowRange;
	int chanToRead_HighRange;
	Keithley * keith_LowRange;
	Keithley * keith_HighRange;
	Mensor * mens_LowRange;
	Mensor * mens_HighRange;
	FluxConverter converter;

public:

	// Pass in the references to double variables to get the pressure
	bool ReadLowRange(double* pressureLowrange);

	// Pass in the references to float variables to get the pressure
	bool ReadLowRange(double* pressureLowrange, char* message);

	// Pass in the references to double variables to get the pressure
	bool ReadHighRange(double* pressureHighRange);

	// Pass in the references to float variables to get the pressure
	bool ReadHighRange(double* pressureHighRange, char* message);
};

