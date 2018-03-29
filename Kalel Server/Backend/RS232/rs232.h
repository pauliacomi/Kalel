// This class makes the link to the serial port of the computer.
// It is used by any serial instrument class as an extension
//
// This class uses an OVERLAPPED means of RS232 communications
// which means that it is therefore safely accessed by multiple 
// threads

#pragma once

#include <windows.h>

class RS232
{
public:
	RS232();
	~RS232();

	// Open the COM port
	bool OpenCOM(int nId);

	// Read the COM port
	bool ReadCOM(char* buffer, int nBytesToRead);

	// Write to the COM port
	bool WriteCOM(const char* buffer, int nBytesToWrite, int* pBytesWritten);

	// Close the COM port, must always be called to make sure the handle is closed
	bool CloseCOM();

protected:
	HANDLE g_hCOM;
	COMMTIMEOUTS g_cto;
	DCB g_dcb;
};
