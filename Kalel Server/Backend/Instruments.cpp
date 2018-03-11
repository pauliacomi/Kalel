#include "Instruments.h"

#include "../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../Kalel Shared/Resources/DefineText.h"
#include "../../Kalel Shared/log.h"

#include <algorithm> 

Instruments::Instruments(const MachineSettings & m)
{	
	Reset(m);
}


Instruments::~Instruments()
{
}



//
// initiation functions
//

void Instruments::Reset(const MachineSettings & m)
{
	// This loop goes through all the instruments in the Parameters file
	// Depending on the type of instruments it will output parameters for
	//
	// * Calorimeter
	// * Pressure
	// * Temperature
	// * Control
	//
	// The parameters for each are 
	//
	// * Type:			if using a keithley or a mensor
	// * COM port		the COM port that the instrument is connected to
	// * Channel:		the channel that has to be read (only makes sense for a keithley)
	//

	//**********************************
	// Checks
	//**********************************
	for (auto i = m.readers.begin(); i != m.readers.end(); i++)					// For each incoming reader
	{
		if (m.instruments.find(i->second.instrument) == m.instruments.end())
		{
			LOG(logERROR) << "Reader " << i->first << " does not have a corresponding instrument";
			return;
		}
	}

	for (auto i = m.controllers.begin(); i != m.controllers.end(); i++)					// For each incoming controller
	{
		if (m.instruments.find(i->second.instrument) == m.instruments.end())
		{
			LOG(logERROR) << "Controller " << i->first << " does not have a corresponding instrument";
			return;
		}
	}
	// TODO Add a check for duplicate ports?

	//**********************************
	// Instruments
	//**********************************

	for (auto i = m.instruments.begin(); i != m.instruments.end(); i++)			// For each incoming instrument
	{
		bool existing = false;
		for (auto t = instruments.begin(); t != instruments.end(); t++)			// Go through the existing instruments
		{
			if (i->second == t->second)											// If they are the same
			{
				if (i->first != t->first)										// but different ids
				{
					LOG(logERROR) << "Instrument id" << i->first << " already exists as id " << t->first;
					return;														// Then quit without doing anything
				}
				existing = true;												// Otherwise set the flag for already existing
				break;															// And go to the next incoming instrument
			}																	// Or continue through the array
			if (existing) break;
		}

		// We have now eliminated the possibility that the instrument is already existing, either in the 
		// correct id or through a false id
		// Now let's look if the ports are different or the instrument does not exist at all

		auto instrumentAtID = instruments.find(i->first);						// Get the instrument with the same id

		if (instrumentAtID != instruments.end()) {								// Check if it actually is a good id

			if (i->second.type == instrumentAtID->second.type)					// If it's the same instrument type
			{
				if (i->second.port != instrumentAtID->second.port)				// But port is not the same
				{
					instrumentAtID->second = i->second;							// We equate it to the new instrument
					ChangePort(instrumentAtID->second);							// And need to change the port
				}
				instrumentAtID->second.initialised = true;						// And say it's initialised
			}
			else
			{
				instrumentAtID->second = i->second;								// Otherwise,just equate it to the new instrument
				InitInstrument(instrumentAtID->second);							// And initialise it
			}
		}
		else																	// if it is a new instrument
		{
			instruments.emplace(std::make_pair(i->first, i->second));			// then store it
			InitInstrument(instruments[i->first]);								// and initialize it
		}
	}

	for (auto i = instruments.begin(); i != instruments.end(); i++)				// now for each existing instrument
	{
		if (m.instruments.find(i->first) == m.instruments.end())				// If it is not in the new settings
		{
			instruments.erase(i);												// Delete it
		}
	}

	//**********************************
	// Readers and controllers
	//**********************************
	readers.empty();															// empty existing readers

	for (auto i = m.readers.begin(); i != m.readers.end(); i++)					// now for each existing reader
	{
		unsigned int key = i->second.type + i->second.identifier;				// generate the key
		readers.emplace(key, i->second);										// construct the reader
		BindReader(readers[key]);												// bind the function
	}

	controllers.empty();

	for (auto i = m.controllers.begin(); i != m.controllers.end(); i++)			// now for each existing controller
	{
		unsigned int key = i->second.type + i->second.identifier;				// generate the key
		controllers.emplace(key, i->second);									// construct the controller
		BindController(controllers[key]);										// bind the function
	}
}

