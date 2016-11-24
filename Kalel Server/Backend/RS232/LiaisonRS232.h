// This class makes the link to the serial port of the computer.
// It is used by any serial instrument class as an extension

#ifndef _LIAISONRS232__H_
#define _LIAISONRS232__H_
#pragma once

#include <windows.h>
#include <string>

class LiaisonRS232
{
public:
	LiaisonRS232();
	~LiaisonRS232();

	// Open the COM port
	bool OpenCOM(int nId);

	// Read the COM port
	bool ReadCOM(char* buffer, int nBytesToRead);

	// Write to the COM port
	bool WriteCOM(void* buffer, int nBytesToWrite, int* pBytesWritten);

	// Close the COM port, must always be called to make sure the handle is closed
	bool CloseCOM();

	// Error reporting
	void GetError(std::string * err);

protected:
	HANDLE g_hCOM;
	COMMTIMEOUTS g_cto;
	DCB g_dcb;
	std::string errorKeep;
};
#endif