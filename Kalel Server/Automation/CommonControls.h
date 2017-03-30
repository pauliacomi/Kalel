#ifndef COMCTRL_H
#define COMCTRL_H
#pragma once

#include "../MessageHandler.h"
#include "../Automation/FileWriter.h"									// Writing file
#include "../Backend/Wrapper Classes/ValveController.h"					// Valve class
#include "../Backend/Wrapper Classes/Temperature.h"						// Temperature recording
#include "../Backend/Wrapper Classes/SerialInstruments.h"				// Pressure & Calorimeter recording
#include "../Automation/Measurement/Security.h"							// Security parameters
#include "../../Kalel Shared/timeHelpers.h"

#include "CommonPointers.h"

#include <memory>

class Controls {
public:

	Controls(Storage &h);
	~Controls(void);

	Storage & storage;

	std::shared_ptr<MessageHandler> messageHandler;							// The message sender class.		Thread Safe!
	std::shared_ptr<ValveController> valveControls;							// The valve control/query class.	Thread Safe!
	std::shared_ptr<FileWriter> fileWriter;									// The file writing class
	std::shared_ptr<TemperatureInstruments> temperatureReader;				// Pointer to the class that deals with temperature recording
	std::shared_ptr<SerialInstruments> serialReaders;						// Pointer to the class that deals with calorimeter & pressure recording
	std::shared_ptr<Security> security;										// Pointer to the class that deals with security

	// Timers
	timeh::timer timerExperiment;											// Class for measuring the time from the experiment start
	timeh::timer timerMeasurement;											// Class for measuring the time between each measurement
	timeh::timer timerWaiting;												// Class for measuring the time to wait

	// On machine settings change
	void on_setmachineSettings() {
		if (valveControls->GetReadPort() != storage.machineSettings->PortVannes)
		{
			valveControls->SetReadPort(storage.machineSettings->PortVannes);
		}
		if (temperatureReader->GetReadPort() != storage.machineSettings->PortTemperatures)
		{
			temperatureReader->SetReadPort(storage.machineSettings->PortTemperatures);
		}
		serialReaders->SetSensitivity(
			storage.machineSettings->SensibiliteCalo,
			storage.machineSettings->SensibiliteCapteurBassePression,
			storage.machineSettings->SensibiliteCapteurHautePression
			);
	}
};



inline Controls::Controls(Storage &h)
	: storage{ h }
{
	// Create objects from controls class
	fileWriter			= std::make_shared<FileWriter>();
	messageHandler		= std::make_shared<MessageHandler>(h);
	valveControls		= std::make_shared<ValveController>(*messageHandler, h.machineSettings->PortVannes);
	temperatureReader	= std::make_shared<TemperatureInstruments>(h.machineSettings->PortTemperatures);
	serialReaders		= std::make_shared<SerialInstruments>(*messageHandler, *h.machineSettings);
	security			= std::make_shared<Security>(h.machineSettings->ActivationSecurite, *valveControls, *messageHandler);
}

inline Controls::~Controls(void)
{
}



#endif