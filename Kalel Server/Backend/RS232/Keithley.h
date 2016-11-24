#ifndef _KEITHLEY__H_
#define _KEITHLEY__H_
#pragma once

#include "LiaisonRS232.h"

#include <string>
#include <windows.h>


class Keithley :
	public LiaisonRS232
{

public:
	Keithley(void);
	~Keithley(void);

private:
	CRITICAL_SECTION Sync_keithley;
	bool connectionOpen = false;
	bool readingPort = false;

public:

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool InitKeithley();

	bool ReadChannel(int chanNo, double* result);

	bool ReadChannel1(double* result); 
	bool ReadChannel2(double* result);
};
#endif

