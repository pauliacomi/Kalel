#pragma once
#ifndef NI_USB_6008_H_
#define NI_USB_6008_H_

#include "NIDAQmx.h"
#include <math.h>
#include <string>

#define ouvert 1
#define ferme  0

class NI_USB_6008
{
public:
	NI_USB_6008(void);
	NI_USB_6008(int dev);
	~NI_USB_6008(void);
	
	int DevNI_USB_6008;								// USB port

	int etatPort0[8];								// Array for keeping port states
	int etatPort1[4];								// Array for keeping port states


	int GetDevNI_USB_6008();						// Get USB port
	void SetDevNI_USB_6008(int dev);				// Set USB port

private:
	bool ActionPort0();
	bool ActionPort1();
	bool ActionDigital(char chan[], uInt32 w_data[]);
	std::string errorKeep;

public:
	
	// Functions to open / close a particular bit from a port or all bits

	bool OuvrirPort0(int num);
	bool FermerPort0(int num);
	bool OuvrirPort1(int num);
	bool FermerPort1(int num);

	bool FermerPort0Tous();
	bool OuvrirPort0Tous();
	bool FermerPort1Tous();
	bool OuvrirPort1Tous();

	// Check for open/closed functions

	bool EstOuvertPort0(int num);
	bool EstFermePort0(int num);
	bool EstOuvertPort1(int num);
	bool EstFermePort1(int num);

	// Error retrieval

	void GetError(std::string* err);
};

#endif