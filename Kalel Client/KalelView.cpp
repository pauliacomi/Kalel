
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
#include "../Kalel Shared/log.h"

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
	ON_MESSAGE(UWM_DISPLAYMESSAGE,					&CKalelView::AffichageMessages)					// Callback to display a message from the automation thread
	ON_MESSAGE(UWM_DISPLAYMESSAGEBOX,				&CKalelView::MessageBoxAlert)					// Displays an messageBox to alert user of something
	ON_MESSAGE(UWM_DISPLAYMESSAGEBOXSERVER,			&CKalelView::MessageBoxAlertServer)				// Displays an messageBox to alert user of something from the server
																							
	
	//************************************
	// Standard messages
	//************************************

	// Buttons which are used for automatic/advanced functionality
	ON_BN_CLICKED(IDC_LANCER,								&CKalelView::OnBnClickedLancer)
	ON_BN_CLICKED(IDC_ARRETER,								&CKalelView::OnBnClickedArreter)
	ON_BN_CLICKED(IDC_REPRISE,								&CKalelView::OnBnClickedReprise)
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
	machineSettings = std::make_unique<MachineSettings>();
	experimentSettings = std::make_unique<ExperimentSettings>();
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
	commHandler.SaveAuth(savedParams.GetUsername(), savedParams.GetPassword());
	commHandler.Connect(savedParams.GetServerAddress(), savedParams.GetServerPort());
	ManualSync(NULL, NULL);

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
	if (pApp->serverConnected && pApp->synced) {

			//*****
			// Refresh data timer
			//*****
		if (nIDEvent == dataTimer)
		{
			// We check to see if we need to delete some of the stored data
			// in the case of no experiments running
			if (!pApp->experimentRunning && !dataCollection.empty())
			{
				auto check_delete = dataCollection.upper_bound(dataCollection.rbegin()->first - std::chrono::minutes(5));
				if (check_delete != dataCollection.begin())
				{
					dataCollection.erase(dataCollection.begin(), check_delete);
				}
			}

			// Request ongoing sync
			commHandler.GetExperimentSettings(experimentSettings->tp);
			commHandler.GetExperimentStatus(experimentStatus->tp);
			commHandler.GetMachineSettings(machineSettings->tp);
			commHandler.GetControlInstrumentState(machineStateTime);

			if (!dataCollection.empty())
				commHandler.GetData(dataCollection.rbegin()->first);
			else
				commHandler.GetData(noTime);

			// Send the request for logs
			if (!logCollection.empty())
				commHandler.GetLog(logCollection.rbegin()->first);
			else
				commHandler.GetLog(noTime);

			// Send the request for user input
			if (!requestCollection.empty())
				commHandler.GetRequests(requestCollection.rbegin()->first);
			else
				commHandler.GetRequests(noTime);


			// We check to see if there are any requests from the experiment

			// Write textbox values
			if (!dataCollection.empty())
				DisplayTextboxValues(dataCollection.rbegin()->second, *experimentStatus);

			// Write the current step
			DisplayStepProgress(*experimentStatus);
		}
		//*****
		// Refresh graph timer
		//*****
		if (nIDEvent == graphTimer)
		{
			if (!dataCollection.empty()) {
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
	connectServer.SetConnection(savedParams.GetServerAddress(), savedParams.GetServerPort());
	connectServer.SetCredentials(savedParams.GetUsername(), savedParams.GetPassword());

	if (connectServer.DoModal() == IDOK)
	{
		std::wstring address{ connectServer.GetAddress() };
		std::wstring port{ connectServer.GetPort() };
		std::wstring username{ connectServer.GetUsername() };
		std::wstring password{ connectServer.GetPassword() };
		
		// First save the address and user/password
		savedParams.SetServerAddress(address);
		savedParams.SetServerPort(port);
		savedParams.SetCredentials(username, password);

		// Then connect to the server
		if (!port.empty())
			commHandler.Connect(address, port);
		else
			commHandler.Connect(address);
		commHandler.SaveAuth(username, password);
		ManualSync(NULL, NULL);
	}

	return 0;
}

LRESULT CKalelView::ManualSync(WPARAM, LPARAM)
{
	commHandler.GetExperimentSettings(noTime);
	commHandler.GetExperimentStatus(noTime);
	commHandler.GetMachineSettings(noTime);
	commHandler.GetControlInstrumentState(noTime);
	return 0;
}


LRESULT CKalelView::DisplayPortDialog(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		ConnectionPort m_connection_ports;
		m_connection_ports.PassSettings(*machineSettings);
		if (m_connection_ports.DoModal() == IDOK) {
			if (m_connection_ports.Changed())
			{
				tempMchSettings.reset(m_connection_ports.GetSettings());			// Save it separately to prevent extra communication
				commHandler.SetMachineSettings(*tempMchSettings);
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
		apparatusParameters.PassSettings(*machineSettings);
		if (apparatusParameters.DoModal() == IDOK) {
			if (apparatusParameters.Changed())
			{
				tempMchSettings.reset(apparatusParameters.GetSettings());			// Save it separately to prevent extra communication
				commHandler.SetMachineSettings(*tempMchSettings);
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
				// Block menu and set running flag
				pApp->experimentRunning = true;
				pApp->menuIsAvailable = false;
				UpdateButtons();

				// Initialise temporary experiment settings
				tempExpSettings = std::make_unique<ExperimentSettings>();
				tempExpSettings->experimentType = EXPERIMENT_TYPE_SAMPLE_VACUUM;
				commHandler.SetExperimentSettings(*tempExpSettings);
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
				// Block menu and set running flag
				pApp->experimentRunning = true;
				pApp->menuIsAvailable = false;
				UpdateButtons();

				// Initialise temporary experiment settings
				tempExpSettings = std::make_unique<ExperimentSettings>();
				tempExpSettings->experimentType = EXPERIMENT_TYPE_BOTTLE_VACUUM;
				commHandler.SetExperimentSettings(*tempExpSettings);
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
				// Block menu and set running flag
				pApp->experimentRunning = true;
				pApp->menuIsAvailable = false;
				UpdateButtons();

				// Initialise temporary experiment settings
				tempExpSettings = std::make_unique<ExperimentSettings>();
				tempExpSettings->experimentType = EXPERIMENT_TYPE_BOTTLE_CHANGE;
				commHandler.SetExperimentSettings(*tempExpSettings);
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
	return 0;
}

LRESULT CKalelView::OnServerDisconnected(WPARAM, LPARAM)
{
	experimentStatus.reset();
	pApp->serverConnected = false;
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();
	return 0;
}

LRESULT CKalelView::OnSync(WPARAM, LPARAM)
{
	if (!pApp->synced) {

		if (--pApp->synced_counter == 0)
		{
			pApp->synced = true;
		}
	}

	// Make sure buttons are updated
	if (experimentStatus && experimentStatus->inProgress) {
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

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnSetMachineSettings(WPARAM, LPARAM)
{
	// Make the temporary local version official
	machineSettings.reset(tempMchSettings.release());
	tempMchSettings.reset();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnExchangeExperimentSettings(WPARAM wParam, LPARAM incomingExperimentSettings)
{
	// Get the incoming pointer
	experimentSettings.reset(reinterpret_cast<ExperimentSettings*>(incomingExperimentSettings));

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnExchangeExperimentStatus(WPARAM wParam, LPARAM incomingExperimentStatus)
{	
	// Get the incoming pointer
	experimentStatus.reset(reinterpret_cast<ExperimentStatus*>(incomingExperimentStatus));

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnSetExperimentSettings(WPARAM, LPARAM)
{
	// Make the temporary local version official
	experimentSettings.reset(tempExpSettings.release());

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}


LRESULT CKalelView::OnExchangeInstrumentState(WPARAM wParam, LPARAM incomingInstrumentState) {

	// Cast the parameters object and take ownership
	std::unique_ptr<ControlInstrumentState> maParam(reinterpret_cast<ControlInstrumentState*>(incomingInstrumentState));
	buttonStates.Update(*maParam);

	// Set time
	machineStateTime = timeh::NowTime();

	// Update GUI
	OnSync(NULL, NULL);

	return 0;
}

LRESULT CKalelView::OnSetInstrumentState(WPARAM wParam, LPARAM incomingInstrumentState) {

	// Cast the parameters object and take ownership
	std::unique_ptr<ControlInstrumentStateData> maParam(reinterpret_cast<ControlInstrumentStateData*>(incomingInstrumentState));

	// Update buttons
	buttonStates.EndCommand(*maParam);

	return 0;
}

LRESULT CKalelView::OnExchangeData(WPARAM, LPARAM incomingExperimentData)
{
	// Get the incoming vector and add it to the local data
	std::unique_ptr<StampedSafeStorage<ExperimentData>::Base> newData(
		reinterpret_cast<StampedSafeStorage<ExperimentData>::Base *>(incomingExperimentData));
	dataCollection.insert(std::make_move_iterator(newData->begin()), 
							std::make_move_iterator(newData->end()));

	return 0;
}

LRESULT CKalelView::OnExchangeLogs(WPARAM, LPARAM incomingLogs)
{
	// Get the incoming vector and add it to the local logs
	std::unique_ptr<StampedSafeStorage<std::wstring>::Base> newLogs(
		reinterpret_cast<StampedSafeStorage<std::wstring>::Base*>(incomingLogs));
	logCollection.insert(newLogs->begin(), newLogs->end());

	// Display logs
	CString * temp = new CString();
	for (auto i = newLogs->begin(); i != newLogs->end(); ++i)
	{
		CString time(timeh::TimePointToShortString(i->first).c_str());
		temp->Append(time + " " + i->second.c_str() + _T("\r\n"));
	}
	AffichageMessages(NULL, (LPARAM)temp);
	
	return 0;
}


LRESULT CKalelView::OnExchangeRequests(WPARAM, LPARAM incomingRequests)
{
	// Get the incoming vector and add it to the local logs
	std::unique_ptr<StampedSafeStorage<std::wstring>::Base> newRequests(
		reinterpret_cast<StampedSafeStorage<std::wstring>::Base*>(incomingRequests));

	for (auto &i : *newRequests)
	{
		auto ret = requestCollection.insert(i);											// attempt to insert it into the map
		if (ret.second) {																// if it was inserted
			commHandler.messageHandler.DisplayMessageBoxServer(i.first, i.second);		// then process it
		}
	}

	return 0;
}


/******************************************
// Message boxes
******************************************/

LRESULT CKalelView::MessageBoxAlert(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::unique_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	std::unique_ptr<UINT> nType(reinterpret_cast<UINT*>(wParam));
	
	int result = AfxMessageBox(*message, *nType);

	return result;
}

LRESULT CKalelView::MessageBoxAlertServer(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointers and cast them as smart pointers
	std::unique_ptr<std::chrono::system_clock::time_point> time(reinterpret_cast<std::chrono::system_clock::time_point*>(wParam));
	std::unique_ptr<std::wstring> message(reinterpret_cast<std::wstring*>(lParam));


	size_t delimiter = message->find(':');
	TLogLevel level = GeneralLog::FromString(message->substr(0, delimiter));
	int result;

	switch (level)
	{
	case logEVENT:
	{
		TQuestionType qtype = GeneralLog::QtypeFromString(message->substr(delimiter + 1, 1));

		UINT ntype;

		switch (qtype)
		{
		case qOK: 
			ntype = MB_OK; 
			break;
		case qYESNO: 
			ntype = MB_YESNO;
			break;
		case qYESCANCEL: 
			ntype = MB_OKCANCEL; 
			break;
		case qYESTRYCANCEL: 
			ntype = MB_CANCELTRYCONTINUE; 
			break;
		default:
			ntype = MB_OK;
			break;
		}

		bool continuer = true;
		do {
			result = AfxMessageBox(message->substr(delimiter + 2 + message->substr(delimiter + 1).find(':')).c_str(), ntype);
			switch (result)
			{
			case IDCONTINUE:
			case IDYES:
			case IDOK:
				commHandler.UserYes(*time);
				continuer = false;
				break;
			case IDNO:
			case IDCANCEL:
				commHandler.UserNo(*time);
				continuer = false;
				break;
			case IDTRYAGAIN:
				commHandler.UserWait(*time);
				continuer = false;
				break;
			default:
				break;
			}
		} while (continuer);
		break;
	}
	case logERROR:
	case logWARNING:
	{
		result = AfxMessageBox(message->c_str(), MB_ICONERROR | MB_OK);
		if (result)
		{
			commHandler.UserYes(*time);
		}
		break;
	}
	case logINFO:
	case logDEBUG:
	case logDEBUG1:
	case logDEBUG2:
	case logDEBUG3:
	case logDEBUG4:
	default:
		break;
	}

	return result;
}