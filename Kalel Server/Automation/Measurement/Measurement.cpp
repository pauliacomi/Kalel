#include "../../stdafx.h"
#include "Measurement.h"

// Resources
#include "../../../Kalel Shared/Resources/DefineText.h"					// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineAutomationSettings.h"		// All settings for automation are stored here

// Synchronization classes
#include "../../MessageHandler.h"											// Handles all the messages from this class to the client
#include "../../../Kalel Shared/Com Classes/ExperimentData.h"

// Measurement classes
#include "../../Backend/Wrapper Classes/Temperature.h"						// Temperature recording
#include "../../Backend/Wrapper Classes/SerialInstruments.h"				// Pressure & Calorimeter recording

// Utilities
#include "../../../Kalel Shared/timestamp.h"
#include "../../../Kalel Server/Automation/FileWriter.h"					// Writing file
#include "../../../Kalel Server/Backend/Wrapper Classes/Vannes.h"			// Valve class
#include "../Utils/Chrono.h"												// Time keeping


// std::functionality
#include <string>
#include <thread>



Measurement::Measurement()
{
	// Initialise threads
	h_MeasurementThread[0] = NULL;
	h_MeasurementThread[1] = NULL;
	h_MeasurementThread[2] = NULL;
	h_MeasurementThread[3] = NULL;

	// Initialise events
	//   - Non signalled by default
	//   - With manual reinitiallisation
	h_MeasurementThreadStartEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// Initialisation of the critical section
	InitializeCriticalSection(&criticalSection);

	// Initialise instruments
	g_pTemperature = new CTemperature();
	g_pSerialInstruments = new SerialInstruments();

	std::string errorInit;
	if (!g_pSerialInstruments->Init(&errorInit))
	{
		messageHandler->DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
	}

	// Initialise security
	InitialisationSecurity();
}


Measurement::~Measurement()
{
	// Delete instruments
	delete g_pTemperature;
	delete g_pSerialInstruments;

	// Destroy the events
	CloseHandle(h_MeasurementThreadStartEvent);

	// Destroy the critical sections
	DeleteCriticalSection(&criticalSection);
}

////////////////////////////////////////////////////////
//	Main execution function
//
//	This is an infinite loop which has to be broken form inside
//	The general flowchart goes as follows:
//	
//	initialize everything
//	loop
//	{
//		1. Get the experiment settings if they are new
//		2. Run through the automation algorithm for the chosen program (nothing, manual, automatic, vacuum, etc)
//		3. Measure values from instruments
//		4. Do a security and safety check on the values
//		5. Record the time the measurement was done / the time between measurments
//		6. IF RECORDING, save the data to the file, restart timer between measurements and increment measurement number
//		7. IF WAITING, check whether the wait is complete and reset the wait
//		8. Display the data to the GUI
//		9. Event-based wait. If any events are triggered in this time, the thread performs the requested action.
//	}
//
//
//
////////////////////////////////////////////////////////

void Measurement::Execution()
{
	while (measuring)
	{
		/*
		*
		*		DEVICE AND TIME READING
		*
		*/

		// Start threads and read the data
		ThreadMeasurement();

		// Record time
		experimentLocalData->measurementsMade++;										// Save the measurement number
		experimentLocalData->timeElapsed = timerExperiment->TempsActuel();				// Save the time elapsed from the beginning of the experiment
		experimentLocalData->timeToEquilibrateCurrent = timerWaiting->TempsActuel();	// Save the waiting time if it exists
		experimentLocalData->timestamp = NowTime();

		/*
		*
		*		SECURITY CHECKS
		*
		*/

		// Do the security checks
		SecuriteTemperatures();
		SecuriteHautePression();

		/*
		*
		*		FILE WRITING AND MEASUREMENT RESET
		*
		*/

		// Write data
		if (timerMeasurement->TempsActuel() > T_BETWEEN_RECORD)						// If enough time between measurements
		{
			if (experimentLocalData->experimentRecording)								// If we started recording
			{
				// Save the data to the file
				fwrt->FileMeasurementRecord(*experimentLocalData, valves->VanneEstOuvert(6));
			}

			// Restart the timer to record time between measurements
			timerMeasurement->TopChrono();

			// Increment the measurement number
			experimentLocalData->experimentGraphPoints++;
		}

		/*
		*
		*		DATA EXCHANGE
		*
		*/

		// Send the data to be displayed to the GUI
		messageHandler->ExchangeData(*experimentLocalData);
	}
}





void Measurement::ThreadMeasurement()
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

DWORD WINAPI Measurement::ThreadProc_ReadCalorimeter(LPVOID lpParam)
{
	Measurement *manipulation = static_cast<Measurement *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadCalorimeter();

	return 0;
}

DWORD WINAPI Measurement::ThreadProc_ReadHighPressure(LPVOID lpParam)
{
	Measurement *manipulation = static_cast<Measurement *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadHighPressure();

	return 0;
}

DWORD WINAPI Measurement::ThreadProc_ReadLowPressure(LPVOID lpParam)
{
	Measurement *manipulation = static_cast<Measurement *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadLowPressure();

	return 0;
}

DWORD WINAPI Measurement::ThreadProc_ReadTemperature(LPVOID lpParam)
{
	Measurement *manipulation = static_cast<Measurement *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadTemperatures();

	return 0;
}


///------------------- Thread corresponding functions

void Measurement::ReadCalorimeter()
{
	// Read the value from the calorimeter
	double calorimeter = NULL;
	bool success;
	std::string error;

	success = g_pSerialInstruments->ReadCalorimeter(&calorimeter);
	if (!success)
		g_pSerialInstruments->GetErrorCalrimeter(&error);

	if (success == false) {
		messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData->resultCalorimeter = calorimeter;
	LeaveCriticalSection(&criticalSection);
}


void Measurement::ReadLowPressure()
{
	// Read the value from the calorimeter
	double pressureLowRange = NULL;
	bool success;
	std::string error;

	success = g_pSerialInstruments->ReadPressureLowRange(&pressureLowRange);
	if (!success)
		g_pSerialInstruments->GetErrorLowRange(&error);

	if (success == false) {
		messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData->pressureLow = pressureLowRange;
	LeaveCriticalSection(&criticalSection);
}


void Measurement::ReadHighPressure()
{
	// Read the value from the calorimeter
	double pressureHighRange = NULL;
	bool success;
	std::string error;

	success = g_pSerialInstruments->ReadPressureHighRange(&pressureHighRange);
	if (!success)
		g_pSerialInstruments->GetErrorHighRange(&error);

	if (success == false) {
		messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData->pressureHigh = pressureHighRange;
	LeaveCriticalSection(&criticalSection);
}

void Measurement::ReadTemperatures()
{
	// Read the value from the calorimeter
	double dTemperatureCalo = NULL, dTemperatureCage = NULL, dTemperaturePiece = NULL;
	bool success;
	std::string error;

	success = g_pTemperature->Read(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);
	if (!success)
		g_pTemperature->GetError(&error);

	if (success == false) {
		messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData->temperatureCalo = dTemperatureCalo;
	experimentLocalData->temperatureCage = dTemperatureCage;
	experimentLocalData->temperatureRoom = dTemperaturePiece;
	LeaveCriticalSection(&criticalSection);
}
