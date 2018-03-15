#include "Measurement.h"

// Resources
#include "../../../Kalel Shared/Resources/DefineText.h"						// Definitions for the text in the messages
#include "../../../Kalel Shared/Resources/DefineInstruments.h"				// All settings for automation are stored here

// Synchronization classes
#include "../../../Kalel Shared/Com Classes/ExperimentData.h"

// Utilities
#include "../../../Kalel Shared/timeHelpers.h"
#include "../../../Kalel Shared/log.h"

// std::functionality
#include <string>
#include <thread>


Measurement::Measurement(Storage &s, Controls &c)
	: storage{ s }
	, controls{ c }
{
	controls.timerMeasurement.Start();				// Start the timer to record time between measurements
}


Measurement::~Measurement()
{
}

////////////////////////////////////////////////////////
//	Main execution function
//
//	This is an infinite loop which has to be broken from inside
//	The general flowchart goes as follows:
//	
//	initialize everything
//	loop
//	{
//		1. Measure values from instruments + Record the time the measurement was done / the time between measurments
//		2. Do a security and safety check on the values
//		3. IF RECORDING 
//				save the data to the file, restart timer between measurements and increment measurement number
//		4. Save data to list in memory
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
		auto measurementTime = timeh::NowTime();

		/*
		*
		*		2. Do a security and safety check on the values
		*
		*/

		// Do the security checks
		controls.security.SecurityTemperatures(storage);
		controls.security.SecurityOverPressure(storage);

		/*
		*
		*		3. IF RECORDING save the data to the file, and restart timer between records
		*
		*/

		// Write data
		if (storage.experimentStatus.experimentRecording)													// If we started recording
		{
			if (controls.timerMeasurement.TimeMilliseconds() > storage.machineSettings.TimeBetweenRecording)// If enough time between measurements
			{
				// Save the data to the file
				bool err = controls.fileWriter.FileMeasurementRecord(
					timeh::TimePointToWString(measurementTime),
					storage.experimentSettings.dataGeneral ,
					storage.currentData, 
					storage.experimentStatus, 
					controls.valveControls.ValveIsOpen(VALVE_6));
				if (err) {
					LOG(logERROR) << MESSAGE_WARNING_FILE;
				}

				// Restart the timer to record time between measurements
				controls.timerMeasurement.Start();
			}
		}

		/*
		*
		*		4. Save data to list in memory
		*
		*/

		// Send the data out
		storage.dataCollection.push(measurementTime, ExperimentData(storage.currentData));

		// If no experiment running do not keep too many points
		if (!storage.experimentStatus.experimentInProgress && storage.dataCollection.size() > 500)
		{
			storage.dataCollection.del_first();
		}
		/*
		*
		*		5. Wait until next measurement
		*
		*/

		std::this_thread::sleep_for(std::chrono::milliseconds(storage.machineSettings.TimeBetweenMeasurement));
	}
}



void Measurement::ThreadMeasurement()
{
	// Start the threads
	measurementThreads.push_back(std::thread(&Measurement::ReadCalorimeter, this));
	measurementThreads.push_back(std::thread(&Measurement::ReadPressure, this));
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
	// Write it in the shared object - NO need for mutex
	storage.currentData.resultCalorimeter = controls.instruments.MeasureReader(CALO);

}

void Measurement::ReadPressure()
{
	// Wait until called
	std::unique_lock<std::mutex> lock(lockingMutex);
	syncThreadStart.wait(lock, [this] {return ready; });

	// Read the value from the pressure transmitter
	// Write it in the shared object - NO need for mutex
	storage.currentData.pressureLow = controls.instruments.MeasureReader(PRESSURE_LP);
	storage.currentData.pressureHigh = controls.instruments.MeasureReader(PRESSURE_HP);
}

void Measurement::ReadTemperatures()
{
	// Wait until called
	std::unique_lock<std::mutex> lock(lockingMutex);
	syncThreadStart.wait(lock, [this] {return ready; });

	// Read the value from the temperatures
	// Write it in the shared object - NO need for mutex
	storage.currentData.temperatureCalo = controls.instruments.MeasureReader(TEMPERATURE_CALO);
	storage.currentData.temperatureCage = controls.instruments.MeasureReader(TEMPERATURE_CAGE);
	storage.currentData.temperatureRoom = controls.instruments.MeasureReader(TEMPERATURE_ROOM);
}					   
