#pragma once

#ifndef _KEITHLEY__H_
#define _KEITHLEY__H_
#include "LiaisonRS232.h"
#include <string>
#include <windows.h>



class Keithley :
	public LiaisonRS232
{

public:
	Keithley(void);
	~Keithley(void);

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool InitKeithley();

	// Channel 1 functions

	bool ReadChannel1(std::string* result);
	bool ReadChannel1(double* resultat);
	bool ReadChannel1(std::string* result, double* resultat);
	std::string ReadChannel1();
	double ReadChannel1_double();

	// Channel 2 functions

	bool ReadChannel2(std::string* result);
	bool ReadChannel2(double* resultat);
	bool ReadChannel2(std::string* result, double* resultat);
	std::string ReadChannel2();
	double ReadChannel2_double();


	double Conversion(std::string resultat);
};
#endif

