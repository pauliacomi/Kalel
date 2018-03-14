
// KalelView.cpp : implementation of the CKalelView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kalel.h"
#endif

#include "KalelDoc.h"
#include "KalelView.h"


// Dialog Box includes
#include "DialogMachineParameters.h"
#include "DialogConnectionPort.h"
#include "DialogConnectServer.h"

#include "DefinePostMessages.h"										// Definition of messages received from the automation functionality
#include "DefineMenuMessages.h"										// Definition of messages received from the menu

#include "ExperimentPropertySheet.h"								// The dialog asking the user to input the experiment parameters
#include "../Kalel Shared/timeHelpers.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CKalelView

IMPLEMENT_DYNCREATE(CKalelView, CFormView)

//*************************************************************************************************************************
//						MESSAGE MAP
//*************************************************************************************************************************

BEGIN_MESSAGE_MAP(CKalelView, CFormView)

	//************************************
	// CUSTOM MESSAGE MAP MESSAGES
	//************************************

	// Menu messages:
	ON_MESSAGE(UWM_DISP_CONNECTS_DIALOG,			&CKalelView::DisplayConnectDialog)				// Display dialog connection
	ON_MESSAGE(UWM_DISP_MANUAL_SYNC,				&CKalelView::ManualSync)						// Force a complete sync
	ON_MESSAGE(UWM_DISP_PORT_DIALOG,				&CKalelView::DisplayPortDialog)					// Display dialog ports
	ON_MESSAGE(UWM_DISP_DEVSETTINGS_DIALOG,			&CKalelView::DisplayApparatusSettingsDialog)	// Display dialog machine parameters

	// Server requests from other views
	ON_MESSAGE(UWM_FUNC_VACUUM_SAMPLE,				&CKalelView::OnMsvAmpoule)						// Request sample under vacuum
	ON_MESSAGE(UWM_FUNC_VACUUM_BOTTLE,				&CKalelView::OnMsvBouteille)					// Request bottle under vacuum
	ON_MESSAGE(UWM_FUNC_CHANGE_BOTTLE,				&CKalelView::OnChangementBouteille)				// Request bottle change procedure
	ON_MESSAGE(UWM_THREAD_START,					&CKalelView::BackgroundThreadStart)				// Request thread shutdown
	ON_MESSAGE(UWM_THREAD_STOP,						&CKalelView::BackgroundThreadStop)				// Request thread stop
	ON_MESSAGE(UWM_THREAD_RESTART,					&CKalelView::BackgroundThreadRestart)			// Request thread restart

	// Server callbacks
	ON_MESSAGE(UWM_SIGNAL_SERVER_CONNECTED,			&CKalelView::OnServerConnected)					// Callback to notify of successful server connection
	ON_MESSAGE(UWM_SIGNAL_SERVER_DISCONNECTED,		&CKalelView::OnServerDisconnected)				// Callback to notify of server disconnect
	ON_MESSAGE(UWM_SYNCED,							&CKalelView::OnSync)							// Callback on initial instrument sync
	ON_MESSAGE(UWM_SET_MACHINESETTINGS,				&CKalelView::OnSetMachineSettings)				// Callback on notify of server-side changed MachineSettings
	ON_MESSAGE(UWM_EXCHANGE_MACHINESETTINGS,		&CKalelView::OnExchangeMachineSettings)			// Callback to notify of received MachineSettings
	ON_MESSAGE(UWM_EXCHANGESTATE,					&CKalelView::OnExchangeInstrumentState)			// Calls to update all button pairs and associated display on a manual message
	ON_MESSAGE(UWM_EXCHANGESTATESPECIFIC,			&CKalelView::OnSetInstrumentState)				// Calls to update a specific button pair and associated display on a manual message
	ON_MESSAGE(UWM_SET_EXPERIMENTSETTINGS,			&CKalelView::OnSetExperimentSettings)			// Callback on notify of server-side changed MachineSettings
	ON_MESSAGE(UWM_EXCHANGE_EXPERIMENTSETTINGS,		&CKalelView::OnExchangeExperimentSettings)		// Callback to notify of received ExperimetnSettings
	ON_MESSAGE(UWM_EXCHANGE_EXPERIMENTSTATUS,		&CKalelView::OnExchangeExperimentStatus)		// Callback to notify of received ExperimetnSettings
	ON_MESSAGE(UWM_EXCHANGEDATA,					&CKalelView::OnExchangeData)					// Callback to notify of incoming ExperimentData array
	ON_MESSAGE(UWM_EXCHANGELOGS,					&CKalelView::OnExchangeLogs)					// Callback to notify of incoming log array
	ON_MESSAGE(UWM_EXCHANGEREQUESTS,				&CKalelView::OnExchangeRequests)				// Callback to notify of incoming request array
	ON_MESSAGE(UWM_AUTOEXPFINISHED,					&CKalelView::OnAutoExperimentFinished)			// Calls when an experiment ends
	ON_MESSAGE(UWM_DISPLAYMESSAGE,					&CKalelView::AffichageMessages)					// Callback to display a message from the automation thread
	ON_MESSAGE(UWM_DISPLAYMESSAGEBOX,				&CKalelView::MessageBoxAlert)					// Displays an messageBox to alert user of something
	ON_MESSAGE(UWM_DISPLAYMESSAGEBOXCONF,			&CKalelView::MessageBoxConfirmation)			// Displays an messageBox to or ask user for confirmation
	ON_MESSAGE(UWM_CANCELEXPERIMENT,				&CKalelView::CancelBeforeStarting)						
																							
	
	//************************************
	// Standard messages
	//************************************

	// Buttons which are used for automatic/advanced functionality
	ON_BN_CLICKED(IDC_LANCER,								&CKalelView::OnBnClickedLancer)
	ON_BN_CLICKED(IDC_ARRETER,								&CKalelView::OnBnClickedArreter)
	ON_BN_CLICKED(IDC_REPRISE,								&CKalelView::OnBnClickedReprise)
	ON_BN_CLICKED(IDC_ARRET_SOUS_VIDE,						&CKalelView::OnBnClickedArretSousVide)
	ON_BN_CLICKED(IDC_PAUSE,								&CKalelView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_PROCHAINE_COMMANDE,					&CKalelView::OnBnClickedProchaineCommande)
	ON_BN_CLICKED(IDC_PROCHAINE_DOSE,						&CKalelView::OnBnClickedProchaineDose)
	ON_BN_CLICKED(IDC_PROCHAINE_ETAPE,						&CKalelView::OnBnClickedProchaineEtape)
	ON_BN_CLICKED(IDC_BUTTON_PARAMETRES_EXPERIENCE,			&CKalelView::OnBnClickedButtonParametresExperience)
	
	// Messages for UI buttons used for simple instrument manipulation
	ON_BN_CLICKED(IDC_OUVRIR1,								&CKalelView::OnBnClickedOuvrir1)
	ON_BN_CLICKED(IDC_OUVRIR2,								&CKalelView::OnBnClickedOuvrir2)
	ON_BN_CLICKED(IDC_OUVRIR3,								&CKalelView::OnBnClickedOuvrir3)
	ON_BN_CLICKED(IDC_OUVRIR4,								&CKalelView::OnBnClickedOuvrir4)
	ON_BN_CLICKED(IDC_OUVRIR5,								&CKalelView::OnBnClickedOuvrir5)
	ON_BN_CLICKED(IDC_OUVRIR6,								&CKalelView::OnBnClickedOuvrir6)
	ON_BN_CLICKED(IDC_OUVRIR7,								&CKalelView::OnBnClickedOuvrir7)
	ON_BN_CLICKED(IDC_OUVRIR8,								&CKalelView::OnBnClickedOuvrir8)
	ON_BN_CLICKED(IDC_FERMER1,								&CKalelView::OnBnClickedFermer1)
	ON_BN_CLICKED(IDC_FERMER2,								&CKalelView::OnBnClickedFermer2)
	ON_BN_CLICKED(IDC_FERMER3,								&CKalelView::OnBnClickedFermer3)
	ON_BN_CLICKED(IDC_FERMER4,								&CKalelView::OnBnClickedFermer4)
	ON_BN_CLICKED(IDC_FERMER5,								&CKalelView::OnBnClickedFermer5)
	ON_BN_CLICKED(IDC_FERMER6,								&CKalelView::OnBnClickedFermer6)
	ON_BN_CLICKED(IDC_FERMER7,								&CKalelView::OnBnClickedFermer7)
	ON_BN_CLICKED(IDC_FERMER8,								&CKalelView::OnBnClickedFermer8)
	ON_BN_CLICKED(IDC_ACTIVER_EV1,							&CKalelView::OnBnClickedActiverEV1)
	ON_BN_CLICKED(IDC_DESACTIVER_EV1,						&CKalelView::OnBnClickedDesactiverEV1)
	ON_BN_CLICKED(IDC_ACTIVER_EV2,							&CKalelView::OnBnClickedActiverEV2)
	ON_BN_CLICKED(IDC_DESACTIVER_EV2,						&CKalelView::OnBnClickedDesactiverEV2)
	ON_BN_CLICKED(IDC_ACTIVER_POMPE,						&CKalelView::OnBnClickedActiverPompe)
	ON_BN_CLICKED(IDC_DESACTIVER_POMPE,						&CKalelView::OnBnClickedDesactiverPompe)


	//************************************
	// Standard messages
	//************************************
	ON_WM_TIMER()					

