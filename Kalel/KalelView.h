#pragma once

#include "afxwin.h"

#include "Resources/DefineStages.h"									// Stages/experiment type definitions
#include "Resources/StringTable.h"									// Error message strings

#include "Com Classes/ExperimentData.h"								// Where data about the experimental parameters, results and current status is stored
#include "Com Classes/ExperimentSettings.h"							// Where returned data from results is stored
#include "Com Classes/MachineSettings.h"							// Where the settings for the connected machine are stored
#include "Parametres.h"												// Parameters file read/write
#include "CommHandler.h"											// Responsible for all communication to/from client

#include "KalelDoc.h"
#include "Kalel.h"

// Forward declarations
class ExperimentPropertySheet;

class CKalelView : public CFormView
{

public:
	CKalelApp* pApp;				// pointer to the main app
	CKalelDoc* m_mainDocument;		// pointer to the main document

	Parameters savedParams;

	// Storage for all the data
	ExperimentSettings * experimentSettings;
	ExperimentData * experimentData;
	std::shared_ptr<MachineSettings> machineSettings;
	std::shared_ptr<MachineSettings> tempSettings;
	CommHandler commHandler;

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

protected:
	//------- KalelView

	void DoEvents(void);
	void GetExperimentData(ExperimentPropertySheet * dialogExperimentProperties, bool initialRequest);
	void UpdateButtons();

	// Menu messages

	LRESULT OnMsvAmpoule(WPARAM wParam, LPARAM lParam);
	LRESULT OnMsvBouteille(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangementBouteille(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayConnectDialog(WPARAM, LPARAM);
	LRESULT DisplayPortDialog(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayApparatusSettingsDialog(WPARAM wParam, LPARAM lParam);

	// Thread callbacks

	LRESULT OnServerConnected(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMachineSettings(WPARAM wParam, LPARAM lParam);
	LRESULT OnSync(WPARAM wParam, LPARAM lParam);
	LRESULT OnExchangeData(WPARAM wParam, LPARAM lParam);
	LRESULT OnRegularThreadFinished(WPARAM wParam, LPARAM);
	LRESULT CancelBeforeStarting(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadStart(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadStop(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadRestart(WPARAM wParam, LPARAM lParam);

protected:

	//-------- KalelView_Affichage

protected:
	void OnTimer(UINT nIDEvent);	//timer for window update

	LRESULT AffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT AffichageMesures();
	LRESULT AffichageEtape();
	LRESULT MessageBoxAlert(WPARAM wParam, LPARAM lParam);
	LRESULT MessageBoxConfirmation(WPARAM wParam, LPARAM);
	LRESULT GraphReset(WPARAM wparam, LPARAM lParam);

	
	//
	//-------- KalelView_Boutons
	//

protected:
	void OnBnClickedLancer();
	void OnBnClickedArreter();
	void OnBnClickedButtonParametresExperience();
	void OnBnClickedArretSousVide();
	void OnBnClickedPause();
	void OnBnClickedProchaineCommande();
	void OnBnClickedProchaineDose();
	void OnBnClickedProchaineEtape();
	void OnBnClickedReprise();

	//
	//-------- KalelView_Boutons_Vannes
	//

protected:
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

};


#ifndef _DEBUG  // debug version in KalelView.cpp
inline CKalelDoc* CKalelView::GetDocument() const
   { return reinterpret_cast<CKalelDoc*>(m_pDocument); }
#endif

