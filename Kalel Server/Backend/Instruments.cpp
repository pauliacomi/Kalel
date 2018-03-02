#include "Instruments.h"

#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/Resources/DefineText.h"
#include "../../Kalel Shared/log.h"

#include <algorithm> 

Instruments::Instruments(MachineSettings & m)
{	
	Reset(m);
}


Instruments::~Instruments()
{
}



//
// initiation functions
//

void Instruments::Reset(MachineSettings & m)
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

	for (auto i = m.readers.begin(); i != m.readers.end(); ++i)
	{
		// Check if the reader function exists for the instrument referenced
		if (readerfunctions.find(i->first) == readerfunctions.end())
		{
			Init(i->first, i->second, i->second.instrument, m.instruments[i->second.instrument]);
		}
		else
		{
			// Check if the same instrument as was before
			if (i->second.instrument != readers[i->first].instrument)
			{
				// Delete the function
				readerfunctions.erase(i->first);
				// Create the new instrument and function
				Init(i->first, i->second, i->second.instrument, m.instruments[i->second.instrument]);
				// TODO: might need deletion of the old instrument
			}
			else
			{
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
}


void Instruments::Init(int readernumber, Reader & r, int instrumentnumber, Instrument & i)
{
	switch (i.type)
	{
	case INSTRUMENT_KEITHLEY:
	{
		// See if it is initialised
		if (!i.initialised)
		{
			try {
				// Create instrument
				keithleys.emplace(std::make_pair(instrumentnumber, i.port));
				i.initialised = true;
			}
			catch (const std::exception&) {

			}
		}

		// Save read function
		std::function<double(void)> read;
		switch (r.channel)
		{
		case 1:
			read = std::bind(&Keithley::ReadChannel1, &keithleys[instrumentnumber]);
			break;
		case 2:
			read = std::bind(&Keithley::ReadChannel2, &keithleys[instrumentnumber]);
			break;
		default:
			throw;
			break;
		}
		readerfunctions.insert(std::make_pair(readernumber, read));

		break;
	}

	case INSTRUMENT_MENSOR:
	{
		if (!i.initialised)
		{
			try	{
				// Create instrument
				mensors.emplace(std::make_pair(instrumentnumber, i.port));
				i.initialised = true;
			}
			catch (const std::exception&) {

			}
		}

		// Save read function
		auto read = std::bind(&Mensor::ReadMensor, &mensors[instrumentnumber]);
		readerfunctions.insert(std::make_pair(readernumber, read));

		break;
	}

	case INSTRUMENT_NI_USB_9211A:
	{
		try {
			// Create instrument
			NI_USB_9211As.emplace(std::make_pair(instrumentnumber, i.port));
			i.initialised = true;
		}
		catch (const std::exception&) {
			std::string errorInit;
			LOG(logERROR) << MESSAGE_INSTRUMENT_INIT_FAIL << errorInit;
		}

		// Save read functions
		std::function<double(void)> read;
		switch (r.channel)
		{
		case 1:
			read = std::bind(&NI_USB_9211A::LectureThermocouple_0, &NI_USB_9211As[instrumentnumber]);
			break;
		case 2:
			read = std::bind(&NI_USB_9211A::LectureThermocouple_1, &NI_USB_9211As[instrumentnumber]);
			break;
		case 3:
			read = std::bind(&NI_USB_9211A::LectureThermocouple_2, &NI_USB_9211As[instrumentnumber]);
			break;
		default:
			throw;
			break;
		}
		readerfunctions.insert(std::make_pair(readernumber, read));

		break;
	}

	case NONE:
	case INSTRUMENT_UNDEF:
		break;
	default:
		// Should never be reached
		throw;
		break;
	}
}

void Instruments::ChangePort(int instrumentnumber, Instrument & i)
{
	switch (i.type)
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

	case INSTRUMENT_NI_USB_9211A:
		try {
			NI_USB_9211As[instrumentnumber].SetPortUSB(i.port);
		}
		catch (const std::exception&) {
		}
		break;

	case NONE:
	case INSTRUMENT_UNDEF:
		break;
	default:
		// Should never be reached
		throw;
		break;
	}
}

void Instruments::DeleteInstrument(int instrumentnumber, Instrument & i)
{
	switch (i.type)
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

	case INSTRUMENT_NI_USB_9211A:
		try {
			NI_USB_9211As.erase(instrumentnumber);
		}
		catch (const std::exception&) {
		}
		break;

	case NONE:
	case INSTRUMENT_UNDEF:
		break;
	default:
		// Should never be reached
		throw;
		break;
	}
}
