#include "../Forcelib.h"
#include "MachineSettings.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	// TODO: remove initialisations

	instruments.insert(std::make_pair(1, Instrument()));
	instruments.insert(std::make_pair(2, Instrument()));
	instruments.insert(std::make_pair(3, Instrument()));
	instruments.insert(std::make_pair(4, Instrument()));

	readers.insert(std::make_pair(1, Reader()));
	readers.insert(std::make_pair(2, Reader()));
	readers.insert(std::make_pair(3, Reader()));
	readers.insert(std::make_pair(4, Reader()));
	readers.insert(std::make_pair(5, Reader()));
	readers.insert(std::make_pair(6, Reader()));
}


MachineSettings::~MachineSettings()
{
}


void MachineSettings::AddInstrument(Instrument i, unsigned int position)
{
	if (position != 0) {
		instruments[position].type = i.type;
		instruments[position].port = i.port;
	}
	else
	{
		if (i.type != 0)
		{
			int placer = instruments.rbegin()->first + 1;
			instruments.emplace(std::make_pair(placer, i));
		}
	}
}

void MachineSettings::AddReader(Reader r, unsigned int position)
{
	if (position != 0) {
		readers[position].type			= r.type		;
		readers[position].identifier	= r.identifier	;
		readers[position].sensitivity	= r.sensitivity	;
		readers[position].channel		= r.channel		;
		readers[position].instrument	= r.instrument	;
	}
	else
	{
		if (r.type != 0 && r.identifier != 0)
		{
			int placer = readers.rbegin()->first + 1;
			readers.emplace(std::make_pair(placer, r));
		}
	}
}

void MachineSettings::AddController(Controller r, unsigned int position)
{
	if (position != 0) {
		controllers[position].type			= r.type		;
		controllers[position].identifier	= r.identifier	;
		controllers[position].sensitivity	= r.sensitivity	;
		controllers[position].channel		= r.channel		;
		controllers[position].instrument	= r.instrument	;
	}
	else
	{
		if (r.type != 0 && r.identifier != 0)
		{
			int placer = controllers.rbegin()->first + 1;
			controllers.emplace(std::make_pair(placer, r));
		}
	}
}

