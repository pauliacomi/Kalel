// This class is used to make the interaction between threads and MFC-derived objects 
// as smooth as possible. All messages will be defined in the header and then will be 
// used to sent PostMessage commends to the specified window

#pragma once
#ifndef MFCMESSAGEHANDLER_H
#define MFCMESSAGEHANDLER_H

// Message definitions
#include "DefinePostMessages.h"


class MFCMessageHandler
{
	// Constructor and destructor
public:
	MFCMessageHandler();
	virtual ~MFCMessageHandler();
	
	// Window handle for message postback
	HWND windowHandle;

public:

	// PostMessage functions
	BOOL ResetDocumentGraph(LPVOID pParam);
	BOOL DisplayMeasurement(CString pParam);
	BOOL DisplayMessage(CString pParam);
	BOOL DisplayAddMessage(CString pParam);

	// Steps
	BOOL DisplayStep(CString pParam);
	BOOL DisplaAddStep(CString pParam);
	BOOL DisplayPreviousStep();

	// Data
	BOOL DisplayCalorimeter();
	BOOL DisplayHighPressure();
	BOOL DisplayLowPressure();
	BOOL DisplayTemperatures();

	// Messageboxes
	BOOL DisplayMessageBox(CString message, UINT nType);

};


#endif // !MFCMESSAGEHANDLER_H