END_MESSAGE_MAP()

//*************************************************************************************************************************
//						Constructor/Destructor
//*************************************************************************************************************************

CKalelView::CKalelView()
	: CFormView(CKalelView::IDD)
	, buttonStates(*this)
{
}


CKalelView::~CKalelView()
{
}

//*************************************************************************************************************************
//						Liaising between variables and controls
//*************************************************************************************************************************
void CKalelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX,		IDC_LANCER,				m_ButtonLancer			);
	DDX_Control(pDX,		IDC_ARRETER,			m_ButtonArreter			);

	DDX_Control(pDX,		IDC_EDIT_MESSAGES,		pEditMessages			);
	DDX_Text(pDX,			IDC_EDIT_MESSAGES,		m_StrEditMessages		);

	DDX_Text(pDX,			IDC_CALO,				m_StrCalo				);
	DDX_Text(pDX,			IDC_BASSE_PRESSION,		m_StrBassePression		);
	DDX_Text(pDX,			IDC_HAUTE_PRESSION,		m_StrHautePression		);
	DDX_Text(pDX,			IDC_TEMPERATURE_CALO,	m_StrTemperatureCalo	);
	DDX_Text(pDX,			IDC_TEMPERATURE_CAGE,	m_StrTemperatureCage	);
	DDX_Text(pDX,			IDC_TEMPERATURE_PIECE,	m_StrTemperaturePiece	);
	DDX_Text(pDX,			IDC_TEMPS,				m_StrTemps				);
	DDX_Text(pDX,			IDC_PRESSION_INITIALE,	m_StrPressionInitiale	);
	DDX_Text(pDX,			IDC_PRESSION_FINALE,	m_StrPressionFinale		);
	DDX_Text(pDX,			IDC_EDIT_ETAPE,			m_StrEtape				);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE1,		m_StrTemoinVanne1		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE2,		m_StrTemoinVanne2		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE3,		m_StrTemoinVanne3		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE4,		m_StrTemoinVanne4		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE5,		m_StrTemoinVanne5		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE6,		m_StrTemoinVanne6		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE7,		m_StrTemoinVanne7		);
	DDX_Text(pDX,			IDC_TEMOIN_VANNE8,		m_StrTemoinVanne8		);
	DDX_Text(pDX,			IDC_TEMOIN_EV1,			m_StrTemoinEV1			);
	DDX_Text(pDX,			IDC_TEMOIN_EV2,			m_StrTemoinEV2			);
	DDX_Text(pDX,			IDC_TEMOIN_POMPE,		m_StrTemoinPompe		);
}


