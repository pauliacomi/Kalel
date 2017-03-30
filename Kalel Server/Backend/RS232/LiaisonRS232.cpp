#include "LiaisonRS232.h"

#include <iostream>

#define RX_SIZE         4096    /* taille tampon d'entrée  */
#define TX_SIZE         4096    /* taille tampon de sortie */
#define MAX_WAIT_READ   200		/* temps max d'attente pour lecture (en ms) */
#define MAX_WAIT_WRITE  200		/* temps max d'attente pour lecture (en ms) */


LiaisonRS232::LiaisonRS232()
{
	// Null out the handle
	g_hCOM = NULL;

	// Configuration of timeouts
	g_cto.ReadIntervalTimeout			= MAX_WAIT_READ;
	g_cto.ReadTotalTimeoutMultiplier	= 0;
	g_cto.ReadTotalTimeoutConstant		= MAX_WAIT_READ;
	g_cto.WriteTotalTimeoutMultiplier	= 0;
	g_cto.WriteTotalTimeoutConstant		= 0;

	// Configuration of port
	g_dcb.DCBlength			= sizeof(DCB);
	g_dcb.BaudRate			= 9600;
	g_dcb.fBinary			= true;
	g_dcb.fParity			= false;
	g_dcb.fOutxCtsFlow		= false;
	g_dcb.fOutxDsrFlow		= false;
	g_dcb.fDtrControl		= DTR_CONTROL_ENABLE;
	g_dcb.fDsrSensitivity	= false;
	g_dcb.fTXContinueOnXoff = false;
	g_dcb.fOutX				= false;
	g_dcb.fInX				= false;
	g_dcb.fErrorChar		= false;
	g_dcb.fNull				= false;
	g_dcb.fRtsControl		= RTS_CONTROL_ENABLE;
	g_dcb.fAbortOnError		= false;
	g_dcb.fDummy2			= 0;
	g_dcb.wReserved			= 0;
	g_dcb.XonLim			= 0x100;
	g_dcb.XoffLim			= 0x100;
	g_dcb.ByteSize			= 8;
	g_dcb.Parity			= NOPARITY;
	g_dcb.StopBits			= ONESTOPBIT;
	g_dcb.XonChar			= 0x11;
	g_dcb.XoffChar			= 0x13;
	g_dcb.ErrorChar			= '?';
	g_dcb.EofChar			= 0x1A;
	g_dcb.EvtChar			= 0x10;
}

LiaisonRS232::~LiaisonRS232()
{
}

bool LiaisonRS232::OpenCOM(int pnId)
{
	// Construct port specifier
	std::wstring szCOM = L"\\\\.\\COM%d" + std::to_wstring(pnId);
	
	// Open COM port
    g_hCOM = CreateFile(
		szCOM.c_str(),											// Port specifier 
		GENERIC_READ|GENERIC_WRITE,								// Access mode
		0,														// Sharing: 0 as COM ports cannot be shared
		NULL,													// Security: None
		OPEN_EXISTING,											// Creation : open_existing
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED,			// Flag: overlapped operation
		NULL													// No templates file for COM port
	);

	// Error check
    if(g_hCOM == INVALID_HANDLE_VALUE)
    {
        errorKeep = "Error opening port COM" + std::to_string(pnId);
		g_hCOM = NULL;
        return false;
    }

    // Configure port
    if(!SetCommTimeouts(g_hCOM, &g_cto) || !SetCommState(g_hCOM, &g_dcb))
    {
		errorKeep = "Error configuring port COM" + std::to_string(pnId);
        CloseHandle(g_hCOM);
        return false;
    }

	// Set buffer sizes
	SetupComm(g_hCOM, RX_SIZE, TX_SIZE);

    // Purge the buffer to make sure everything is empty
    PurgeComm(g_hCOM, PURGE_TXCLEAR|PURGE_RXCLEAR|PURGE_TXABORT|PURGE_RXABORT);
	
	// Raise DTR
    EscapeCommFunction(g_hCOM, SETDTR);

    return true;
}


