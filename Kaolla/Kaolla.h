
// Kaolla.h : main header file for the Kaolla application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"				// main symbols
#include "StringTable.h"			// For all the strings in the program

// CKaollaApp:
// See Kaolla.cpp for the implementation of this class
//

class CKaollaApp : public CWinApp
{
public:
	CKaollaApp();

	// Global flags
	bool menuIsAvailable;
	bool experimentRunning;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

	// menu commands
	void OnParamatresAppareil();
	void OnConnectionPorts();
	void OnDonneesExperience();

	// update commands
	afx_msg void OnUpdateDonneesExperience(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMsvAmpoule(CCmdUI *pCmdUI);
	afx_msg void OnUpdateMsvBouteille(CCmdUI *pCmdUI);
	afx_msg void OnUpdateParamatresAppareil(CCmdUI *pCmdUI);
	afx_msg void OnUpdateConnectionPorts(CCmdUI *pCmdUI);
	afx_msg void OnUpdateChangementBouteille(CCmdUI *pCmdUI);
};

extern CKaollaApp theApp;