//*************************************************************************************************************************
//						Creation functions, initial calls, drawing
//*************************************************************************************************************************

BOOL CKalelView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}


void CKalelView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	// Get a pointer to the app itself for access to the menu availability
	pApp = static_cast<CKalelApp *>(AfxGetApp());

	// Pass stored array to graph
	GetDocument()->GraphSetArray(dataCollection);
	
	// Initial button set-up
	UpdateButtons();
	
	// Pass window handle to comm handler
	commHandler.SetHandle(GetSafeHwnd());

	// Then connect to the server if the address exists
	commHandler.Connect(savedParams.GetServerAddress());

	// Set the timers for the window update
	dataTimer = SetTimer(1, savedParams.GetDataRefreshInterval(), NULL);
	graphTimer = SetTimer(1, savedParams.GetGraphRefreshInterval(), NULL);
}


// CKalelView drawing

void CKalelView::OnDraw(CDC* /*pDC*/)
{
	CKalelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

}

//*************************************************************************************************************************
//						Diagnostics
//*************************************************************************************************************************

#ifdef _DEBUG
void CKalelView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKalelView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKalelDoc* CKalelView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKalelDoc)));
	return (CKalelDoc*)m_pDocument;
}
#endif //_DEBUG


// Returns a pointer to the view itself or NULL if it fails
CKalelView * CKalelView::GetView()
{
	CDocTemplate* pTemplate;
	CKalelApp* pAppl = static_cast<CKalelApp *>(AfxGetApp());

	POSITION pos = pAppl->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pTemplate = pAppl->GetNextDocTemplate(pos);
		ASSERT(pTemplate);

		// iterate through template documents
		POSITION pos2 = pTemplate->GetFirstDocPosition();
		while (pos2)
		{
			CDocument* pDoc = pTemplate->GetNextDoc(pos2);
			ASSERT(pDoc);

			// iterate through views
			POSITION pos3 = pDoc->GetFirstViewPosition();
			while (pos3 != NULL)
			{
				CView* pView = pDoc->GetNextView(pos3);
				ASSERT(pView);
				if (::IsWindow(pView->GetSafeHwnd()))
				{
					if (pView->IsKindOf(RUNTIME_CLASS(CKalelView)))
					{
						CKalelView* pKV = static_cast<CKalelView *>(pView);
						return pKV;
					}
				}
			}
		}
	}

	return NULL;
}


