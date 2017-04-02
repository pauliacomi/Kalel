//*****************************************************************************************************
//
//	This class extends the temperature reading class
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************

#ifndef _TEMPERATURE_H_
#define _TEMPERATURE_H_
#pragma once

#include <string>

class MachineSettings;
class ReadingInstruments;

class TemperatureReader
{
public:
	TemperatureReader(ReadingInstruments & s, MachineSettings & m);
	~TemperatureReader(void);
	void Reset(MachineSettings & m);

	// Get calorimeter temperature
	double ReadCalo();

	// Get cage temperature
	double ReadCage();

	// Get room temperature
	double ReadRoom();

private:
	ReadingInstruments & instruments;
	int calo_t;
	int cage_t;
	int room_t;
};

#endif