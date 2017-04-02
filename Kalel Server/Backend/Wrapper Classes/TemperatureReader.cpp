#include "TemperatureReader.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../Instruments.h"

TemperatureReader::TemperatureReader(Instruments & s, MachineSettings & m)
	: instruments{ s }
{
}

TemperatureReader::~TemperatureReader(void) 
{
}

void TemperatureReader::Reset(MachineSettings & m)
{
	for (auto i = m.readers.begin(); i != m.readers.end(); ++i)
	{
		if (i->second.type == READER_TEMPERATURE)
		{
			if (i->second.identifier == TEMPERATURE_CALO)
			{
				calo_t = i->first;
			}
			if (i->second.identifier == TEMPERATURE_CAGE)
			{
				cage_t = i->first;
			}
			if (i->second.identifier == TEMPERATURE_ROOM)
			{
				room_t = i->first;
			}
		}
	}
}


double TemperatureReader::ReadCalo()
{	
	return instruments.readerfunctions[calo_t]() / instruments.readers[calo_t].sensitivity;
}

double TemperatureReader::ReadCage()
{
	return instruments.readerfunctions[cage_t]() / instruments.readers[cage_t].sensitivity;
}

double TemperatureReader::ReadRoom()
{
	return instruments.readerfunctions[room_t]() / instruments.readers[room_t].sensitivity;
}