/**********************************************************************************************************************************
//				Timer function
**********************************************************************************************************************************/

void CKalelView::OnTimer(UINT_PTR nIDEvent)
{
	if (pApp->serverConnected) {

		if (!dataCollection.empty())
		{
			//*****
			// Refresh data timer
			//*****
			if (nIDEvent == dataTimer)
			{
				// We check to see if we need to delete some of the stored data
				// in the case of no experiments running
				if (!pApp->experimentRunning)
				{
					auto check_delete = dataCollection.upper_bound(dataCollection.rbegin()->first - std::chrono::minutes(5));
					if (check_delete != dataCollection.begin())
					{
						dataCollection.erase(dataCollection.begin(), check_delete);
					}
				}

				// Request ongoing sync
				commHandler.Sync(false, 
					timeh::TimePointToString(experimentSettingsTime), 
					timeh::TimePointToString(machineSettingsTime), 
					timeh::TimePointToString(machineStateTime),
					timeh::TimePointToString(experimentStatusTime)
				);

				// Send the request for data
				commHandler.GetData(timeh::TimePointToString(dataCollection.rbegin()->first));

				// Send the request for logs
				if (!logCollection.empty())
					commHandler.GetLog(timeh::TimePointToString(logCollection.rbegin()->first));
				else
					commHandler.GetLog();

				// Send the request for user input
				if (!requestCollection.empty())
					commHandler.GetRequests(timeh::TimePointToString(requestCollection.rbegin()->first));
				else
					commHandler.GetRequests();


				// We check to see if there are any requests from the experiment

				// Write textbox values
				DisplayTextboxValues(dataCollection.rbegin()->second, experimentStatus);

				// Write the current step
				DisplayStepProgress(experimentStatus);
			}

			//*****
			// Refresh graph timer
			//*****
			if (nIDEvent == graphTimer)
			{
				// Write graph
				GetDocument()->UpdateAllViews(this);
			}
		}
	}

	CFormView::OnTimer(nIDEvent);	// Call base class handler.
}


/**********************************************************************************************************************************
// Menu functionality
**********************************************************************************************************************************/

LRESULT CKalelView::DisplayConnectDialog(WPARAM, LPARAM)
{
	DialogConnectServer connectServer;
	if (connectServer.DoModal() == IDOK)
	{
		std::wstring address{ connectServer.GetAddress() };
		
		// First save the address
		savedParams.SetServerAddress(address);

		// Then connect to the server
		commHandler.Connect(address);
	}

	return 0;
}

LRESULT CKalelView::ManualSync(WPARAM, LPARAM)
{
	commHandler.Sync(true);
	return 0;
}


