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


class CalorimeterReader
{
public:
	CalorimeterReader(int port);
	~CalorimeterReader(void);

	// Pass in the references to double variables to get the three temperatures
	bool Read(double* calorimeter);

	// Get the port for temperature readings
	int GetReadPort();

	// Set the port for temperature readings
	void SetReadPort(int port);

private:
};

#endif