//*****************************************************************************************************
//
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************

#pragma once

#include <string>

class MachineSettings;
class Instruments;

class CalorimeterReader
{
public:
	CalorimeterReader(Instruments & s, MachineSettings & m);
	~CalorimeterReader(void);
	void Reset(MachineSettings & m);

	// Pass in the references to double variables to get the three temperatures
	double Read();

private:
	Instruments & instruments;
	int calorimeter;
};