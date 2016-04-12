
// KaollaView.h : interface of the CKaollaView class
//

#pragma once

#include "afxwin.h"
#include <afxmt.h> // CEvent

#include "DefinePostMessages.h"

#include "Parametres.h"
#include "ThreadManager.h"				// The threading functionality
#include "ExperimentData.h"				// Where data about the experimental parameters, results and current status is stored
#include "ExperimentPropertySheet.h"	// Dialog box for setting experimental properties


class CKaollaView : public CFormView
{

public:
	// pointer to the main document
	CKaollaDoc* m_mainDocument;
	ThreadManager* threadManager;

	// Storage for all the data
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
	CKaollaView();
	DECLARE_DYNCREATE(CKaollaView)

public:
	enum { IDD = IDD_KAOLLA_FORM };

// Attributes
public:
	CKaollaDoc* GetDocument() const;
	static CKaollaView * GetView();


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
	virtual ~CKaollaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

public:
	//------- KaollaView

	void DoEvents(void);
	LRESULT DebloqueMenu(WPARAM wParam, LPARAM lParam);
	LRESULT MiseAJour(WPARAM wParam, LPARAM lParam);
	LRESULT UnlockStartButton(WPARAM wParam, LPARAM lParam);
	void OnMsvAmpoule(void);
	void OnMsvBouteille(void);
	void OnChangementBouteille(void);

	void GetExperimentData(ExperimentPropertySheet * dialogExperimentProperties);

	// Thread callbacks
	LRESULT OnRegularThreadFinished(WPARAM wParam, LPARAM);

protected:

	//-------- KaollaView_Affichage

public:
	LRESULT AffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT AffichageMessages(CString message);								// Temporary overload
	LRESULT RajoutAffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT AffichageMesures(WPARAM wParam, LPARAM lParam);
	LRESULT AffichageEtape(WPARAM wParam, LPARAM lParam);
	LRESULT RajoutAffichageEtape(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayPreviousStep(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayCalorimeter(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayLowPressure(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayHighPressure(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayTemperatures(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayInitialPressure(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayFinalPressure(WPARAM wParam, LPARAM lParam);
	LRESULT ExchangeData(WPARAM wParam, LPARAM lParam);

	LRESULT OnThreadAffichage(WPARAM wParam, LPARAM lParam);
	LRESULT MessageBoxConfirmation(WPARAM wParam, LPARAM);

	//-------- KaollaView_Boutons
public:
	void OnBnClickedLancer();
	void OnBnClickedArreter();
	LRESULT Annuler(WPARAM wParam, LPARAM lParam);

	void OnBnClickedButtonParametresExperience();
	void OnBnClickedArretSousVide();
	void OnBnClickedPause();
	void OnBnClickedProchaineCommande();
	void OnBnClickedProchaineDose();
	void OnBnClickedProchaineEtape();
	void OnBnClickedReprise();

	//-------- KaollaView_Boutons_Vannes
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
};


#ifndef _DEBUG  // debug version in KaollaView.cpp
inline CKaollaDoc* CKaollaView::GetDocument() const
   { return reinterpret_cast<CKaollaDoc*>(m_pDocument); }
#endif

