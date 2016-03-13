#pragma once
#ifndef _VANNE_H_
#define _VANNE_H_


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

#include "NI_USB_6008.h"

#include "Parametres.h"


class CVannes :
	public NI_USB_6008
{
public:
	CVannes(void);
	~CVannes(void);

	bool VanneEstOuvert(int num);
	bool VanneEstFerme(int num);
	bool EV1EstActive();
	bool EV1EstDesactive();
	bool EV2EstActive();
	bool EV2EstDesactive();
	bool PompeEstActive();
	bool PompeEstDesactive();
	bool Ouvrir(int num);
	bool Fermer(int num);
	bool FermerToutesLesVannes();
	bool ActiverEV(int num);
	bool ActiverPompe();
	bool DesactiverEV(int num);
	bool DesactiverPompe();
	bool FermerLesValvesEtLaPompe();
	bool ToutFermer();
	int PortUSBVannes();
};

#endif