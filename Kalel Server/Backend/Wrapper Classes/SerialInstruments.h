#pragma once

#include "Keithley.h"
#include "Mensor.h"
#include "FluxConverter.h"

class SerialInstruments
{
public:
	SerialInstruments();
	~SerialInstruments();

protected:
	int calorimeter;
	int pressureLowRange;
	int pressureHighRange;

	int calorimeterCOM;
	int pressureLowRangeCOM;
	int pressureHighRangeCOM;

	int calorimeterChannel;
	int pressureLowRangeChannel;
	int pressureHighRangeChannel;

	bool keithleyInitiated;

	// There is only one Keithley
	Keithley * keithley;

	// And up to two mensors
	Mensor * mens_LowRange;
	Mensor * mens_HighRange;

	// Converter
	FluxConverter converter;

public:
	bool Init(std::string * errorInit);

	bool InitiateCalorimeter();
	bool ReadCalorimeter(double * calorimeter);

	bool InitiatePressureLowRange();
	bool ReadPressureLowRange(double * pressure);

	bool InitiatePressureHighRange();
	bool ReadPressureHighRange(double * pressure);
	
	void GetErrorCalrimeter(std::string * err);
	void GetErrorLowRange(std::string * err);
	void GetErrorHighRange(std::string * err);
};

