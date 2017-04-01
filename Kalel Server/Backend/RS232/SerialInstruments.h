#pragma once

#include "../../Parameters/FluxConverter.h"

#include "../RS232/Keithley.h"
#include "../RS232/Mensor.h"
#include "../USB/NI_USB_6008.h"
#include "../USB/NI_USB_9211A.h"


#include <string>

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

	std::vector<Instrument> instruments;

	// For Keithley
	std::vector<Keithley> keithleys;
	// For Mensors
	std::vector<Mensor> mensors;
	// For NI_USB_6008
	std::vector<NI_USB_6008> NI_USB_6008s;
	// For NI_USB_9211A
	std::vector<NI_USB_9211A> NI_USB_9211As;

	// Converter
	FluxConverter converter;
	double sensitivity_calo  = 0.0f;
	double sensitivity_lp	= 0.0f;
	double sensitivity_hp	= 0.0f;

public:
	void Reset(MachineSettings & m);
	std::function<void(double* p)> Link(Reader & r);

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

