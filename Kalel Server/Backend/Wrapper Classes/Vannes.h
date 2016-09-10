#ifndef _VANNE_H_
#define _VANNE_H_
#pragma once


#include <stdio.h>
#include <tchar.h>

#include "../USB/NI_USB_6008.h"
#include "Parametres.h"


class CVannes :
	public NI_USB_6008
{
public:
	CVannes(void);
	~CVannes(void);

	bool Ouvrir(int num);				// Open a valve
	bool Fermer(int num);				// Close a valve

	bool ActiverEV(int num);			// Activate EV
	bool DesactiverEV(int num);			// Deactivate EV

	bool ActiverPompe();				// Activate Pump
	bool DesactiverPompe();				// Deactivate Pump

	bool ToutFermer();					// Close everything
	bool FermerToutesLesVannes();		// Close all the valves
	bool FermerLesEVEtLaPompe();		// Close the Pump and EV's

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
};

#endif