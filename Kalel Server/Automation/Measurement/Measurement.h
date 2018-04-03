#pragma once

#include "Security.h"
#include "../CommonControls.h"
#include "../../../Kalel Shared/dispatchQueue.h"

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
	
	std::atomic_bool eventShutdown = false;

	dispatch_queue measurementThreads{std::thread::hardware_concurrency()};			// Threads for measurement

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

