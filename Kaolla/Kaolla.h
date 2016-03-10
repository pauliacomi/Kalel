// Kaolla.h : fichier d'en-tête principal pour l'application Kaolla
//
#pragma once



#ifndef __AFXWIN_H__
	#error "incluez 'stdafx.h' avant d'inclure ce fichier pour PCH"
#endif

#include "resource.h"       // symboles principaux
#include "IncludeResources.h"


#include "Parametres_appareil.h"
#include "Connection_port.h"
#include "Donnees_Experience.h"


// CKaollaApp:
// Consultez Kaolla.cpp pour l'implémentation de cette classe
//

class CKaollaApp : public CWinApp
{
public:
	CKaollaApp();

	CParametres_appareil m_parametres_appareil;
	CConnection_port m_connection_ports;
	CDonnees_Experience m_donnees_experience;

	bool disponibilite_menu;

// Substitutions
public:
	virtual BOOL InitInstance();

// Implémentation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnParamatresAppareil();
	afx_msg void OnConnectionPorts();
	afx_msg void OnUpdateParamatresAppareil(CCmdUI *pCmdUI);

	afx_msg void OnUpdateConnectionPorts(CCmdUI *pCmdUI);
	afx_msg void OnDonneesExperience();
	afx_msg void OnUpdateDonneesExperience(CCmdUI *pCmdUI);
	afx_msg void OnMsvAmpoule();
	afx_msg void OnUpdateMsvAmpoule(CCmdUI *pCmdUI);
	afx_msg void OnMsvBouteille();
	afx_msg void OnUpdateMsvBouteille(CCmdUI *pCmdUI);
	afx_msg void OnChangementBouteille();
	afx_msg void OnUpdateChangementBouteille(CCmdUI *pCmdUI);
};

extern CKaollaApp theApp;