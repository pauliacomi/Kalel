/**********************************************
*
*
*
*
*
*
*	Calls to underlying functionality may use the same USB or serial port. 
*	Because of this, any "Read" call must be contained into a syncronisation primitive (here a CriticalSection)
*
*
*
*
***********************************************/

#include "StdAfx.h"
#include "Automation.h"


void Automation::ThreadMeasurement()
{
	// Start the 4 threads and get their handles
	// The functions wait for the hEvent
	h_MeasurementThread[0] = CreateThread(NULL, NULL, ThreadProc_ReadCalorimeter, this, NULL, NULL);
	h_MeasurementThread[1] = CreateThread(NULL, NULL, ThreadProc_ReadLowPressure, this, NULL, NULL);
	h_MeasurementThread[2] = CreateThread(NULL, NULL, ThreadProc_ReadHighPressure, this, NULL, NULL);
	h_MeasurementThread[3] = CreateThread(NULL, NULL, ThreadProc_ReadTemperature, this, NULL, NULL);

	// Set the event to start the threads
	SetEvent(h_MeasurementThreadStartEvent);

	// Wait for the threads to finish
	WaitForMultipleObjects(4, h_MeasurementThread, TRUE, INFINITE);

	// Clean-up
	for (size_t i = 0; i < 4; i++)
	{
		CloseHandle(h_MeasurementThread[i]);
		h_MeasurementThread[i] = NULL;
	}
	ResetEvent(h_MeasurementThreadStartEvent);
}


///------------------- Threads

DWORD WINAPI Automation::ThreadProc_ReadCalorimeter(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadCalorimeter();

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadHighPressure(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadHighPressure();

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadLowPressure(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadLowPressure();

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadTemperature(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadTemperatures();

	return 0;
}


///------------------- Thread corresponding functions

void Automation::ReadCalorimeter()
{
	// Read the value from the calorimeter
	double calorimeter = NULL;
	bool success;
	
	EnterCriticalSection(&criticalSection);
	success = g_pSerialInstruments->ReadCalorimeter(&calorimeter);
	LeaveCriticalSection(&criticalSection);

	if (success == false) {
		std::string error;
		EnterCriticalSection(&criticalSection);
		g_pSerialInstruments->GetErrorCalrimeter(&error);
		LeaveCriticalSection(&criticalSection);
		messageHandler.DisplayMessage(GENERIC_STRING, error.c_str());
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData.resultCalorimeter = calorimeter;
	LeaveCriticalSection(&criticalSection);
}


void Automation::ReadLowPressure()
{
	// Read the value from the calorimeter
	double pressureLowRange = NULL;
	bool success;

	EnterCriticalSection(&criticalSection);
	success = g_pSerialInstruments->ReadPressureLowRange(&pressureLowRange);
	LeaveCriticalSection(&criticalSection);

	if (success == false) {
		std::string error;
		EnterCriticalSection(&criticalSection);
		g_pSerialInstruments->GetErrorLowRange(&error);
		LeaveCriticalSection(&criticalSection);
		messageHandler.DisplayMessage(GENERIC_STRING, error.c_str());
	}
	
	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData.pressureLow = pressureLowRange;
	LeaveCriticalSection(&criticalSection);
}


void Automation::ReadHighPressure()
{
	// Read the value from the calorimeter
	double pressureHighRange = NULL;
	bool success;

	EnterCriticalSection(&criticalSection);
	success = g_pSerialInstruments->ReadPressureHighRange(&pressureHighRange);
	LeaveCriticalSection(&criticalSection);

	if (success == false) {
		std::string error;
		EnterCriticalSection(&criticalSection);
		g_pSerialInstruments->GetErrorHighRange(&error);
		LeaveCriticalSection(&criticalSection);
		messageHandler.DisplayMessage(GENERIC_STRING, error.c_str());
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData.pressureHigh = pressureHighRange;
	LeaveCriticalSection(&criticalSection);
}

void Automation::ReadTemperatures()	// another problem is that the threads are reading something from the automation thread and may cause unexpected behaviour
{
	// Read the value from the calorimeter
	double dTemperatureCalo = NULL, dTemperatureCage = NULL, dTemperaturePiece = NULL;
	bool success;

	EnterCriticalSection(&criticalSection);
	success = g_pTemperature->Read(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);
	LeaveCriticalSection(&criticalSection);

	if (success == false) {
		std::string error;
		EnterCriticalSection(&criticalSection);
		g_pTemperature->GetError(&error);
		LeaveCriticalSection(&criticalSection);
		messageHandler.DisplayMessage(GENERIC_STRING, error.c_str());
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection); 
	experimentLocalData.temperatureCalo = dTemperatureCalo;
	experimentLocalData.temperatureCage = dTemperatureCage;
	experimentLocalData.temperatureRoom = dTemperaturePiece;
	LeaveCriticalSection(&criticalSection);
}
