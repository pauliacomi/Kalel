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


class PressureReader
{
public:
	PressureReader(int port);
	~PressureReader(void);

	// Pass in the references to double variables to get the three temperatures
	bool Read(double* pressure_low_range, double* pressure_high_range);

	// Get calorimeter temperature
	bool ReadLowRangeP(double* pressure_low_range);

	// Get cage temperature
	bool ReadHighRangeP(double* pressure_high_range);

	// Get the port for temperature readings
	int GetReadPort();

	// Set the port for temperature readings
	void SetReadPort(int port);

private:
};

#endif