#include "PressureReader.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../Instruments.h"

PressureReader::PressureReader(Instruments & s, MachineSettings & m)
	: instruments{ s }
{
	Reset(m);
}

PressureReader::~PressureReader(void)
{
}

void PressureReader::Reset(MachineSettings & m)
{
	for (auto i = m.readers.begin(); i != m.readers.end(); ++i)
	{
		if (i->second.type == READER_PRESSURE)
		{
			if (i->second.identifier == PRESSURE_LP)
			{
				lowrange = i->first;
			}
			if (i->second.identifier == PRESSURE_HP)
			{
				highrange = i->first;
			}
		}
	}
}

double PressureReader::ReadLowRangeP()
{
	return instruments.readerfunctions[lowrange]() / instruments.readers[lowrange].sensitivity;
}

double PressureReader::ReadHighRangeP()
{
	return instruments.readerfunctions[highrange]() / instruments.readers[highrange].sensitivity;
}

