#pragma once

#include "CommonStorage.h"

#include "../Backend/Instruments.h"										// Instruments

#include "../Backend/Wrapper Classes/ValveController.h"					// Valve class

#include "../Automation/FileWriter.h"									// Writing file
#include "../Automation/Measurement/Security.h"							// Security parameters
#include "../../Kalel Shared/timeHelpers.h"


class Controls {
public:

	Controls(const MachineSettings &ms);
	~Controls();

public:

	// A class containing and managing all the reading instruments
	Instruments instruments;

	FileWriter fileWriter;													// The file writing class
	ValveController valveControls{ instruments };							// The valve control/query class -> simplification for instruments
	Security security{ valveControls };										// Pointer to the class that deals with security

};



inline Controls::Controls(const MachineSettings &ms)
	: instruments{ms}
{
}

inline Controls::~Controls(void)
{
}