#pragma once

#include "afxwin.h"

#include "../Kalel Shared/Resources/DefineStages.h"					// Stages/experiment type definitions
#include "../Kalel Shared/Resources/StringTable.h"					// Error message strings

#include "Kalel.h"
#include "KalelDoc.h"

#include "../Kalel Shared/Com Classes/ExperimentData.h"				// Where results are stored
#include "../Kalel Shared/Com Classes/ExperimentStatus.h"			// Where data about the experimental parameters and current status is stored
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"			// Where returned data from results is stored
#include "../Kalel Shared/Com Classes/MachineSettings.h"			// Where the settings for the connected machine are stored

#include "Parametres.h"												// Parameters file read/write
#include "CommHandler.h"											// Responsible for all communication to/from client
#include "ListOfInstrumentButtons.h"
#include <deque>


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
	CKalelApp* pApp = nullptr;				// pointer to the main app
	CKalelDoc* m_mainDocument = nullptr;	// pointer to the main document

	Parameters savedParams;					// Class for saving the parameters file
	CommHandler commHandler;				// Class dealing with all communication between frontend and backend

	//*************************************************************************************************************************
	//						Storage classes
	//*************************************************************************************************************************
private:
	std::chrono::system_clock::time_point												noTime;					// Timepoint storing when the experiment settings were generated

	// Machine settings related
	std::chrono::system_clock::time_point												machineSettingsTime;	// Timepoint storing when the machine settings were generated
	std::unique_ptr<MachineSettings>													machineSettings;		// Pointer to a machineSettings object
	std::unique_ptr<MachineSettings>													tempMchSettings;		// Temporary machineSettings stored here between sending change request to server and server acknowledgement
	
	// Control state related
	ListOfInstrumentButtons																buttonStates;			// The class which deals which keeps all the state of the buttons, for displaying them 
	std::chrono::system_clock::time_point												machineStateTime;		// Timepoint storing when the control state was generated
	
	// Experiment Settings related
	std::chrono::system_clock::time_point												experimentSettingsTime;	// Timepoint storing when the experiment settings were generated
	std::unique_ptr<ExperimentSettings>													experimentSettings;		// Local storage of experimentSettings
	std::unique_ptr<ExperimentSettings>													tempExpSettings;		// Temporary experimentSettings stored here between sending change request to server and server acknowledgement
	
	// Experiment Status related
	std::chrono::system_clock::time_point												experimentStatusTime;	// Timepoint storing when the experiment status was generated
	ExperimentStatus																	experimentStatus;		// Local storage of experimentStatus

	// Data/Logs/Request collections
	std::map<std::chrono::system_clock::time_point, std::wstring> 						logCollection;			// Log points for an experiment are stored here
	std::map<std::chrono::system_clock::time_point, std::wstring> 						requestCollection;		// Log points for an all errors or requests are stored here
	std::map<std::chrono::system_clock::time_point, ExperimentData>						dataCollection;			// Measurement points for an experiment are stored here
		

	//*************************************************************************************************************************
	//						Some storage variables for each MFC control
	//*************************************************************************************************************************
private:
	CButton m_ButtonLancer;
	CButton m_ButtonArreter;

	CEdit pEditMessages;

	CString m_StrEditMessages					= _T("");

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
	UINT_PTR graphTimer = 0;			// Timer for graph refresh
	UINT_PTR dataTimer = 0;				// Timer for data refresh, faster than graph refresh
	void OnTimer(UINT_PTR nIDEvent);	// Timer function for all timers

	// Menu messages

	LRESULT DisplayConnectDialog(WPARAM, LPARAM);
	LRESULT ManualSync(WPARAM, LPARAM);
	LRESULT OnMsvAmpoule(WPARAM wParam, LPARAM lParam);
	LRESULT OnMsvBouteille(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangementBouteille(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayPortDialog(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayApparatusSettingsDialog(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadStart(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadStop(WPARAM wParam, LPARAM lParam);
	LRESULT BackgroundThreadRestart(WPARAM wParam, LPARAM lParam);

	// Thread callbacks

	LRESULT OnServerConnected(WPARAM , LPARAM );
	LRESULT OnServerDisconnected(WPARAM, LPARAM);
	LRESULT OnSync(WPARAM , LPARAM );

	LRESULT OnSetMachineSettings(WPARAM, LPARAM);
	LRESULT OnExchangeMachineSettings(WPARAM wParam, LPARAM incomingMachineSettings);

	LRESULT OnSetExperimentSettings(WPARAM, LPARAM);
	LRESULT OnExchangeExperimentSettings(WPARAM wParam, LPARAM incomingExperimentSettings);

	LRESULT OnExchangeExperimentStatus(WPARAM wParam, LPARAM incomingExperimentStatus);

	LRESULT OnExchangeInstrumentState(WPARAM wParam, LPARAM lParam);
	LRESULT OnSetInstrumentState(WPARAM wParam, LPARAM lParam);

	LRESULT OnExchangeData(WPARAM, LPARAM incomingExperimentData);
	LRESULT OnExchangeLogs(WPARAM, LPARAM incomingLogs);
	LRESULT OnExchangeRequests(WPARAM, LPARAM incomingRequests);

	LRESULT OnExperimentFinished(WPARAM wParam, LPARAM);

	//*************************************************************************************************************************
	//						KalelView_Display
	//*************************************************************************************************************************

private:

	LRESULT AffichageMessages(WPARAM wParam, LPARAM lParam);
	LRESULT DisplayTextboxValues(const ExperimentData &data, const ExperimentStatus &status);
	LRESULT DisplayStepProgress(const ExperimentStatus &status);
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

	bool GetExperimentData(ExperimentPropertySheet & pDialogExperimentProperties, ExperimentSettings & expS, bool initialRequest);
	void ReplaceExperimentSettings(const ExperimentPropertySheet & pDialogExperimentProperties, ExperimentSettings & expS);

	//*************************************************************************************************************************
	//						KalelView_Boutons_Vannes
	//*************************************************************************************************************************

private:
	void AskThreadForManualCommand(int instrument, int i, bool askToActivate);
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

