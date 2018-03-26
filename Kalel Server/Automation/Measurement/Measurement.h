#pragma once

#include "../CommonControls.h"
#include "Security.h"

// std::functionality
#include <atomic>
#include <string>
#include <memory>

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

	void ReadCalorimeter();
	void ReadPressure();
	void ReadTemperatures();
};

