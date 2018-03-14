#pragma once

#include "CommonStorage.h"

#include "../Backend/Instruments.h"										// Instruments

#include "../Backend/Wrapper Classes/ValveController.h"					// Valve class

#include "../Automation/FileWriter.h"									// Writing file
#include "../Automation/Measurement/Security.h"							// Security parameters
#include "../../Kalel Shared/timeHelpers.h"

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

	FileWriter fileWriter;													// The file writing class
	ValveController valveControls{ instruments };							// The valve control/query class -> simplification for instruments
	Security security{ valveControls };										// Pointer to the class that deals with security

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
}

inline Controls::~Controls(void)
{
}