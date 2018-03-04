// Class which reads the Mensors (Pressure Transmitters)
#pragma once

#include "RS232.h"

class Mensor :
	public RS232
{
public:
	Mensor(void);
	Mensor(int comport);
	~Mensor(void);

private:
	char buffer[256]	= { "\0" };
	bool connectionOpen = false;
public:
	bool OpenCOM(int nId);
	bool CloseCOM();
	bool ChangeCOM(int nId);
	double ReadMensor();

};
