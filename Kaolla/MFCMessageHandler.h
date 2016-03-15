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
	
	// PostMessage functions
public:
	BOOL ResetDocumentGraph(LPVOID pParam);
};


#endif // !MFCMESSAGEHANDLER_H