#include "MachineSettings.h"


MachineSettings::MachineSettings()
{
}


MachineSettings::~MachineSettings()
{
}


int MachineSettings::TypeInstrument(int num)
{
	return typeInstruments[num];
}

int MachineSettings::COMInstrument(int num)
{
	return COMInstruments[num];
}

int MachineSettings::FonctionInstrument(int num)
{
	return FunctionInstruments[num];
}

void MachineSettings::SetInstrument(int num, int TypeInstrument, int COMInstrument, int FonctionInstrument)
{
	SetTypeInstrument(num, TypeInstrument);
	SetCOMInstrument(num, COMInstrument);
	SetFunctionInstrument(num, FonctionInstrument);
}

void MachineSettings::SetTypeInstrument(int num, int TypeInstrument)
{
	typeInstruments[num] = TypeInstrument;
}

void MachineSettings::SetCOMInstrument(int num, int COMInstrument)
{
	COMInstruments[num] = COMInstrument;
}

void MachineSettings::SetFunctionInstrument(int num, int FonctionInstrument)
{
	FunctionInstruments[num] = FonctionInstrument;
}