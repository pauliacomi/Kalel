//*****************************************************************************************************
//
//	This class extends the pressure reading class
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************

#pragma once

#include <string>

class MachineSettings;
class Instruments;

class PressureReader
{
public:
	PressureReader(Instruments & s, MachineSettings & m);
	~PressureReader(void);
	void Reset(MachineSettings & m);

	// Get low range pressure
	double ReadLowRangeP();

	// Get high range pressure
	double ReadHighRangeP();

private:
	Instruments & instruments;
	int lowrange;
	int highrange;
};
