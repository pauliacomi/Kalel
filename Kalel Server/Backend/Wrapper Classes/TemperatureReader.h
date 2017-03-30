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

#include "../USB/NI_USB_9211A.h"

#include <mutex>

class TemperatureReader :
	public NI_USB_9211A
{
protected:

public:
	TemperatureReader(int port);
	~TemperatureReader(void);

	// Pass in the references to double variables to get the three temperatures
	bool Read(double* Temperature_Calo, double* Temperature_Cage, double* Temperature_Piece);	

	// Get calorimeter temperature
	bool ReadCalo(double* Temperature_Calo);

	// Get cage temperature
	bool ReadCage(double* Temperature_Cage);

	// Get room temperature
	bool ReadRoom(double* Temperature_Piece);

	// Get the port for temperature readings
	int GetReadPort();

	// Set the port for temperature readings
	void SetReadPort(int port);

private:
	std::mutex ctrlmutex;						// locks to prevent clash of threads 
};

#endif