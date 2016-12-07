#include "../../stdafx.h"
#include "Measurement.h"

// Resources
#include "../../../Kalel Shared/Resources/DefineText.h"					// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineAutomationSettings.h"		// All settings for automation are stored here

// Synchronization classes
#include "../../../Kalel Shared/Com Classes/ExperimentData.h"

// Measurement classes
#include "../../Backend/Wrapper Classes/Temperature.h"						// Temperature recording
#include "../../Backend/Wrapper Classes/SerialInstruments.h"				// Pressure & Calorimeter recording

// Utilities
#include "../../../Kalel Shared/timestamp.h"
#include "../../../Kalel Server/Automation/FileWriter.h"					// Writing file
#include "../../../Kalel Server/Backend/Wrapper Classes/ValveController.h"	// Valve class
#include "../Utils/Chrono.h"												// Time keeping


// std::functionality
#include <string>
#include <thread>



Measurement::Measurement(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
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

	// Initialise instruments
	g_pTemperature = new CTemperature(); 
	g_pSerialInstruments = new SerialInstruments();

	std::string errorInit;
	if (!g_pSerialInstruments->Init(&errorInit))
	{
		controls.messageHandler->DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
	}

	// Initialise security;
	security = new Security(
		storage.machineSettings->ActivationSecurite, 
		*controls.valveControls,								
		*controls.messageHandler);
}


Measurement::~Measurement()
{
	// Delete security class
	delete security;

	// Delete instruments
	delete g_pTemperature;
	delete g_pSerialInstruments;

	// Destroy the events
	CloseHandle(h_MeasurementThreadStartEvent);
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
//		1. Measure values from instruments + Record the time the measurement was done / the time between measurments
//		2. Do a security and safety check on the values
//		3. IF RECORDING 
//				save the data to the file, restart timer between measurements and increment measurement number
//		4. Save data
//		5. Wait until next measurement
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
		*		1. Measure values from instruments + Record the time the measurement was done / the time between measurments
		*
		*/

		// Start threads and read the data
		ThreadMeasurement();

		// Record time
		storage.currentData->measurementsMade++;												// Save the measurement number
		storage.currentData->timeElapsed = controls.timerExperiment.TempsActuel();				// Save the time elapsed from the beginning of the experiment
		storage.currentData->timeToEquilibrateCurrent = controls.timerWaiting.TempsActuel();	// Save the waiting time if it exists
		storage.currentData->timestamp = NowTime();

		/*
		*
		*		2. Do a security and safety check on the values
		*
		*/

		// Do the security checks
		security->SecurityTemperatures(
			storage.experimentSettings->experimentType,											// Experiment type to ensure proper response
			storage.currentData->temperatureCalo + securite_temperature,						// Maximum temperature limit
			storage.currentData->temperatureCalo - securite_temperature,						// Minimum temperature limit
			*storage.currentData																// Current data recorded to do the checks
			);

		security->SecurityHighPressure(															
			storage.experimentSettings->experimentType,											// Experiment type to ensure proper response
			storage.machineSettings->PressionSecuriteBassePression,								// Maximum pressure limit
			storage.machineSettings->PressionSecuriteHautePression,								// Minimum pressure limit
			*storage.currentData																// Current data recorded to do the checks
		);

		/*
		*
		*		3. IF RECORDING save the data to the file, restart timer between measurements and increment measurement number
		*
		*/

		// Write data
		if (controls.timerMeasurement.TempsActuel() > T_BETWEEN_RECORD)							// If enough time between measurements
		{
			if (storage.currentData->experimentRecording)										// If we started recording
			{
				// Save the data to the file
				controls.fileWriter->FileMeasurementRecord(storage.experimentSettings->dataGeneral ,*storage.currentData, controls.valveControls->ValveIsOpen(6));
			}

			// Restart the timer to record time between measurements
			controls.timerMeasurement.TopChrono();

			// Increment the measurement number
			storage.currentData->experimentGraphPoints++;
		}

		/*
		*
		*		4. Save data
		*
		*/

		// Send the data to be displayed to the GUI
		controls.messageHandler->ExchangeData(*storage.currentData);

		/*
		*
		*		5. Wait until next measurement
		*
		*/

		std::this_thread::sleep_for(std::chrono::milliseconds(T_BETWEEN_MEASURE));
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
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->resultCalorimeter = calorimeter;
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
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->pressureLow = pressureLowRange;
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
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->pressureHigh = pressureHighRange;
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
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->temperatureCalo = dTemperatureCalo;
	storage.currentData->temperatureCage = dTemperatureCage;
	storage.currentData->temperatureRoom = dTemperaturePiece;
}
