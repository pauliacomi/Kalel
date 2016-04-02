// Localisation.h : main header file for the Localisation DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLocalisationApp
// See Localisation.cpp for the implementation of this class
//

class CLocalisationApp : public CWinApp
{
public:
	CLocalisationApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
