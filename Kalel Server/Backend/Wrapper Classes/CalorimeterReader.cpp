#include "CalorimeterReader.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../Instruments.h"

CalorimeterReader::CalorimeterReader(Instruments & s, MachineSettings & m)
	: instruments{ s }
{
	Reset(m);
}

CalorimeterReader::~CalorimeterReader(void)
{
}

void CalorimeterReader::Reset(MachineSettings & m)
{
	for (auto i = m.readers.begin(); i != m.readers.end(); ++i)
	{
		if (i->second.type == READER_CALO)
		{
			calorimeter = i->first;
		}
	}
}

double CalorimeterReader::Read()
{
	return instruments.readerfunctions[calorimeter]() / instruments.readers[calorimeter].sensitivity;
}

