#pragma once

#include "../../Parameters/FluxConverter.h"

#include <string>

class Keithley;
class Mensor;
class MessageHandler;

class SerialInstruments
{
public:
	SerialInstruments(MessageHandler & messageHandler);
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

	bool keithleyInitiated	= false;

	// There is only one Keithley
	Keithley * keithley		= nullptr;

	// And up to two mensors
	Mensor * mens_LowRange	= nullptr;
	Mensor * mens_HighRange	= nullptr;

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

private:
	MessageHandler & messageHandler;
};

