#include "StdAfx.h"
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <iostream>
#include <string.h>
#include "LiaisonRS232.h"

using namespace std;

#define RX_SIZE         4096    /* taille tampon d'entrée  */
#define TX_SIZE         4096    /* taille tampon de sortie */
#define MAX_WAIT_READ   500		/* temps max d'attente pour lecture (en ms) */


LiaisonRS232::LiaisonRS232()
{
	// Null out the handle
	g_hCOM = NULL;

	//configuration du temps d'attente
	g_cto.ReadIntervalTimeout = MAX_WAIT_READ;
	g_cto.ReadTotalTimeoutMultiplier = 0;
	g_cto.ReadTotalTimeoutConstant = MAX_WAIT_READ;
	g_cto.WriteTotalTimeoutMultiplier = 0;
	g_cto.WriteTotalTimeoutConstant = 0;

	//Configuration du Port
	g_dcb.DCBlength = sizeof(DCB);
	g_dcb.BaudRate = 9600;
	g_dcb.fBinary = true;
	g_dcb.fParity = false;
	g_dcb.fOutxCtsFlow = false;
	g_dcb.fOutxDsrFlow = false;
	g_dcb.fDtrControl = DTR_CONTROL_ENABLE;
	g_dcb.fDsrSensitivity = false;
	g_dcb.fTXContinueOnXoff = false;
	g_dcb.fOutX = false;
	g_dcb.fInX = false;
	g_dcb.fErrorChar = false;
	g_dcb.fNull = false;
	g_dcb.fRtsControl = RTS_CONTROL_ENABLE;
	g_dcb.fAbortOnError = false;
	g_dcb.fDummy2 = 0;
	g_dcb.wReserved = 0;
	g_dcb.XonLim = 0x100;
	g_dcb.XoffLim = 0x100;
	g_dcb.ByteSize = 8;
	g_dcb.Parity = NOPARITY;
	g_dcb.StopBits = ONESTOPBIT;
	g_dcb.XonChar = 0x11;
	g_dcb.XoffChar = 0x13;
	g_dcb.ErrorChar = '?';
	g_dcb.EofChar = 0x1A;
	g_dcb.EvtChar = 0x10;
}

LiaisonRS232::~LiaisonRS232()
{
}

bool LiaisonRS232::OpenCOM(int pnId)
{
	// Construct port specifier
	char szCOM[5];
    sprintf_s(szCOM, "COM%d", pnId);
	
	// Open COM port
    g_hCOM = CreateFile(
		szCOM,									// Port specifier 
		GENERIC_READ|GENERIC_WRITE,				// Access mode
		0,										//
		NULL,									// security  (None)
		OPEN_EXISTING,							// creation : open_existing
		FILE_ATTRIBUTE_SYSTEM,					//
		NULL									// no templates file for COM poort
	);

	// Error check
    if(g_hCOM == INVALID_HANDLE_VALUE)
    {
        errorKeep = "Error opening port COM" + to_string(pnId);
        return false;
    }

    /* affectation taille des tampons d'émission et de réception */
    SetupComm(g_hCOM, RX_SIZE, TX_SIZE);

    // Configure port
    if(!SetCommTimeouts(g_hCOM, &g_cto) || !SetCommState(g_hCOM, &g_dcb))
    {
		errorKeep = "Error configuring port COM" + to_string(pnId);
        CloseHandle(g_hCOM);
        return false;
    }

    /* on vide les tampons d'émission et de réception, mise à 1 DTR */
    PurgeComm(g_hCOM, PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
    EscapeCommFunction(g_hCOM, SETDTR);

    return true;
}


int LiaisonRS232::ReadCOM(char *buffer, int nBytesToRead)
{
	DWORD result;
	ReadFile(g_hCOM, buffer, nBytesToRead, &result, NULL);

	if(result==0)//test de la lecture du port
	{
		return -1;
	}
	else
	{
		buffer[result]='\0';
		return (result+1);
	}
}

bool LiaisonRS232::WriteCOM(void* buffer, int nBytesToWrite, int* pBytesWritten)
{
    return WriteFile(g_hCOM, buffer, nBytesToWrite, (LPDWORD)pBytesWritten, NULL);
}

bool LiaisonRS232::CloseCOM()
{
	if (g_hCOM) {
		CloseHandle(g_hCOM);
		g_hCOM = NULL;
	}
    return true;
}

void LiaisonRS232::GetError(std::string* err)
{
	*err = errorKeep;
}