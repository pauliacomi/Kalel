#include "../Forcelib.h"

#include "MachineSettings.h"

#include "../timeHelpers.h"
#include "../Resources/DefineInstruments.h"


MachineSettings::MachineSettings()
{
	timeChanged = timeh::NowTime();
}

MachineSettings::~MachineSettings()
{
}

///*******************
///		Getters
///*******************

std::wstring MachineSettings::get_CaloName()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->CaloName;
}

std::wstring MachineSettings::get_CaloPrefix()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->CaloPrefix;
}

std::wstring MachineSettings::get_DefaultPath()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->DefaultPath;
}

bool MachineSettings::get_hasSonicNozzle()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->hasSonicNozzle;
}

bool MachineSettings::get_SafetyOn()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->SafetyOn;
}

float MachineSettings::get_VolumeRef()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->VolumeRef;
}

float MachineSettings::get_VolumeP6()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->VolumeP6;
}

int MachineSettings::get_InjectionAttemptNumber()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->InjectionAttemptNumber;
}

float MachineSettings::get_InjectionMargin()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->InjectionMargin;
}

float MachineSettings::get_InjectionMultiplier()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->InjectionMultiplier;
}

int MachineSettings::get_TimeBetweenMeasurement()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeBetweenMeasurement;
}

int MachineSettings::get_TimeBetweenRecording()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeBetweenRecording;
}

int MachineSettings::get_TimeBetweenAutomation()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeBetweenAutomation;
}

int MachineSettings::get_TimeWaitValves()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeWaitValves;
}

int MachineSettings::get_TimeWaitValvesShort()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeWaitValvesShort;
}

int MachineSettings::get_TimeWaitPump()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeWaitPump;
}

int MachineSettings::get_TimeVacuumEmergency()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeVacuumEmergency;
}

int MachineSettings::get_TimeVacuumBottle()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeVacuumBottle;
}

int MachineSettings::get_TimeVacuumEndDefault()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->TimeVacuumEndDefault;
}

float MachineSettings::get_PressurePumpVacuum()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->PressurePumpVacuum;
}

float MachineSettings::get_PressureLimitVacuum()
{
	std::lock_guard<std::mutex> lock(mtx);
	return this->PressureLimitVacuum;
}


///*******************
///		Setters
///*******************

void MachineSettings::set_CaloName(const std::wstring & s)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->CaloName = s;
}

void MachineSettings::set_CaloPrefix(const std::wstring & s)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->CaloPrefix = s;
}

void MachineSettings::set_DefaultPath(const std::wstring & s)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->DefaultPath = s;
}

void MachineSettings::set_hasSonicNozzle(bool x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->hasSonicNozzle = x;
}
void MachineSettings::set_SafetyOn(bool x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->SafetyOn = x;
}
void MachineSettings::set_VolumeRef(float x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->VolumeRef = x;
}
void MachineSettings::set_VolumeP6(float x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->VolumeP6 = x;
}
void MachineSettings::set_InjectionAttemptNumber(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->InjectionAttemptNumber = x;
}
void MachineSettings::set_InjectionMargin(float x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->InjectionMargin = x;
}
void MachineSettings::set_InjectionMultiplier(float x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->InjectionMultiplier = x;
}
void MachineSettings::set_TimeBetweenMeasurement(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeBetweenMeasurement = x;
}
void MachineSettings::set_TimeBetweenRecording(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeBetweenRecording = x;
}
void MachineSettings::set_TimeBetweenAutomation(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeBetweenAutomation = x;
}
void MachineSettings::set_TimeWaitValves(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeWaitValves = x;
}
void MachineSettings::set_TimeWaitValvesShort(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeWaitValvesShort = x;
}
void MachineSettings::set_TimeWaitPump(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeWaitPump = x;
}
void MachineSettings::set_TimeVacuumEmergency(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeVacuumEmergency = x;
}
void MachineSettings::set_TimeVacuumBottle(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeVacuumBottle = x;
}
void MachineSettings::set_TimeVacuumEndDefault(int x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->TimeVacuumEndDefault = x;
}
void MachineSettings::set_PressurePumpVacuum(float x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->PressurePumpVacuum = x;
}
void MachineSettings::set_PressureLimitVacuum(float x)
{
	std::lock_guard<std::mutex> lock(mtx);
	timeChanged = timeh::NowTime();
	this->PressureLimitVacuum = x;
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
