#pragma once

#include "../CommonControls.h"
#include "Security.h"

// std::functionality
#include <atomic>
#include <string>
#include <memory>

// forward declarations
class CTemperature;
class SerialInstruments;
class Storage;

class Measurement
{
public:
	Measurement(Storage &s, Controls &c);
	~Measurement();

	/**********************************************************************************************************************************
	// Variables
	***********************************************************************************************************************************/

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------
	
	// Inherited
	Storage & storage;
	Controls & controls;
	
	// New
	std::unique_ptr<CTemperature> g_pTemperature;									// Pointer to the class that deals with temperature recording
	std::unique_ptr<SerialInstruments> g_pSerialInstruments;						// Pointer to the class that deals with calorimeter & pressure recording
	std::unique_ptr<Security> security;

	//------------------------------------------------------------
	// Syncronisation primitives and threads
	//------------------------------------------------------------
	
	std::atomic_bool measuring = true;

	std::vector<std::thread> measurementThreads;									// Threads for measurement
	std::mutex lockingMutex;														// Thread sync mutex
	std::condition_variable syncThreadStart;										// Thread sync condition var
	bool ready = false;																// Thread sync bool

	/**********************************************************************************************************************************
	*
	// Measurements functions and threads
	*
	***********************************************************************************************************************************/

	void Execution();

	void ThreadMeasurement();
	void ReadCalorimeter();
	void ReadLowPressure();
	void ReadHighPressure();
	void ReadTemperatures();
};

