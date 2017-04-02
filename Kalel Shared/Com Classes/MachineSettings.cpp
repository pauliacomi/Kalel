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


void MachineSettings::AddReader(Reader r, Instrument i)
{
	bool existingi = false;
	int placei;

	for (auto kr : instruments) {
		if (kr.second.name == i.name)
		{
			kr.second.port = i.port;
			existingi = true;
			placei = kr.first;
		}
	}
	if (!existingi)
	{
		placei = instruments.rbegin()->first + 1;
		instruments.emplace(std::make_pair(placei, i));
	}


	bool existingr = false;
	int placer;

	for (auto kr : readers) {
		if (kr.second.type == r.type && kr.second.identifier == r.identifier)
		{
			kr.second.sensitivity = r.sensitivity;
			kr.second.channel = r.channel;
			kr.second.instrument = r.instrument;
			bool existingr = true;
		}
	}
	if (!existingr)
	{
		placer = readers.rbegin()->first + 1;
		readers.emplace(std::make_pair(placer, r));
	}
}
