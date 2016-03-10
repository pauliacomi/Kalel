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
	std::string message;

public:
	Mensor(void);
	~Mensor(void);
	bool OpenCOM(int nId);
	bool OpenCOM(int nId,std::string* info);
	bool CloseCOM();
	bool CloseCOM(std::string* info);
	bool ReadMensor(double* pression);
	bool ReadMensor(double* pression, std::string* info);
	double ReadMensor();
	std::string getMessage();
};
#endif
