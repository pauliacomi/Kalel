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


void MachineSettings::AddInstrument(Instrument i, unsigned int position)
{
	if (position != 0) {
		instruments[position].name = i.name;
		instruments[position].port = i.port;
	}
	else
	{
		if (i.name != 0)
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

