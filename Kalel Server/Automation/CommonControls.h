#pragma once

#include "CommonPointers.h"

#include "../Backend/Instruments.h"										// Instruments

#include "../Backend/Wrapper Classes/ValveController.h"					// Valve class

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

public:

	// A class containing and managing all the reading instruments
	Instruments instruments;

	std::shared_ptr<FileWriter> fileWriter;									// The file writing class
	ValveController valveControls{ instruments };							// The valve control/query class -> simplification for instruments

	std::shared_ptr<Security> security;										// Pointer to the class that deals with security

	// Timers
	timeh::timer timerExperiment;											// Class for measuring the time from the experiment start
	timeh::timer timerMeasurement;											// Class for measuring the time between each measurement
	timeh::timer timerWaiting;												// Class for measuring the time to wait

	// TODO: why?
	// On machine settings change
	void on_setmachineSettings() {
		instruments.Reset(*storage.machineSettings);
	}
};



inline Controls::Controls(Storage &h)
	: storage{ h }
	, instruments{*h.machineSettings}
{
	// Create objects from controls class
	fileWriter			= std::make_shared<FileWriter>();
	security			= std::make_shared<Security>(h.machineSettings->ActivationSecurite, valveControls);
}

inline Controls::~Controls(void)
{
}