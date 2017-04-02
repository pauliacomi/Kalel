// Class which reads the Mensors (Pressure Transmitters)
#ifndef _MENSOR__H_
#define _MENSOR__H_
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
#endif
