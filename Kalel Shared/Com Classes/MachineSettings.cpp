#include "../Forcelib.h"
#include "MachineSettings.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	// TODO: remove initialisations
	Instrument a;
	a.name = INSTRUMENT_KEITHLEY;
	a.port = 1;

	instruments.insert(std::make_pair(1, a));
	instruments.insert(std::make_pair(2, a));
	instruments.insert(std::make_pair(3, a));


	Reader b;
	b.type = READER_PRESSURE;
	b.identifier = PRESSURE_LP;
	b.channel = 1;
	b.sensitivity = 1;
	b.instrument = 1;

	readers.insert(std::make_pair(1, b));
	readers.insert(std::make_pair(2, b));
	readers.insert(std::make_pair(3, b));
	readers.insert(std::make_pair(4, b));
	readers.insert(std::make_pair(5, b));
	readers.insert(std::make_pair(6, b));
}


MachineSettings::~MachineSettings()
{
}
