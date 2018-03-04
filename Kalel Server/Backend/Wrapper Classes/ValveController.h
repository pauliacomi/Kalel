//*****************************************************************************************************
//
//	This class extends the valve controller class
//	It is used as an easy-to-read interface that should make any instrument changes easy
//	by providing a guide for the functions to implement.
//
//*****************************************************************************************************

#pragma once

#include "../USB/NI_USB_6008.h"

#include <mutex>

class MachineSettings;

class ValveController :
	public NI_USB_6008
{
public:
	ValveController(int port);
	~ValveController(void);
	void Reset(MachineSettings & m);

	bool ValveOpen(int num, bool verbose);		// Open a valve
	bool ValveClose(int num, bool verbose);		// Close a valve

	bool EVActivate(int num, bool verbose);		// Activate EV
	bool EVDeactivate(int num, bool verbose);	// Deactivate EV

	bool PumpActivate(bool verbose);			// Activate Pump
	bool PumpDeactivate(bool verbose);			// Deactivate Pump

	bool CloseAll(bool verbose);				// Close everything
	bool CloseAllValves(bool verbose);			// Close all the valves
	bool CloseEVsAndPump(bool verbose);			// Close the Pump and EV's

	// Checks for activation

	bool ValveIsOpen(int num);
	bool EV1IsActive();
	bool EV2IsActive();
	bool PumpIsActive();

	// Get the port
	int GetReadPort();

	// Set the port
	void SetReadPort(int port);

private:
	std::mutex ctrlmutex;						// locks to prevent clash of threads
};