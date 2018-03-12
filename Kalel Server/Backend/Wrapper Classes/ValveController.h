//*****************************************************************************************************
//
//	This class makes the valve controlling easier by providing simple functions
//
//*****************************************************************************************************

#pragma once

#include "../Instruments.h"

class ValveController
{
public:
	ValveController(Instruments & i);
	~ValveController(void);

	Instruments & instruments;

	bool ValveOpen(int num, bool verbose);		// Open a valve
	bool ValveClose(int num, bool verbose);		// Close a valve

	bool EVActivate(int num, bool verbose);		// Activate EV
	bool EVDeactivate(int num, bool verbose);	// Deactivate EV

	bool PumpActivate(bool verbose);			// Activate Pump
	bool PumpDeactivate(bool verbose);			// Deactivate Pump

	bool CloseAll(bool verbose);				// Close everything
	bool CloseAllValves(bool verbose);			// Close all the valves
	bool CloseEVsAndPump(bool verbose);			// Close the Pump and EV's
	bool CloseEverything(bool verbose);			// Close all things

	// Checks for activation

	bool ValveIsOpen(int num);
	bool EVIsActive(int num);
	bool PumpIsActive();
};