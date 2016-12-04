#ifndef _VANNE_H_
#define _VANNE_H_
#pragma once


#include <stdio.h>
#include <tchar.h>

#include "../USB/NI_USB_6008.h"
#include "../../Parameters/Parametres.h"

class MessageHandler;

class ValveController :
	public NI_USB_6008
{
public:
	ValveController(MessageHandler & messageHandler);
	~ValveController(void);

	bool ValveOpen(int num, bool verbose);		// Open a valve
	bool ValveClose(int num, bool verbose);		// Close a valve

	bool ValveOpenClose(int num, bool verbose); // Open, then close a valve

	bool EVActivate(int num, bool verbose);		// Activate EV
	bool EVDeactivate(int num, bool verbose);	// Deactivate EV

	bool PumpActivate(bool verbose);			// Activate Pump
	bool PumpDeactivate(bool verbose);			// Deactivate Pump

	bool CloseAll(bool verbose);				// Close everything
	bool CloseAllValves(bool verbose);			// Close all the valves
	bool CloseEVsAndPump(bool verbose);			// Close the Pump and EV's

	// Checks for activation

	bool VanneEstOuvert(int num);
	bool VanneEstFerme(int num);
	bool EV1EstActive();
	bool EV1EstDesactive();
	bool EV2EstActive();
	bool EV2EstDesactive();
	bool PompeEstActive();
	bool PompeEstDesactive();

	// Get the USB port
	int PortUSB();

private:
	MessageHandler & messageHandler;
};

#endif