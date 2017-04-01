#include "Instruments.h"

#include "../MessageHandler.h"

#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/Resources/DefineText.h"

#include <algorithm> 

ReadingInstruments::ReadingInstruments(MessageHandler & messageHandler, MachineSettings & m)
	: mh{ messageHandler }
{
	// This loop goes through all the instruments in the Parameters file
	// Depending on the type of instruments it will output parameters for
	//
	// * Calorimeter Apparatus
	// * Low Range Pressure Apparatus
	// * High Range Pressure Apparatus
	//
	// The parameters for each are 
	//
	// * Type:			if using a keithley or a mensor
	// * COM port		the COM port that the instrument is connected to
	// * Channel:		the channel that has to be read (only makes sense for a keithley)
	//
	
	Reset(m);
}


ReadingInstruments::~ReadingInstruments()
{
}



//
// initiation functions
//

void ReadingInstruments::Reset(MachineSettings & m)
{
	for (auto i = m.readers.begin(); i != m.readers.end(); ++i)
	{
		// Check if this is the first time that the instrument has been seen
		if (!m.instruments[i->second.instrument].initialised)
		{
			Init(i->first, i->second, i->second.instrument, m.instruments[i->second.instrument]);
		}
		else
		{
			// Check if the same instrument as was before
			if (i->second.instrument != readers[i->first].instrument)
			{
				// If not
				// TODO: only if not referenced 
				// Delete the instrument 
				DeleteInstrument(i->second.instrument, m.instruments[i->second.instrument]);
				// Create the new instrument
				Init(i->first, i->second, i->second.instrument, m.instruments[i->second.instrument]);
			}
			else
			{
				// If yes
				// Check that the port of the instrument is the same
				if (m.instruments[i->second.instrument].port != instruments[i->second.instrument].port)
				{
					ChangePort(i->second.instrument, instruments[i->second.instrument]);
				}
			}
		}
	}

	instruments = m.instruments;
	readers = m.readers;
	controllers = m.controllers;
}


void ReadingInstruments::Init(int readernumber, Reader & r, int instrumentnumber, Instrument & i)
{
	switch (i.name)
	{
	case INSTRUMENT_KEITHLEY:
		try {
			// Create instrument
			keithleys.emplace(std::make_pair(instrumentnumber, i.port));
			i.initialised = true;

			// Save read function
			std::function<bool(double*)> read;
			switch (r.channel)
			{
			case 1:
				read = std::bind(&Keithley::ReadChannel1, &keithleys[instrumentnumber], std::placeholders::_1);
				break;
			case 2:
				read = std::bind(&Keithley::ReadChannel2, &keithleys[instrumentnumber], std::placeholders::_1);
				break;
			default:
				throw;
				break;
			}
			readerfunctions.insert(std::make_pair(readernumber, read));
		}
		catch (const std::exception&) {

		}
		break;

	case INSTRUMENT_MENSOR:
		try {
			// Create instrument
			mensors.emplace(std::make_pair(instrumentnumber ,i.port));
			i.initialised = true;

			// Save read function
			auto read = std::bind(&Mensor::ReadMensor, &mensors[instrumentnumber], std::placeholders::_1);
			readerfunctions.insert(std::make_pair(readernumber, read));
		}
		catch (const std::exception&) {

		}
		break;

		// TODO: introduce this back
	case INSTRUMENT_NI_USB_9211A:
		try {
			// Create instrument
			NI_USB_9211As.emplace(std::make_pair(instrumentnumber, i.port));
			i.initialised = true;

			// Save read function
			std::function<bool(double*)> read;
			switch (r.channel)
			{
			case 1:
				read = std::bind(&Keithley::ReadChannel1, &keithleys[instrumentnumber], std::placeholders::_1);
				break;
			case 2:
				read = std::bind(&Keithley::ReadChannel2, &keithleys[instrumentnumber], std::placeholders::_1);
				break;
			default:
				throw;
				break;
			}
			readerfunctions.insert(std::make_pair(readernumber, read));
		}
		catch (const std::exception&) {
			std::string errorInit;
			messageHandler.DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
		}
		break;

	case INSTRUMENT_NONE:
	case INSTRUMENT_UNDEF:
		break;
	default:
		// Should never be reached
		throw;
		break;
	}
}

void ReadingInstruments::ChangePort(int instrumentnumber, Instrument & i)
{
	switch (i.name)
	{
	case INSTRUMENT_KEITHLEY:
		try {
			keithleys[instrumentnumber].ChangeCOM(i.port);
		}
		catch (const std::exception&) {

		}
		break;

	case INSTRUMENT_MENSOR:
		try {
			mensors[instrumentnumber].ChangeCOM(i.port);
		}
		catch (const std::exception&) {

		}
		break;

		// TODO: introduce this back
	case INSTRUMENT_NI_USB_9211A:
		try {
			NI_USB_9211As[instrumentnumber].SetDevNI_USB_9211A(i.port);
		}
		catch (const std::exception&) {
		}
		break;

	case INSTRUMENT_NONE:
	case INSTRUMENT_UNDEF:
		break;
	default:
		// Should never be reached
		throw;
		break;
	}
}

void ReadingInstruments::DeleteInstrument(int instrumentnumber, Instrument & i)
{
	switch (i.name)
	{
	case INSTRUMENT_KEITHLEY:
		try {
			keithleys.erase(instrumentnumber);
		}
		catch (const std::exception&) {

		}
		break;

	case INSTRUMENT_MENSOR:
		try {
			mensors.erase(instrumentnumber);
		}
		catch (const std::exception&) {

		}
		break;

		// TODO: introduce this back
	case INSTRUMENT_NI_USB_9211A:
		try {
			NI_USB_9211As.erase(instrumentnumber);
		}
		catch (const std::exception&) {
		}
		break;

	case INSTRUMENT_NONE:
	case INSTRUMENT_UNDEF:
		break;
	default:
		// Should never be reached
		throw;
		break;
	}
}


//
// read function
//
std::function<bool(double *)> ReadingInstruments::Bind(Reader & r) {

	bool success = false;

	for (auto i = readerfunctions.begin(); i != readerfunctions.end(); ++i)
	{
		if (i->first.type == r.type && i->first.identifier == r.identifier)
		{
			 return = i->second;
		}
	}
	return success;
}

// 
// error function
//

bool ReadingInstruments::GetError(Reader & r, double * value)
{
	return false;
}
