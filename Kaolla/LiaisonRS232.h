#pragma once

#ifndef _LIAISONRS232__H_
#define _LIAISONRS232__H_
#include <windows.h>

class LiaisonRS232
{
public:
	LiaisonRS232();
	~LiaisonRS232();
	bool OpenCOM(int nId);
	// bool ReadCOM(void* buffer, int nBytesToRead, int* pBytesRead);
	int ReadCOM(char* buffer, int nBytesToRead);
	bool WriteCOM(void* buffer, int nBytesToWrite, int* pBytesWritten);
	bool CloseCOM();
protected:
	HANDLE g_hCOM;
	COMMTIMEOUTS g_cto;
	DCB g_dcb;
};
#endif