// Class which reads the Mensors (Pressure Transmitters)

#pragma once

#ifndef _MENSOR__H_
#define _MENSOR__H_
#include "LiaisonRS232.h"
#include <string>

class Mensor :
	public LiaisonRS232
{
private:
	char buffer[256];
	CRITICAL_SECTION Sync_mensor;

public:
	Mensor(void);
	~Mensor(void);

	bool OpenCOM(int nId);
	bool CloseCOM();
	bool ReadMensor(double* pression);

};
#endif
