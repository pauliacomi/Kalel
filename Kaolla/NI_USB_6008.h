#pragma once
#ifndef NI_USB_6008_H_
#define NI_USB_6008_H_

#include "NIDAQmx.h"
#include <math.h>

#define ouvert 1
#define ferme  0

class NI_USB_6008
{
public:
	NI_USB_6008(void);
	NI_USB_6008(int dev);
	~NI_USB_6008(void);
	
	int DevNI_USB_6008;

	int etatPort0[8];
	int etatPort1[4];

	int GetDevNI_USB_6008();
	void SetDevNI_USB_6008(int dev);

	bool ActionPort0();
	bool ActionPort1();
	bool ActionDigital(char chan[], uInt32 w_data[]);

	bool OuvrirPort0(int num);
	bool FermerPort0(int num);
	bool OuvrirPort1(int num);
	bool FermerPort1(int num);

	bool FermerPort0Tous();
	bool OuvrirPort0Tous();
	bool FermerPort1Tous();
	bool OuvrirPort1Tous();

	bool EstOuvertPort0(int num);
	bool EstFermePort0(int num);
	bool EstOuvertPort1(int num);
	bool EstFermePort1(int num);

};

#endif