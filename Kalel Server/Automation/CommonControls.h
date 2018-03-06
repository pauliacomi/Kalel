#pragma once

#include "CommonPointers.h"

#include "../Backend/Instruments.h"										// Instruments

#include "../Backend/Wrapper Classes/ValveController.h"					// Valve class
#include "../Backend/Wrapper Classes/TemperatureReader.h"				// Temperature recording
#include "../Backend/Wrapper Classes/PressureReader.h"					// Pressure recording
#include "../Backend/Wrapper Classes/CalorimeterReader.h"				// Calorimeter recording

#include "../Automation/FileWriter.h"									// Writing file
#include "../Automation/Measurement/Security.h"							// Security parameters
#include "../../Kalel Shared/timeHelpers.h"

// Todo: remove
#include "../../Kalel Shared/Resources/DefineInstruments.h"


#include <memory>

class Controls {
public:

	Controls(Storage &h);
	~Controls(void);

private:
	Storage & storage;

	// A class containing and managing all the reading instruments
	Instruments instruments;

public:

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

	// TODO: why?
	// On machine settings change
	void on_setmachineSettings() {
		instruments.Reset(*storage.machineSettings);
		valveControls->Reset(*storage.machineSettings);
		pressureReader->Reset(*storage.machineSettings);
		temperatureReader->Reset(*storage.machineSettings);
		calorimeterReader->Reset(*storage.machineSettings);
	}
};



inline Controls::Controls(Storage &h)
	: storage{ h }
	, instruments{*h.machineSettings}
{
	// Create objects from controls class
	fileWriter			= std::make_shared<FileWriter>();

	// controls
	// TODO: rewrite valve in the style of others
	for (auto i = h.machineSettings->instruments.begin(); i != h.machineSettings->instruments.end(); ++i) {
		if (i->second.type == INSTRUMENT_NI_USB_6008) {
			valveControls = std::make_shared<ValveController>(i->second.port);
		}
	}
	// readers
	pressureReader		= std::make_shared<PressureReader>(instruments, *h.machineSettings);
	temperatureReader	= std::make_shared<TemperatureReader>(instruments, *h.machineSettings);
	calorimeterReader	= std::make_shared<CalorimeterReader>(instruments, *h.machineSettings);

	security			= std::make_shared<Security>(h.machineSettings->ActivationSecurite, *valveControls);
}

inline Controls::~Controls(void)
{
}