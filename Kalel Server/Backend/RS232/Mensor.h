// Class which reads the Mensors (Pressure Transmitters)
#ifndef _MENSOR__H_
#define _MENSOR__H_
#pragma once

#include "LiaisonRS232.h"

class Mensor :
	public LiaisonRS232
{
private:
	char buffer[256];
	bool connectionOpen = false;

public:
	Mensor(void);
	~Mensor(void);

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool ReadMensor(double* pression);

};
#endif
