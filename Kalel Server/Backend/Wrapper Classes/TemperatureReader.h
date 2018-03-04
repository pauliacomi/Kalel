//*****************************************************************************************************
//
//	This class extends the temperature reading class
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************


#pragma once

#include <string>

class MachineSettings;
class Instruments;

class TemperatureReader
{
public:
	TemperatureReader(Instruments & s, MachineSettings & m);
	~TemperatureReader(void);
	void Reset(MachineSettings & m);

	// Get calorimeter temperature
	double ReadCalo();

	// Get cage temperature
	double ReadCage();

	// Get room temperature
	double ReadRoom();

private:
	Instruments & instruments;
	int calo_t;
	int cage_t;
	int room_t;
};
