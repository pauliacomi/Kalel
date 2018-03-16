#include "../Forcelib.h"

#include "MachineSettings.h"

#include "../timeHelpers.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	timeChanged = timeh::NowTime();
}

MachineSettings::MachineSettings(const MachineSettings &)
{
}

MachineSettings::~MachineSettings()
{
}

MachineSettings & MachineSettings::operator=(const MachineSettings * p)
{
	if (this != p) {  // make sure not same object

		CaloName = p->CaloName;
		CaloPrefix = p->CaloPrefix;
		DefaultPath = p->DefaultPath;
		hasSonicNozzle = p->hasSonicNozzle;							
		SafetyOn = p->SafetyOn;									
		VolumeRef = p->VolumeRef;									
		VolumeP6 = p->VolumeP6;										
		InjectionAttemptNumber = p->InjectionAttemptNumber;
		InjectionMargin = p->InjectionMargin;
		InjectionMultiplier = p->InjectionMultiplier;
		TimeBetweenMeasurement = p->TimeBetweenMeasurement;						
		TimeBetweenRecording = p->TimeBetweenRecording;
		TimeBetweenAutomation = p->TimeBetweenAutomation;
		TimeWaitValves = p->TimeWaitValves;									
		TimeWaitValvesShort = p->TimeWaitValvesShort;
		TimeWaitPump = p->TimeWaitPump;
		TimeVacuumEmergency = p->TimeVacuumEmergency;
		TimeVacuumBottle = p->TimeVacuumBottle;
		TimeVacuumEndDefault = p->TimeVacuumEndDefault;
		PressurePumpVacuum = p->PressurePumpVacuum;
		PressureLimitVacuum = p->PressureLimitVacuum;

		instruments = p->instruments;			
		readers = p->readers;				
		controllers = p->controllers;		

	}
	return *this;    // Return ref for multiple assignment
}


void MachineSettings::AddInstrument(Instrument i, unsigned int position /*=0*/)
{
	if (position == 0)
	{
		unsigned int placer = instruments.size() + 1;
		instruments.emplace(std::make_pair(placer, i));
		return;
	}
	if (instruments.find(position) != instruments.end()) {
		instruments[position] = i;
	}
	else
	{
		if (i.type != 0)
		{
			instruments.emplace(std::make_pair(position, i));
		}
	}
}

void MachineSettings::AddReader(Reader r, unsigned int position)
{
	if (position == 0)
	{
		unsigned int placer = readers.size() + 1;
		readers.emplace(std::make_pair(placer, r));
		return;
	}
	if (readers.find(position) != readers.end()) {
		readers[position] = r;
		readers[position].readerfunction = r.readerfunction;
	}
	else
	{
		if (r.type != 0 && r.identifier != 0)
		{
			readers.emplace(std::make_pair(position, r));
		}
	}
}

void MachineSettings::AddController(Controller r, unsigned int position)
{
	if (position == 0)
	{
		unsigned int placer = controllers.size() + 1;
		controllers.emplace(std::make_pair(placer, r));
		return;
	}
	if (controllers.find(position) != controllers.end()) {
		controllers[position] = r;
		controllers[position].controllerfunction = r.controllerfunction;
	}
	else
	{
		if (r.type != 0 && r.identifier != 0)
		{
			controllers.emplace(std::make_pair(position, r));
		}
	}
}
