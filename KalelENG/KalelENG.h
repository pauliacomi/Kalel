// KalelENG.h : main header file for the KalelENG DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CKalelENGApp
// See KalelENG.cpp for the implementation of this class
//

class CKalelENGApp : public CWinApp
{
public:
	CKalelENGApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
