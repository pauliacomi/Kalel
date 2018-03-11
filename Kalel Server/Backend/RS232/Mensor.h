// Class which reads the Mensors (Pressure Transmitters)
#pragma once

#include "RS232.h"
#include "../Wrapper Classes/InstrumentInterface.h"

class Mensor :
	public RS232, InstrumentInterface
{
public:
	Mensor(int comport);
	~Mensor(void);

private:
	int portRS;										// RS232 port used

	char buffer[256]	= { "\0" };
	bool connectionOpen = false;
	bool OpenCOM(int nId);
	bool CloseCOM();

public:

	void SetComPort(int nId) override;
	int GetComPort() override;

	double Read() override;
};
