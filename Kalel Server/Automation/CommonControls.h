#ifndef COMCTRL_H
#define COMCTRL_H
#pragma once

#include "../MessageHandler.h"

#include "../Backend/RS232/SerialInstruments.h"							// Serial instruments


#include "../Backend/Wrapper Classes/ValveController.h"					// Valve class
#include "../Backend/Wrapper Classes/TemperatureReader.h"				// Temperature recording
#include "../Backend/Wrapper Classes/PressureReader.h"					// Pressure recording
#include "../Backend/Wrapper Classes/CalorimeterReader.h"				// Calorimeter recording
#include "../Automation/FileWriter.h"									// Writing file
#include "../Automation/Measurement/Security.h"							// Security parameters
#include "../../Kalel Shared/timeHelpers.h"

#include "CommonPointers.h"

#include <memory>

class Controls {
public:

	Controls(Storage &h);
	~Controls(void);

private:
	Storage & storage;

	SerialInstruments serialInstruments;

public:

	std::shared_ptr<MessageHandler> messageHandler;							// The message sender class.		Thread Safe!
	std::shared_ptr<FileWriter> fileWriter;									// The file writing class
	std::shared_ptr<ValveController> valveControls;							// The valve control/query class.	Thread Safe!
	std::shared_ptr<TemperatureReader> temperatureReader;					// Pointer to the class that deals with temperature recording
	std::shared_ptr<PressureReader> pressureReader;							// Pointer to the class that deals with pressure recording
	std::shared_ptr<CalorimeterReader> calorimeterReader;					// Pointer to the class that deals with calorimeter recording
	std::shared_ptr<Security> security;										// Pointer to the class that deals with security

	// Timers
	timeh::timer timerExperiment;											// Class for measuring the time from the experiment start
	timeh::timer timerMeasurement;											// Class for measuring the time between each measurement
	timeh::timer timerWaiting;												// Class for measuring the time to wait

	// On machine settings change
	void on_setmachineSettings() {
		if (valveControls->GetReadPort() != storage.machineSettings->PortValves)
		{
			valveControls->SetReadPort(storage.machineSettings->PortValves);
		}
		if (temperatureReader->GetReadPort() != storage.machineSettings->PortTemperatures)
		{
			temperatureReader->SetReadPort(storage.machineSettings->PortTemperatures);
		}
		serialReaders->SetSensitivity(
			storage.machineSettings->SensitivityCalo,
			storage.machineSettings->SensitivityLowPRange,
			storage.machineSettings->SensitivityHighPRange
			);
	}
};



inline Controls::Controls(Storage &h)
	: storage{ h }
	, serialInstruments{*messageHandler, h.machineSettings}
{
	// Create objects from controls class
	fileWriter			= std::make_shared<FileWriter>();
	messageHandler		= std::make_shared<MessageHandler>(h);
	valveControls		= std::make_shared<ValveController>(*messageHandler, h.machineSettings->PortValves);
	pressureReader		= std::make_shared<PressureReader>(h.machineSettings);
	temperatureReader	= std::make_shared<TemperatureReader>(h.machineSettings->PortTemperatures);
	security			= std::make_shared<Security>(h.machineSettings->ActivationSecurite, *valveControls, *messageHandler);
}

inline Controls::~Controls(void)
{
}



#endif