#include "PressureReader.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../RS232/SerialInstruments.h"

PressureReader::PressureReader(ReadingInstruments & s)
	: instruments{ s }
{
	Reader r;
	r.type = READER_PRESSURE;
	r.identifier = PRESSURE_LP;
	readlrp = instruments.Bind(r);

	r.type = READER_PRESSURE;
	r.identifier = PRESSURE_HP;
	readhrp = instruments.Bind(r);
}

PressureReader::~PressureReader(void)
{
}

bool PressureReader::ReadLowRangeP(double * pressure_low_range)
{
	return readlrp(pressure_low_range);
}

bool PressureReader::ReadHighRangeP(double * pressure_high_range)
{
	return readhrp(pressure_high_range);
}

bool PressureReader::GetErrorLowRangeP(std::string * error)
{
	return false;
}

bool PressureReader::GetErrorHighRangeP(std::string * error)
{
	return false;
}