bool LiaisonRS232::ReadCOM(char *buffer, int nBytesToRead)
{
	// Start by checking if port is open
	if (!g_hCOM) {
		errorKeep = "Port must be opened first";
		return false;
	}

	// Now define required variables
	DWORD dwRead;
	DWORD dwRes;
	BOOL fWaitingOnRead = FALSE;
	OVERLAPPED osReader = { 0 };
	bool noErrors = true;
	bool readingComplete = false;

	// Create the overlapped event. Must be closed before exiting to avoid a handle leak.
	osReader.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osReader.hEvent == NULL) {
		// Error creating overlapped event; abort.
		errorKeep = "Error creating COM read overlapped event";
		noErrors = false;
	}

	/*
	REQUEST READ
	*/
	// If not already reading
	if (!fWaitingOnRead && noErrors) {
		// Issue read operation.
		bool ok = ::ReadFile(	g_hCOM, 		  // Handle
								buffer, 		  // Where to write the data
								nBytesToRead, 	  // Amount to read
								&dwRead, 		  // Amount actually read
								&osReader);		  // Overlapped event

		if (!ok) {
			if (GetLastError() != ERROR_IO_PENDING) {	// read not delayed?					
				errorKeep = "Error issuing read command";
				noErrors = false;
			}
			else
				fWaitingOnRead = TRUE;
		}
		else {
			// read completed immediately
			readingComplete = true;
		}
	}

	/*
	WAIT TO READ
	*/
	// Wait for pending read to complete
	if (fWaitingOnRead && noErrors && !readingComplete) {
		dwRes = WaitForSingleObject(osReader.hEvent, MAX_WAIT_READ);
		switch (dwRes)
		{
			// Read completed.
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(g_hCOM, &osReader, &dwRead, FALSE)) {
					if (GetLastError() == ERROR_OPERATION_ABORTED)
						errorKeep = "Read aborted";
					else
						errorKeep = "GetOverlappedResult (in Reader)";

					noErrors = false;
				}
				else
					// Read completed successfully.
					readingComplete = true;

				//  Reset flag so that another opertion can be issued.
				fWaitingOnRead = FALSE;
				break;

			case WAIT_TIMEOUT:
				//
				// timeouts are not reported because they happen too often
				// errorKeep = "Timeout in Reader & Status checking";
				//
				break;

			default:
				// Error in the WaitForSingleObject; abort.
				errorKeep = "WaitForMultipleObjects reader";
				noErrors = false;
				break;
		}
	}

	/*
	END
	*/
	// close event handle
	CloseHandle(osReader.hEvent);

	if (!readingComplete){

	}

	if (noErrors) {
		return true;
	}
	return false;
}

bool LiaisonRS232::WriteCOM(void* buffer, int nBytesToWrite, int* pBytesWritten)
{
	// Start by checking if port is open
	if (!g_hCOM) {
		errorKeep = "Port must be opened first";
		return false;
	}

	// Now define required variables
	OVERLAPPED osWrite = { 0 };
	DWORD dwRes;
	bool fRes;

	// Create this write operation's OVERLAPPED structure's hEvent.
	osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (osWrite.hEvent == NULL) {
		// Error creating overlapped event; abort.
		errorKeep = "Error creating COM write overlapped event";
		return false;
	}


	// Issue write.
	bool ok = ::WriteFile(	g_hCOM, 					   // Handle
							buffer, 					   // Data to write
							nBytesToWrite,				   // Length of data
							(LPDWORD)pBytesWritten,		   // Amount written
							&osWrite);					   // Overlapped event

	if (!ok) {
		if (GetLastError() != ERROR_IO_PENDING) {
			// WriteFile failed, but isn't delayed. Report error and abort.
			errorKeep = "Error writing to COM";
			fRes = false;
		}
		else {
			// Write is pending.
			dwRes = WaitForSingleObject(osWrite.hEvent, MAX_WAIT_WRITE);
			switch (dwRes)
			{
				// OVERLAPPED structure's event has been signaled. 
			case WAIT_OBJECT_0:
				if (!GetOverlappedResult(g_hCOM, &osWrite, (LPDWORD)pBytesWritten, FALSE)) {
					errorKeep = "Error writing to COM";
					fRes = false;
				}
				else
					// Write operation completed successfully.
					fRes = true;
				break;

			default:
				// An error has occurred in WaitForSingleObject.
				// This usually indicates a problem with the
				// OVERLAPPED structure's event handle.
				errorKeep = "WaitForMultipleObjects writer";
				fRes = false;
				break;
			}
		}
	}

	else {
		// WriteFile completed immediately.
		fRes = true;
	}

	CloseHandle(osWrite.hEvent);
	return fRes;
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