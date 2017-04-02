#include "../Forcelib.h"
#include "MachineSettings.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	// TODO: remove initialisations
	Instrument a;

	instruments.insert(std::make_pair(1, a));
	instruments.insert(std::make_pair(2, a));
	instruments.insert(std::make_pair(3, a));
	instruments.insert(std::make_pair(4, a));

	Reader b;

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

void MachineSettings::AddInstrument(unsigned int number, int name, int port)
{
	Instrument a;
	a.name = name;
	a.port = port;
}

void MachineSettings::AddReader(int type, int identifier, int sensitivity, int channel, int instrumentcorresponding)
{
}
