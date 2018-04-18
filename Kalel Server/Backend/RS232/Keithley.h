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

	const char * mens_cls = "*CLS\n";
	const char * mens_rst = "*RST\n";
	const char * mens_chan1_lpas = ":SENS:VOLT:CHAN1:LPAS OFF\n";
	const char * mens_chan1_dfil = ":SENS:VOLT:CHAN1:DFIL:STAT OFF\n";
	const char * mens_chan2_lpas = ":SENS:VOLT:CHAN2:LPAS OFF\n";
	const char * mens_chan2_dfil = ":SENS:VOLT:CHAN2:DFIL:STAT OFF\n";
	const char * mens_read = ":READ?\n";

	const char query_template[15] = ":SENS:CHAN %d\n";

	bool connectionOpen			= false;

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool InitKeithley();
	bool ReadChan(int chanNo, double* result);

public:

	void SetComPort(int nId) override;
	int GetComPort() override;

	double Read(unsigned int channel) override;
};
