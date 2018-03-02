#ifndef _KEITHLEY__H_
#define _KEITHLEY__H_
#pragma once

#include "RS232.h"

#include <mutex>


class Keithley :
	public RS232
{

public:
	Keithley(void);
	Keithley(int comport);
	~Keithley(void);

private:
	std::mutex mutex_keithley;
	bool connectionOpen			= false;
	bool readingPort			= false;

public:

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool ChangeCOM(int nId);
	bool InitKeithley();

	bool ReadChannel(int chanNo, double* result);

	double ReadChannel(int chanNo);
	double ReadChannel1();
	double ReadChannel2();
};
#endif

