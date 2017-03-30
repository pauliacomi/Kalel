#pragma once

#include "../../Parameters/FluxConverter.h"
#include <string>

class Keithley;
class Mensor;
class MessageHandler;
class MachineSettings;

class SerialInstruments
{
public:
	SerialInstruments(MessageHandler & messageHandler, MachineSettings & m);
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
	double sensitivity_calo  = 0.0f;
	double sensitivity_lp	= 0.0f;
	double sensitivity_hp	= 0.0f;

public:
	bool SetSensitivity(double sensitivity_calo, double sensitivity_lp, double sensitivity_hp);

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

