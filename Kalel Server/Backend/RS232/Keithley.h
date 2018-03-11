#pragma once

#include "RS232.h"
#include "../Wrapper Classes/InstrumentInterface.h"

#include <mutex>

class Keithley :
	public RS232, InstrumentInterface
{

public:
	Keithley(int comport);
	~Keithley(void);

private:
	int portRS;										// RS232 port used

	std::mutex mutex_keithley;
	bool connectionOpen			= false;
	bool readingPort			= false;

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool InitKeithley();
	bool ReadChan(int chanNo, double* result);

public:

	void SetComPort(int nId) override;
	int GetComPort() override;

	double Read(unsigned int channel) override;
};
