#include "PressureReader.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../RS232/SerialInstruments.h"

PressureReader::PressureReader(MachineSettings & m, SerialInstruments & s)
{
	for (size_t i = 0; i < m.pressure.size(); i++)
	{
		functions.insert(std::pair<Reader, std::function<void(double* p)>>(m.pressure[i], s.Link(m.pressure[i])));
	}
}

PressureReader::~PressureReader(void)
{
}