void Instruments::InitInstrument(Instrument & i)
{
	if (!i.initialised)
	{
		try
		{
			switch (i.type)
			{
			case INSTRUMENT_KEITHLEY:
				i.iNumber = keithleys.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(i.iNumber),
					std::forward_as_tuple(i.port)).first->first;
				break;
			case INSTRUMENT_MENSOR:
				i.iNumber = mensors.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(i.iNumber),
					std::forward_as_tuple(i.port)).first->first;
				break;
			case INSTRUMENT_NI_USB_9211A:
				i.iNumber = NI_USB_9211As.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(i.iNumber),
					std::forward_as_tuple(i.port)).first->first;
				break;
			case INSTRUMENT_NI_USB_6008:
				i.iNumber = NI_USB_6008s.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(i.iNumber),
					std::forward_as_tuple(i.port)).first->first;
				break;
			case NONE:
			case INSTRUMENT_UNDEF:
				break;
			default:
				// Should never be reached
				throw;
				break;
			}
			i.initialised = true;
		}
		catch (const std::exception& err)
		{
			LOG(logERROR) << MESSAGE_INSTRUMENT_INIT_FAIL << err.what();
		}
	}
}

void Instruments::ChangePort(Instrument & i)
{
	try
	{
		switch (i.type)
		{
		case INSTRUMENT_KEITHLEY:
			keithleys.find(i.iNumber)->second.SetComPort(i.port);
			break;
		case INSTRUMENT_MENSOR:
			mensors.find(i.iNumber)->second.SetComPort(i.port);
			break;
		case INSTRUMENT_NI_USB_9211A:
			NI_USB_9211As.find(i.iNumber)->second.SetComPort(i.port);
			break;
		case INSTRUMENT_NI_USB_6008:
			NI_USB_6008s.find(i.iNumber)->second.SetComPort(i.port);
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
	catch (const std::exception& err)
	{
		LOG(logERROR) << MESSAGE_INSTRUMENT_PORTCHANGE_FAIL << err.what();
	}
}

void Instruments::DeleteInstrument(Instrument & i)
{
	try
	{
		switch (i.type)
		{
		case INSTRUMENT_KEITHLEY:
			keithleys.erase(i.iNumber);
			break;
		case INSTRUMENT_MENSOR:
			mensors.erase(i.iNumber);
			break;
		case INSTRUMENT_NI_USB_9211A:
			NI_USB_9211As.erase(i.iNumber);
			break;
		case INSTRUMENT_NI_USB_6008:
			NI_USB_6008s.erase(i.iNumber);
			break;
		case NONE:
		case INSTRUMENT_UNDEF:
			break;
		default:
			// Should never be reached
			throw;
			break;
		}
		i.initialised = false;
	}
	catch (const std::exception& err)
	{
		LOG(logERROR) << MESSAGE_INSTRUMENT_DESTROY_FAIL << err.what();
	}

}


double Instruments::MeasureReader(unsigned int identifier)
{
	auto reader = readers.find(identifier);

	if (reader == readers.end())
	{
		auto ret = readers.emplace(identifier, Reader());				// get the result of the insertion (pair)
		reader = ret.first;												// and set the iterator the the newly created reader
	}
	return reader->second.readerfunction();
}

bool Instruments::MeasureController(unsigned int identifier)
{
	auto controller = controllers.find(identifier);

	if (controller == controllers.end())
	{
		auto ret = controllers.emplace(identifier, Controller());			// get the result of the insertion (pair)
		controller = ret.first;												// and set the iterator the the newly created controller
	}
	return controller->second.readerfunction();
}

bool Instruments::ActuateController(unsigned int identifier, bool state)
{
	auto controller = controllers.find(identifier);

	if (controller == controllers.end())
	{
		auto ret = controllers.emplace(identifier, Controller());			// get the result of the insertion (pair)
		controller = ret.first;												// and set the iterator the the newly created controller
	}
	return controller->second.controllerfunction(state);
}



void Instruments::BindReader(Reader & r) {

	auto instrument = instruments[r.instrument];

	switch (instrument.type)
	{
	case INSTRUMENT_KEITHLEY:
		r.readerfunction = std::bind(&Keithley::Read, &keithleys.find(instrument.iNumber)->second, r.channel);
		break;

	case INSTRUMENT_MENSOR:
		r.readerfunction = std::bind(&Mensor::Read, &mensors.find(instrument.iNumber)->second);
		break;

	case INSTRUMENT_NI_USB_9211A:
		r.readerfunction = std::bind(&NI_USB_9211A::Read, &NI_USB_9211As.find(instrument.iNumber)->second, r.channel);
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



void Instruments::BindController(Controller & c) {
	switch (c.type)
	{

	case INSTRUMENT_NI_USB_6008:
		c.readerfunction = std::bind(&NI_USB_6008::IsOpenSubchannel, &NI_USB_6008s.find(instruments[c.instrument].iNumber)->second, c.channel, c.identifier);
		c.controllerfunction = std::bind(&NI_USB_6008::SetSubchannel, &NI_USB_6008s.find(instruments[c.instrument].iNumber)->second, c.channel, c.identifier, std::placeholders::_1);
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