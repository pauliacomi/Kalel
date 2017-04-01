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

#include <functional>
#include <map>

class MachineSettings;
class SerialInstruments;

class PressureReader
{
public:
	PressureReader(MachineSettings & m, SerialInstruments & s);
	~PressureReader(void);
	// Get the port for temperature readings
	void Reset();

	// Pass in the references to double variables to get the three temperatures
	bool Read(double* pressure_low_range, double* pressure_high_range);

	// Get calorimeter temperature
	bool ReadLowRangeP(double* pressure_low_range);

	// Get cage temperature
	bool ReadHighRangeP(double* pressure_high_range);

private:
	std::map<Reader, std::function<void(double* pressure)>> functions;
};

#endif