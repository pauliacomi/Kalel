#pragma once

#include "Wrapper Classes/InstrumentInterface.h"

#include "../../Kalel Shared/Com Classes/MachineSettings.h"

#include <map>

class Keithley;
class Mensor;
class NI_USB_9211A;
class NI_USB_6008;

class Instruments
{
public:
	Instruments(const MachineSettings & m);
	~Instruments();

private:


	// For Keithley
	std::map<unsigned int, Keithley> keithleys;
	std::map<unsigned int, Mensor> mensors;
	std::map<unsigned int, NI_USB_9211A> NI_USB_9211As;
	std::map<unsigned int, NI_USB_6008> NI_USB_6008s;

	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine


public:
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available

	bool Reset(const MachineSettings & m);

	double MeasureReader(unsigned int identifier);

	bool MeasureController(unsigned int identifier);
	bool ActuateController(unsigned int identifier, bool state);

private:

	void BindReader(Reader & r);
	void BindController(Controller & c);
	void InitInstrument(Instrument & i);
	void ChangePort(Instrument & i);
	void DeleteInstrument(Instrument & i);
};
