#pragma once

#include "../Parameters/FluxConverter.h"

#include "RS232/Keithley.h"
#include "RS232/Mensor.h"
#include "USB/NI_USB_6008.h"
#include "USB/NI_USB_9211A.h"

#include "../../Kalel Shared/Com Classes/MachineSettings.h"

#include <string>
#include <map>

class MessageHandler;

class ReadingInstruments
{
public:
	ReadingInstruments(MessageHandler & messageHandler, MachineSettings & m);
	~ReadingInstruments();

private:
	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available

	std::map<unsigned int, std::function<bool(double *)>> readerfunctions;

	// For Keithley
	std::map<unsigned int, Keithley> keithleys;
	// For Mensors
	std::map<unsigned int, Mensor> mensors;
	// For NI_USB_6008
	std::map<unsigned int, NI_USB_6008> NI_USB_6008s;
	// For NI_USB_9211A
	std::map<unsigned int, NI_USB_9211A> NI_USB_9211As;

public:
	void Reset(MachineSettings & m);
	std::function<bool(double *)> Bind(Reader & r);
	bool GetError(Reader & r, double * value);

private:
	MessageHandler & mh;
	void Init(int readernumber, Reader & r, int instrumentnumber, Instrument & i);
	void ChangePort(int instrumentnumber, Instrument & i);
	void DeleteInstrument(int instrumentnumber, Instrument & i);
};

