
// Kalel.h : main header file for the Kalel application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"				// main symbols
#include "StringTable.h"			// For all the strings in the program

// CKalelApp:
// See Kalel.cpp for the implementation of this class
//

class CKalelApp : public CWinAppEx
{
public:
	CKalelApp();

	// Global flags
	bool menuIsAvailable;
	bool experimentRunning;

// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	void DisplayPortDialog();
	void DisplayApparatusSettingsDialog();
	void OnMsvAmpoule();
	void OnMsvBouteille();
	DECLARE_MESSAGE_MAP()

	void OnChangementBouteille();

	void OnBackgroundthreadStart();
	void OnBackgroundthreadStop();
	void OnBackgroundthreadRestart();

	// update commands
	void OnUpdateDonneesExperience(CCmdUI *pCmdUI);
	void OnUpdateMsvAmpoule(CCmdUI *pCmdUI);
	void OnUpdateMsvBouteille(CCmdUI *pCmdUI);
	void OnUpdateParamatresAppareil(CCmdUI *pCmdUI);
	void OnUpdateConnectionPorts(CCmdUI *pCmdUI);
	void OnUpdateChangementBouteille(CCmdUI *pCmdUI);
	void OnUpdateBackgroundthreadStart(CCmdUI * pCmdUI);
	void OnUpdateBackgroundthreadStop(CCmdUI * pCmdUI);
	void OnUpdateBackgroundthreadRestart(CCmdUI * pCmdUI);
};

extern CKalelApp theApp;
