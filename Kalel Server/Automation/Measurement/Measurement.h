#pragma once

// std::functionality
#include <atomic>
#include <string>
#include <memory>

// forward declarations
class CTemperature;
class SerialInstruments;
class MessageHandler;
class ExperimentData;
class CChrono;
class ExperimentSettings;
class Storage;
class CVannes;

class Measurement
{
public:
	Measurement();
	~Measurement();

	/**********************************************************************************************************************************
	// Variables
	***********************************************************************************************************************************/

	//------------------------------------------------------------
	// Pointers
	//------------------------------------------------------------

	CTemperature* g_pTemperature;									// Pointer to the class that deals with temperature recording
	SerialInstruments * g_pSerialInstruments;						// Pointer to the class that deals with calorimeter & pressure recording
	MessageHandler* messageHandler;									// This class will send all the messages out
	ExperimentData* experimentLocalData;							// Local data
	CChrono* timerExperiment;										// Class for measuring the time from the experiment start
	CChrono* timerMeasurement;										// Class for measuring the time between each measurement
	CChrono* timerWaiting;											// Class for measuring the time to wait
	Storage* storageVectors;
	CVannes* valves;
	std::shared_ptr<ExperimentSettings> experimentLocalSettings;	// Pointer to the experiment settings from the main class, this is only read, never written

	//------------------------------------------------------------
	// Syncronisation primitives and threads
	//------------------------------------------------------------
	
	std::atomic_bool measuring = true;

	CRITICAL_SECTION criticalSection;					// Critical section for measurement thread sinchronisation

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



	/**********************************************************************************************************************************
	*
	// Security measures and checks
	*
	***********************************************************************************************************************************/

protected:

	bool securityActivated;

	// Keep limits in memory
	float security_PressureHigh_HighRange;
	float security_PressureHigh_LowRange;

	// Bool flags to keep track of security
	bool security_PressureHigh_flag;
	bool security_TemperatureHigh_flag;
	bool security_TemperatureLow_flag;

	void InitialisationSecurity();

	void SecuriteHautePression();
	void SecuriteHautePressionManuelle();
	void SecuriteHautePressionAuto();

	void SecuriteTemperatures();
	void SecuriteTemperaturesManuelle();
	void SecuriteTemperaturesAuto();
};

