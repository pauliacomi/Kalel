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
	std::string message;						// Error/info storage

public:
	Mensor(void);
	~Mensor(void);

	bool OpenCOM(int nId);						// Regular function
	bool OpenCOM(int nId, std::string* info);	// Function overload to get error message as well

	bool CloseCOM();							// Regular function
	bool CloseCOM(std::string* info);			// Function overload to get error message as well
	
	bool ReadMensor(double* pression);							// Regular function
	bool ReadMensor(double* pression, std::string* info);		// Function overload to get error message as well
	double ReadMensor();

	std::string getMessage();					// Get error message
};
#endif
