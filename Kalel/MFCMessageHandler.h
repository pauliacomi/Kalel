// This class is used to make the interaction between threads and MFC-derived objects 
// as smooth as possible. All messages will be defined in the header and then will be 
// used to sent PostMessage commends to the specified window

#pragma once
#ifndef MFCMESSAGEHANDLER_H
#define MFCMESSAGEHANDLER_H

#define default_val -1	// This value is used as the default value for the optional parameters of the message display function. 
						// Make sure that it is set to a value that the actual parameters can never take

// Required to pass the experimental data to the main GUI
#include "ExperimentData.h"		

class MFCMessageHandler
{
	// Constructor and destructor
public:
	MFCMessageHandler();
	virtual ~MFCMessageHandler();
	
	// Window handle for message postback
	HWND windowHandle;

public:
	// Set the handle
	BOOL SetHandle(HWND h);

	// PostMessage functions
	void ExperimentStart();
	void ExperimentEnd();
	void GraphReset();
	BOOL ExchangeData(ExperimentData pParam);
	BOOL DisplayMessage(int pParam, int pInt1 = default_val, int pInt2 = default_val, double pDouble = default_val);								// Simple display message which takes an int

	BOOL DisplayMessage(int pParam, CString m);

	// Messageboxes
	BOOL DisplayMessageBox(int message, UINT nType, bool blocksProgram, double pDouble1 = default_val, double pDouble2 = default_val);
};


#endif // !MFCMESSAGEHANDLER_H