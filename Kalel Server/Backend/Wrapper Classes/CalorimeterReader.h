//*****************************************************************************************************
//
//	This class extends the pressure reading class
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************

#ifndef _CALORIMETER_H_
#define _CALORIMETER_H_
#pragma once

#include <string>

class MachineSettings;
class ReadingInstruments;

class CalorimeterReader
{
public:
	CalorimeterReader(ReadingInstruments & s, MachineSettings & m);
	~CalorimeterReader(void);
	void Reset(MachineSettings & m);

	// Pass in the references to double variables to get the three temperatures
	double Read();

private:
	ReadingInstruments & instruments;
	int calorimeter;
};

#endif