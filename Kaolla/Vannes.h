#pragma once
#ifndef _VANNE_H_
#define _VANNE_H_


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <stdio.h>
#include <tchar.h>

//#include "NIDAQmxBase.h"
#include "NIDAQmx.h"
#include "Parametres.h"
#include <stdio.h>
#include <stdlib.h>
#include "NI_USB_6008.h"


class CVannes :
	public NI_USB_6008
{
protected:

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
	bool ActiverEV1();
	bool ActiverEV2();
	bool ActiverPompe();
	//bool ActiverLaPompe();
	bool DesactiverEV1();
	bool DesactiverEV2();
	bool DesactiverPompe();
	//bool ArreterLaPompe();
	bool FermerLesValvesEtLaPompe();
	bool ToutFermer();
	//bool InitialisationDesPorts();
	int PortUSBVannes();
};

#endif