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
#include "../../../Kalel Shared/timeHelpers.h"
#include "../../../Kalel Server/Automation/FileWriter.h"					// Writing file
#include "../../../Kalel Server/Backend/Wrapper Classes/ValveController.h"	// Valve class

// std::functionality
#include <string>
#include <thread>


Measurement::Measurement(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
	// Initialise instruments
	temperatureReader.reset(new TemperatureInstruments(s.machineSettings->PortTemperatures)); 
	serialReaders.reset(new SerialInstruments());

	std::string errorInit;
	if (!serialReaders->Init(&errorInit))
	{
		controls.messageHandler->DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
	}

	// Initialise security;
	security.reset(new Security(
		storage.machineSettings->ActivationSecurite, 
		*controls.valveControls,								
		*controls.messageHandler)
			);
}


Measurement::~Measurement()
{
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
		++storage.currentData->measurementsMade;												// Save the measurement number
		storage.currentData->timeElapsed = controls.timerExperiment.TimeMilliseconds();			// Save the time elapsed from the beginning of the experiment
		storage.currentData->timeToEquilibrateCurrent = controls.timerWaiting.TimeSeconds();	// Save the waiting time if it exists

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
		if (controls.timerMeasurement.TimeSeconds() > T_BETWEEN_RECORD)							// If enough time between measurements
		{
			if (storage.currentData->experimentRecording)										// If we started recording
			{
				// Save the data to the file
				controls.fileWriter->FileMeasurementRecord(storage.experimentSettings->dataGeneral ,*storage.currentData, controls.valveControls->ValveIsOpen(6));
			}

			// Restart the timer to record time between measurements
			controls.timerMeasurement.Start();

			// Increment the measurement number
			++storage.currentData->experimentGraphPoints;
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
	// Start the 4 threads
	measurementThreads.push_back(std::thread(&Measurement::ReadCalorimeter, this));
	measurementThreads.push_back(std::thread(&Measurement::ReadHighPressure, this));
	measurementThreads.push_back(std::thread(&Measurement::ReadLowPressure, this));
	measurementThreads.push_back(std::thread(&Measurement::ReadTemperatures, this));

	// Give the threads the start signal
	std::unique_lock<std::mutex> lk(lockingMutex);
	ready = true;											// Use bool to prevent accidental wake-up of threads
	syncThreadStart.notify_all();
	lk.unlock();

	// Wait for all threads to complete
	for (auto& th : measurementThreads)
	{
		th.join();
	}

	// Reset
	measurementThreads.clear();
	ready = false;
}


///------------------- Thread corresponding functions

void Measurement::ReadCalorimeter()
{
	// Wait until called
	std::unique_lock<std::mutex> lock(lockingMutex);
	syncThreadStart.wait(lock, [this] {return ready; });

	// Read the value from the calorimeter
	double calorimeter = 0;
	bool success;
	std::string error;

	success = serialReaders->ReadCalorimeter(&calorimeter);
	if (!success)
		serialReaders->GetErrorCalrimeter(&error);

	if (success == false) {
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->resultCalorimeter = calorimeter;
}


void Measurement::ReadLowPressure()
{
	// Wait until called
	std::unique_lock<std::mutex> lock(lockingMutex);
	syncThreadStart.wait(lock, [this] {return ready; });

	// Read the value from the calorimeter
	double pressureLowRange = 0;
	bool success;
	std::string error;

	success = serialReaders->ReadPressureLowRange(&pressureLowRange);
	if (!success)
		serialReaders->GetErrorLowRange(&error);

	if (success == false) {
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->pressureLow = pressureLowRange;
}


void Measurement::ReadHighPressure()
{
	// Wait until called
	std::unique_lock<std::mutex> lock(lockingMutex);
	syncThreadStart.wait(lock, [this] {return ready; });

	// Read the value from the calorimeter
	double pressureHighRange = 0;
	bool success;
	std::string error;

	success = serialReaders->ReadPressureHighRange(&pressureHighRange);
	if (!success)
		serialReaders->GetErrorHighRange(&error);

	if (success == false) {
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->pressureHigh = pressureHighRange;
}

void Measurement::ReadTemperatures()
{
	// Wait until called
	std::unique_lock<std::mutex> lock(lockingMutex);
	syncThreadStart.wait(lock, [this] {return ready; });

	// Read the value from the calorimeter
	double dTemperatureCalo = 0, dTemperatureCage = 0, dTemperaturePiece = 0;
	bool success;
	std::string error;

	success = temperatureReader->Read(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);
	if (!success)
		temperatureReader->GetError(&error);

	if (success == false) {
		controls.messageHandler->DisplayMessage(error);
	}

	// Write it in the shared object - NO need for mutex
	storage.currentData->temperatureCalo = dTemperatureCalo;
	storage.currentData->temperatureCage = dTemperatureCage;
	storage.currentData->temperatureRoom = dTemperaturePiece;
}
