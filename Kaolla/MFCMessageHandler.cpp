#include "stdafx.h"
#include "MFCMessageHandler.h"


MFCMessageHandler::MFCMessageHandler()
{
}


MFCMessageHandler::~MFCMessageHandler()
{
}

BOOL MFCMessageHandler::ResetDocumentGraph(LPVOID pParam)
{
	// Get vie handler and check for validity
	const HWND hViewFrame = reinterpret_cast<HWND>(pParam);
	ASSERT(hViewFrame != NULL);

	// Post the required message. The view should resolve the message to its parent document
	::PostMessage(hViewFrame, WM_RESETDOCUMENTGRAPH, NULL, NULL);

	return 0;
}

