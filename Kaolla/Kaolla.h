
// Kaolla.h : main header file for the Kaolla application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"				// main symbols
#include "IncludeResources.h"		// secondary symbols

// CKaollaApp:
// See Kaolla.cpp for the implementation of this class
//

class CKaollaApp : public CWinApp
{
public:
	CKaollaApp();
	bool menuIsAvailable;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	// menu commands
	afx_msg void OnParamatresAppareil();
	afx_msg void OnConnectionPorts();
	afx_msg void OnDonneesExperience();
	afx_msg void OnMsvAmpoule();
	afx_msg void OnMsvBouteille();
	afx_msg void OnChangementBouteille();

	// update commands
	afx_msg void OnUpdateDonneesExperience(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMsvAmpoule(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMsvBouteille(CCmdUI *pCmdUI);
	afx_msg void OnUpdateParamatresAppareil(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConnectionPorts(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChangementBouteille(CCmdUI *pCmdUI);
};

extern CKaollaApp theApp;
