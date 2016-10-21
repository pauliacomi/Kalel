#pragma once

#include "afxwin.h"

#include "Resources/DefineStages.h"									// Stages/experiment type definitions
#include "Resources/StringTable.h"									// Error message strings

#include "Kalel.h"
#include "KalelDoc.h"

#include "Com Classes/ExperimentData.h"								// Where data about the experimental parameters, results and current status is stored
#include "Com Classes/ExperimentSettings.h"							// Where returned data from results is stored
#include "Com Classes/MachineSettings.h"							// Where the settings for the connected machine are stored

#include "Parametres.h"												// Parameters file read/write
#include "CommHandler.h"											// Responsible for all communication to/from client


// Forward declarations
class ExperimentPropertySheet;

class CKalelView : public CFormView
{

protected: // create from serialization only
	CKalelView();
	DECLARE_DYNCREATE(CKalelView)

public:
	enum { IDD = IDD_Kalel_FORM };

	// Attributes
public:
	CKalelDoc* GetDocument() const;
	static CKalelView * GetView();

	// Overrides
protected:
	virtual void OnDraw(CDC* pDC);							// overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void DoDataExchange(CDataExchange* pDX);		// Prise en charge de DDX/DDV
	virtual void OnInitialUpdate();							// premier appel après la construction

	// Implementation
public:
	virtual ~CKalelView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

	//*************************************************************************************************************************
	//						Class shared
	//*************************************************************************************************************************


	// Classes and pointers
private:
	CKalelApp* pApp;				// pointer to the main app
	CKalelDoc* m_mainDocument;		// pointer to the main document
	Parameters savedParams;			// Class for saving the parameters file
	CommHandler commHandler;		// Class dealing with all communication between frontend and backend

	// Storage
private:
	MeasurementsArray dataCollection;							// Measurement points for an experiment are stored here
	std::shared_ptr<MachineSettings>	machineSettings;		// Pointer to a machineSettings object
	std::shared_ptr<MachineSettings>	tempSettings;			// Temporary machineSettings stored here between sending change request to server and server acknowledgement
	std::shared_ptr<ExperimentSettings> experimentSettings;		// Local storage of experimentSettings
	
	
	// Some storage variables for each MFC control
private:
	CButton m_ButtonLancer;
	CButton m_ButtonArreter;

	CEdit pEditMessages;
	CEdit pEditMesures;

	CString m_StrEditMessages					= _T("");
	CString m_StrEditMesures					= _T("");

	CString m_StrCalo							= _T("");
	CString m_StrBassePression					= _T("");
	CString m_StrHautePression					= _T("");
	CString m_StrTemperatureCalo				= _T("");
	CString m_StrTemperatureCage				= _T("");
	CString m_StrTemperaturePiece				= _T("");
	CString m_StrTemps							= _T("");
	CString m_StrPressionInitiale				= _T("");
	CString m_StrPressionFinale					= _T("");
	CString m_StrEtape							= _T("");
	CString m_StrTemoinVanne1					= _T("");
	CString m_StrTemoinVanne2					= _T("");
	CString m_StrTemoinVanne3					= _T("");
	CString m_StrTemoinVanne4					= _T("");
	CString m_StrTemoinVanne5					= _T("");
	CString m_StrTemoinVanne6					= _T("");
	CString m_StrTemoinVanne7					= _T("");
	CString m_StrTemoinVanne8					= _T("");
	CString m_StrTemoinEV1						= _T("");
	CString m_StrTemoinEV2						= _T("");
	CString m_StrTemoinPompe					= _T("");


	//*************************************************************************************************************************
	//						KalelView
	//*************************************************************************************************************************

private:
	void OnTimer(UINT nIDEvent);	//timer for window update
	void GetExperimentData(ExperimentPropertySheet * dialogExperimentProperties, bool initialRequest);

	// Menu messages

	LRESULT OnMsvAmpoule(WPARAM wParam, LPARAM lParam);
	LRESULT OnMsvBouteille(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangementBouteille(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayConnectDialog(WPARAM, LPARAM);
	LRESULT DisplayPortDialog(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayApparatusSettingsDialog(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadStart(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadStop(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadRestart(WPARAM wParam, LPARAM lParam);

	// Thread callbacks

	LRESULT OnServerConnected(WPARAM wParam, LPARAM lParam);
	LRESULT OnGetMachineSettings(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetMachineSettings(WPARAM wParam, LPARAM lParam);
	LRESULT OnExchangeData(WPARAM wParam, LPARAM lParam);
	LRESULT OnExchangeLogs(WPARAM, LPARAM incomingLogs);
	LRESULT OnAutoExperimentFinished(WPARAM wParam, LPARAM);
	LRESULT CancelBeforeStarting(WPARAM wParam, LPARAM lParam);

	//*************************************************************************************************************************
	//						KalelView_Display
	//*************************************************************************************************************************

private:

	LRESULT AffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayTextboxValues(ExperimentData * data);
	LRESULT DiplayMeasurements(ExperimentData * data);
	LRESULT DisplayStepProgress(ExperimentData * data);
	LRESULT MessageBoxAlert(WPARAM wParam, LPARAM lParam);
	LRESULT MessageBoxConfirmation(WPARAM wParam, LPARAM);

	//*************************************************************************************************************************
	//						KalelView_Boutons
	//*************************************************************************************************************************

private:
	void OnBnClickedLancer();
	void OnBnClickedArreter();
	void OnBnClickedButtonParametresExperience();
	void OnBnClickedArretSousVide();
	void OnBnClickedPause();
	void OnBnClickedProchaineCommande();
	void OnBnClickedProchaineDose();
	void OnBnClickedProchaineEtape();
	void OnBnClickedReprise();

	void UpdateButtons();

	//*************************************************************************************************************************
	//						KalelView_Boutons_Vannes
	//*************************************************************************************************************************

private:
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

