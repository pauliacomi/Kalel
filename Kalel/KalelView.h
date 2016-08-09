
// KalelView.h : interface of the CKalelView class
//

#pragma once

#include "afxwin.h"
#include <afxmt.h> // CEvent

#include "StringTable.h"				// All strings in the program

#include "DefinePostMessages.h"			// Definition of messages received from the automation functionality
#include "DefineStages.h"				// Stages/experiment type definitions

#include "Parametres.h"					// Functions for reading the parameters file
#include "ThreadManager.h"				// The threading functionality
#include "ExperimentData.h"				// Where data about the experimental parameters, results and current status is stored
#include "ExperimentSettings.h"			// Where returned data from results is stored
#include "ExperimentPropertySheet.h"	// Dialog box for setting experimental properties

// Dialog Box includes
#include "DialogMachineParameters.h"
#include "DialogConnectionPort.h"

#include "KalelDoc.h"
#include "Kalel.h"


class CKalelView : public CFormView
{

public:
	CKalelApp* pApp;				// pointer to the main app
	CKalelDoc* m_mainDocument;		// pointer to the main document
	ThreadManager* threadManager;

	// Storage for all the data
	ExperimentSettings * experimentSettings;
	ExperimentData * experimentData;

	// Some storage variables for each MFC control
	CEdit pEditMessages;
	CEdit pEditMesures;
	CString m_StrEditMessages;
	CString m_StrCalo;
	CString m_StrBassePression;
	CString m_StrHautePression;
	CString m_StrTemperatureCalo;
	CString m_StrTemperatureCage;
	CString m_StrTemperaturePiece;
	CString m_StrTemps;
	CButton m_ButtonLancer;
	CButton m_ButtonArreter;
	CString m_StrEditMesures;

	CString m_StrPressionInitiale;
	CString m_StrPressionFinale;
	CString m_StrEtape;

	CString m_StrTemoinVanne1;
	CString m_StrTemoinVanne2;
	CString m_StrTemoinVanne3;
	CString m_StrTemoinVanne4;
	CString m_StrTemoinVanne5;
	CString m_StrTemoinVanne6;
	CString m_StrTemoinVanne7;
	CString m_StrTemoinVanne8;
	CString m_StrTemoinEV1;
	CString m_StrTemoinEV2;
	CString m_StrTemoinPompe;

protected: // create from serialization only
	CKalelView();
	DECLARE_DYNCREATE(CKalelView)

public:
	enum { IDD = IDD_Kalel_FORM };

// Attributes
public:
	CKalelDoc* GetDocument() const;
	static CKalelView * GetView();


// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual void OnInitialUpdate(); // premier appel après la construction

// Implementation
public:
	virtual ~CKalelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	//------- KalelView

	void DoEvents(void);
	void OnMsvAmpoule(void);
	void OnMsvBouteille(void);
	void OnChangementBouteille(void);
	void DisplayPortDialog(void);
	void DisplayApparatusSettingsDialog(void);

	void GetExperimentData(ExperimentPropertySheet * dialogExperimentProperties, bool initialRequest);

	void UpdateButtons();


	// Thread callbacks
	LRESULT OnRegularThreadFinished(WPARAM wParam, LPARAM);

protected:

	//-------- KalelView_Affichage

public:
	LRESULT AffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT AffichageMessages(CString message);								// Temporary overload
	LRESULT RajoutAffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT AffichageMesures();
	LRESULT AffichageEtape();
	LRESULT RajoutAffichageEtape(WPARAM wParam, LPARAM lParam);
	LRESULT ExchangeData(WPARAM wParam, LPARAM lParam);
	LRESULT MessageBoxAlert(WPARAM wParam, LPARAM lParam);
	LRESULT MessageBoxConfirmation(WPARAM wParam, LPARAM);
	LRESULT MessageBoxStopExperiment(WPARAM wParam, LPARAM lParam);

	void OnTimer(UINT nIDEvent);	//timer for window update

	LRESULT GraphReset(WPARAM wparam, LPARAM lParam);

	//-------- KalelView_Boutons
public:
	void OnBnClickedLancer();
	void OnBnClickedArreter();
	LRESULT CancelBeforeStarting(WPARAM wParam, LPARAM lParam);

	void OnBnClickedButtonParametresExperience();
	void OnBnClickedArretSousVide();
	void OnBnClickedPause();
	void OnBnClickedProchaineCommande();
	void OnBnClickedProchaineDose();
	void OnBnClickedProchaineEtape();
	void OnBnClickedReprise();

	//-------- KalelView_Boutons_Vannes
public:
	void AskThreadForManualCommand(int instrument, int i, bool askToActivate);
	LRESULT OnThreadRequestButtonUpdate(WPARAM wParam, LPARAM lParam);

	void OnBnClickedOuvrir1();
	void OnBnClickedOuvrir2();
	void OnBnClickedOuvrir3();
	void OnBnClickedOuvrir4();
	void OnBnClickedOuvrir5();
	void OnBnClickedOuvrir6();
	void OnBnClickedOuvrir7();
	void OnBnClickedOuvrir8();

	void OnBnClickedFermer1();
	void OnBnClickedFermer2();
	void OnBnClickedFermer3();
	void OnBnClickedFermer4();
	void OnBnClickedFermer5();
	void OnBnClickedFermer6();
	void OnBnClickedFermer7();
	void OnBnClickedFermer8();

	void OnBnClickedActiverEV1();
	void OnBnClickedDesactiverEV1();
	void OnBnClickedActiverEV2();
	void OnBnClickedDesactiverEV2();
	void OnBnClickedActiverPompe();
	void OnBnClickedDesactiverPompe();
	afx_msg void OnBackgroundthreadStart();
	afx_msg void OnBackgroundthreadStop();
	afx_msg void OnBackgroundthreadRestart();
};


#ifndef _DEBUG  // debug version in KalelView.cpp
inline CKalelDoc* CKalelView::GetDocument() const
   { return reinterpret_cast<CKalelDoc*>(m_pDocument); }
#endif

