#include "CalorimeterReader.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../Instruments.h"

CalorimeterReader::CalorimeterReader(ReadingInstruments & s)
	: instruments{ s }
{
}

CalorimeterReader::~CalorimeterReader(void)
{
}

bool CalorimeterReader::Read(double * pressure_low_range)
{
	Reader r;
	r.type = READER_PRESSURE;
	r.identifier = PRESSURE_LP;
	return instruments.Bind(r);
}

bool CalorimeterReader::GetError(std::string * error)
{
	return false;
}
