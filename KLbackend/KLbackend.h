// KLbackend.h : main header file for the KLbackend DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CKLbackendApp
// See KLbackend.cpp for the implementation of this class
//

class CKLbackendApp : public CWinApp
{
public:
	CKLbackendApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
