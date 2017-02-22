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
class ExperimentData;
class ExperimentSettings;
class Storage;
class FileWriter;

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

	HANDLE h_MeasurementThreadStartEvent;				// Handle event doing measurement thread signalling
	HANDLE h_MeasurementThread[4];						// Threads for measurement

	/**********************************************************************************************************************************
	*
	// Measurements functions and threads
	*
	***********************************************************************************************************************************/

	void Execution();

	void ThreadMeasurement();
	static DWORD WINAPI  ThreadProc_ReadCalorimeter(LPVOID lpParam);
	static DWORD WINAPI  ThreadProc_ReadHighPressure(LPVOID lpParam);
	static DWORD WINAPI  ThreadProc_ReadLowPressure(LPVOID lpParam);
	static DWORD WINAPI  ThreadProc_ReadTemperature(LPVOID lpParam);

	void ReadCalorimeter();
	void ReadLowPressure();
	void ReadHighPressure();
	void ReadTemperatures();
};

