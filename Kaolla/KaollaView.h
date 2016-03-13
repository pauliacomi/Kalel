
// KaollaView.h : interface of the CKaollaView class
//

#pragma once

#include "afxwin.h"
#include <afxmt.h> // CEvent

#include "DefinePostMessages.h"

#include "Parametres.h"



class CKaollaView : public CFormView
{

public:
	// pointer to the main document
	CKaollaDoc* m_mainDocument;
	CKaollaView* m_mainView;

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

	// these seem to be useless
	/*float TemperatureCalo;
	float TemperatureCage;
	float TemperaturePiece;
	double Calorimetre; 
	double Basse_Pression;
	double Haute_Pression;*/
	//////

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
	void DebloqueMenu(void);
	void MiseAJour(void);
	void OnMsvAmpoule(void);
	void OnMsvBouteille(void);
	void OnChangementBouteille(void);
	
	// Thread callbacks
	LRESULT OnRegularThreadFinished(WPARAM wParam, LPARAM);

	//-------- KaollaView_Affichage

	void AffichageMessages(CString message);
	void RajoutAffichageMessages(CString rajout);
	void AffichageMesures(CString mesure);
	void AffichageEtape(CString etape);
	void RajoutAffichageEtape(CString rajout);

	LRESULT OnThreadAffichage(WPARAM wParam, LPARAM lParam);
	int MessageBoxConfirmation(CString message, UINT nType);

	//-------- KaollaView_Boutons

	afx_msg void OnBnClickedLancer();
	afx_msg void OnBnClickedArreter();
	void UnblockStopButton();
	void Annuler();

	afx_msg void OnBnClickedButtonParametresExperience();
	afx_msg void OnBnClickedArretSousVide();
	afx_msg void OnBnClickedPause();
	afx_msg void OnBnClickedProchaineCommande();
	afx_msg void OnBnClickedProchaineDose();
	afx_msg void OnBnClickedProchaineEtape();
	afx_msg void OnBnClickedReprise();

	//-------- KaollaView_Boutons_Vannes

	void AskThreadForManualCommand(int instrument, int i, bool askToActivate);
	LRESULT OnThreadRequestButtonUpdate(WPARAM wParam, LPARAM lParam);

	afx_msg void OnBnClickedOuvrir1();
	afx_msg void OnBnClickedOuvrir2();
	afx_msg void OnBnClickedOuvrir3();
	afx_msg void OnBnClickedOuvrir4();
	afx_msg void OnBnClickedOuvrir5();
	afx_msg void OnBnClickedOuvrir6();
	afx_msg void OnBnClickedOuvrir7();
	afx_msg void OnBnClickedOuvrir8();

	afx_msg void OnBnClickedFermer1();
	afx_msg void OnBnClickedFermer2();
	afx_msg void OnBnClickedFermer3();
	afx_msg void OnBnClickedFermer4();
	afx_msg void OnBnClickedFermer5();
	afx_msg void OnBnClickedFermer6();
	afx_msg void OnBnClickedFermer7();
	afx_msg void OnBnClickedFermer8();

	afx_msg void OnBnClickedActiverEV1();
	afx_msg void OnBnClickedDesactiverEV1();
	afx_msg void OnBnClickedActiverEV2();
	afx_msg void OnBnClickedDesactiverEV2();
	afx_msg void OnBnClickedActiverPompe();
	afx_msg void OnBnClickedDesactiverPompe();
};


#ifndef _DEBUG  // debug version in KaollaView.cpp
inline CKaollaDoc* CKaollaView::GetDocument() const
   { return reinterpret_cast<CKaollaDoc*>(m_pDocument); }
#endif

