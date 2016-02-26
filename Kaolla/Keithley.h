#pragma once

#ifndef _KEITHLEY__H_
#define _KEITHLEY__H_
#include "LiaisonRS232.h"
#include <string>
#include <windows.h>



class Keithley :
	public LiaisonRS232
{
private:
	std::string message;

public:
	Keithley(void);
	~Keithley(void);

	bool OpenCOM(int nId);
	bool OpenCOM(int nId,std::string* info);
	bool CloseCOM();
	bool CloseCOM(std::string* info);
	bool InitKeithley();
	bool InitKeithley(std::string* info);

	bool ReadChannel1(std::string* result);
	bool ReadChannel1(std::string* result, std::string* info);
	bool ReadChannel1(double* resultat);
	bool ReadChannel1(double* resultat, std::string* info);
	bool ReadChannel1(std::string* result, double* resultat);
	bool ReadChannel1(std::string* result, double* resultat, std::string* info);
	std::string ReadChannel1();
	double ReadChannel1_double();


	bool ReadChannel2(std::string* result);
	bool ReadChannel2(std::string* result, std::string* info);
	bool ReadChannel2(double* resultat);
	bool ReadChannel2(double* resultat, std::string* info);
	bool ReadChannel2(std::string* result, double* resultat);
	bool ReadChannel2(std::string* result, double* resultat, std::string* info);
	std::string ReadChannel2();
	double ReadChannel2_double();

	/*
	void ReadChannel2();
	void ReadChannel2Console();
	*/

	std::string getMessage();
	double Conversion(std::string resultat);
};
#endif

