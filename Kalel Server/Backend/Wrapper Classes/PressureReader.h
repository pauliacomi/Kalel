//*****************************************************************************************************
//
//	This class extends the pressure reading class
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************

#ifndef _PRESSURE_H_
#define _PRESSURE_H_
#pragma once

#include <string>

class MachineSettings;
class ReadingInstruments;

class PressureReader
{
public:
	PressureReader(ReadingInstruments & s, MachineSettings & m);
	~PressureReader(void);
	void Reset(MachineSettings & m);

	// Get low range pressure
	double ReadLowRangeP();

	// Get high range pressure
	double ReadHighRangeP();

private:
	ReadingInstruments & instruments;
	int lowrange;
	int highrange;
};

#endif