LRESULT CKalelView::DisplayPortDialog(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		ConnectionPort m_connection_ports;
		m_connection_ports.PassSettings(machineSettings.get());
		if (m_connection_ports.DoModal() == IDOK) {
			if (m_connection_ports.Changed())
			{
				tempMchSettings = m_connection_ports.GetSettings();			// Save it separately to prevent extra communication
				commHandler.SetMachineSettings(tempMchSettings);
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}

	return 0;
}

LRESULT CKalelView::DisplayApparatusSettingsDialog(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		ApparatusParameters apparatusParameters;
		apparatusParameters.PassSettings(machineSettings.get());
		if (apparatusParameters.DoModal() == IDOK) {
			if (apparatusParameters.Changed())
			{
				tempMchSettings = apparatusParameters.GetSettings();			// Save it separately to prevent extra communication
				commHandler.SetMachineSettings(tempMchSettings);
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}

	return 0;
}

LRESULT CKalelView::BackgroundThreadStart(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		commHandler.StartClient();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
	return 0;
}


LRESULT CKalelView::BackgroundThreadStop(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		commHandler.ShutdownClient();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
	return 0;
}


LRESULT CKalelView::BackgroundThreadRestart(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		commHandler.RestartClient();
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}
	return 0;
}


LRESULT CKalelView::OnMsvAmpoule(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		if (pApp->experimentRunning) {
			AfxMessageBox(ERROR_EXP_INPROGRESS, MB_ICONEXCLAMATION | MB_OK);
		}
		else {
			if (AfxMessageBox(PROMPT_VACUUM_SAMPLE, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				experimentSettings->experimentType = EXPERIMENT_TYPE_SAMPLE_VACUUM;

				// Block menu and set running flag
				pApp->experimentRunning = true;
				pApp->menuIsAvailable = false;
				UpdateButtons();

				// Give command
				commHandler.FunctionSampleVacuum();
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}

	return 0;
}

LRESULT CKalelView::OnMsvBouteille(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		if (pApp->experimentRunning) {
			AfxMessageBox(ERROR_EXP_INPROGRESS, MB_ICONEXCLAMATION | MB_OK);
		}
		else {
			if (AfxMessageBox(PROMPT_VACUUM_BOTTLE, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				experimentSettings->experimentType = EXPERIMENT_TYPE_BOTTLE_VACUUM;

				// Block menu and set running flag
				pApp->experimentRunning = true;
				pApp->menuIsAvailable = false;
				UpdateButtons();

				// Raise the flag for data modified
				commHandler.FunctionBottleVacuum();
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}

	return 0;
}

LRESULT CKalelView::OnChangementBouteille(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		if (pApp->experimentRunning) {
			AfxMessageBox(ERROR_EXP_INPROGRESS, MB_ICONEXCLAMATION | MB_OK);
		}
		else {
			if (AfxMessageBox(PROMPT_CHANGE_BOTTLE, MB_YESNO | MB_ICONQUESTION) == IDYES)
			{
				ASSERT(0);
				experimentSettings->experimentType = EXPERIMENT_TYPE_BOTTLE_VACUUM;

				// Block menu and set running flag
				pApp->experimentRunning = true;
				pApp->menuIsAvailable = false;
				UpdateButtons();

				// Raise the flag for data modified
				commHandler.FunctionChangeBottle();
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}

	return 0;
}

/**********************************************************************************************************************************
// Server callback commands
**********************************************************************************************************************************/

LRESULT CKalelView::OnServerConnected(WPARAM, LPARAM)
{
	pApp->serverConnected = true;
	dataCollection.clear();
	commHandler.SaveAddress(savedParams.GetServerAddress());
	commHandler.Sync(true);
	return 0;
}

LRESULT CKalelView::OnServerDisconnected(WPARAM, LPARAM)
{
	pApp->serverConnected = false;
	pApp->experimentRunning = false;
	UpdateButtons();
	return 0;
}

LRESULT CKalelView::OnSync(WPARAM, LPARAM)
{
	// Make sure buttons are updated
	if (experimentSettings && experimentSettings->experimentType != EXPERIMENT_TYPE_UNDEF) {
		pApp->experimentRunning = true;
		pApp->menuIsAvailable = false;
	}
	else
	{
		pApp->experimentRunning = false;
		pApp->menuIsAvailable = true;
	}

	UpdateButtons();

	return 0;
}

LRESULT CKalelView::OnExchangeMachineSettings(WPARAM, LPARAM incomingMachineSettings)
{
	// Get the incoming pointer
	machineSettings.reset(reinterpret_cast<MachineSettings*>(incomingMachineSettings));
	machineSettingsTime = timeh::NowTime();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnSetMachineSettings(WPARAM, LPARAM)
{
	// Make the temporary local version official
	machineSettings = tempMchSettings;
	machineSettingsTime = timeh::NowTime();
	tempMchSettings.reset();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnExchangeExperimentSettings(WPARAM wParam, LPARAM incomingExperimentSettings)
{
	// Get the incoming pointer
	experimentSettings.reset(reinterpret_cast<ExperimentSettings*>(incomingExperimentSettings));
	experimentSettingsTime = timeh::NowTime();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnExchangeExperimentStatus(WPARAM wParam, LPARAM incomingExperimentStatus)
{	
	// Get the incoming pointer
	experimentStatus.reset(reinterpret_cast<ExperimentStatus*>(incomingExperimentStatus));
	experimentStatusTime = timeh::NowTime();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnSetExperimentSettings(WPARAM, LPARAM)
{
	// Make the temporary local version official
	experimentSettings = tempExpSettings;
	experimentSettingsTime = timeh::NowTime();
	tempExpSettings.reset();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}


LRESULT CKalelView::OnExchangeInstrumentState(WPARAM wParam, LPARAM incomingInstrumentState) {

	// Cast the parameters object and take ownership
	std::auto_ptr<ControlInstrumentState> maParam(reinterpret_cast<ControlInstrumentState*>(incomingInstrumentState));
	buttonStates.Update(*maParam);

	// Set time
	machineStateTime = timeh::NowTime();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnSetInstrumentState(WPARAM wParam, LPARAM incomingInstrumentState) {

	// Cast the parameters object and take ownership
	std::auto_ptr<ControlInstrumentStateData> maParam(reinterpret_cast<ControlInstrumentStateData*>(incomingInstrumentState));

	// Update buttons
	buttonStates.EndCommand(*maParam);

	// unlock the menu
	pApp->menuIsAvailable = true;

	return 0;
}

LRESULT CKalelView::OnExchangeData(WPARAM, LPARAM incomingExperimentData)
{
	// Get the incoming vector and add it to the local data
	auto newData = reinterpret_cast<ExperimentDataStorageArray*>(incomingExperimentData);
	dataCollection.insert(newData->begin(), newData->end());

	// Delete the useless vector now
	delete newData;

	return 0;
}

LRESULT CKalelView::OnExchangeLogs(WPARAM, LPARAM incomingLogs)
{
	// Get the incoming vector and add it to the local logs
	auto newLogs = reinterpret_cast<std::map<std::chrono::system_clock::time_point, std::wstring>*>(incomingLogs);
	logCollection.insert(newLogs->begin(), newLogs->end());

	// Display logs
	CString * temp = new CString();
	for (auto i = newLogs->begin(); i != newLogs->end(); ++i)
	{
		CString time(timeh::TimePointToString(i->first).c_str());
		CString log(i->second.c_str());
		temp->Append(time + " " + log + _T("\r\n"));
	}
	AffichageMessages(NULL, (LPARAM)temp);
	
	// Delete the useless vector now
	delete newLogs;

	return 0;
}


LRESULT CKalelView::OnExchangeRequests(WPARAM, LPARAM incomingRequests)
{
	// Get the incoming vector and add it to the local logs
	auto newRequests = reinterpret_cast<std::map<std::chrono::system_clock::time_point, std::wstring>*>(incomingRequests);
	requestCollection.insert(newRequests->begin(), newRequests->end());

	// Display requests

	// Delete the useless vector now
	delete newRequests;

	return 0;
}

// TODO: deprecated
LRESULT CKalelView::OnAutoExperimentFinished(WPARAM, LPARAM) {

	experimentSettings->ResetData();

	// Signal that this is the experiment end
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();
	
	return 0;
}

// When the experiment is signalled as cancelled from the thread or it times out
LRESULT CKalelView::CancelBeforeStarting(WPARAM, LPARAM)
{
	// Signal that this is the experiment end
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();

	return 0;
}


/******************************************
// Message boxes
******************************************/

LRESULT CKalelView::MessageBoxAlert(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	std::auto_ptr<UINT> nType(reinterpret_cast<UINT*>(wParam));
	
	return AfxMessageBox(*message, *nType);
}

// TODO: make sure the confirmation is what is required
LRESULT CKalelView::MessageBoxConfirmation(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	std::auto_ptr<UINT> nType(reinterpret_cast<UINT*>(wParam));

	int result;
	bool continuer = true;
	do {
		result = AfxMessageBox(*message, *nType);
		if (result == IDCANCEL)
		{
			if (AfxMessageBox(PROMPT_CANCELEXP, MB_YESNO | MB_ICONWARNING, 0) == IDYES) {
				commHandler.ResetClient();
				continuer = false;
			}
		}
		else {
			if (result == IDYES || result == IDOK) {
				commHandler.ResumeClient();
				continuer = false;
			}
			if (result == IDNO) {
				commHandler.ResumeClient();
				commHandler.SetUserChoice();
				continuer = false;
			}
		}
	} while (continuer);
	return result;
}