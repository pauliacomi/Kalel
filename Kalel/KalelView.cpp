
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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INITIAL_ADSORPTION		3
#define INITIAL_DESORPTIONS		1

// CKalelView

IMPLEMENT_DYNCREATE(CKalelView, CFormView)

BEGIN_MESSAGE_MAP(CKalelView, CFormView)

	//************************************
	// Custom messages
	//************************************

	// Menu messages:
	ON_MESSAGE(UWM_DISP_CONNECTS_DIALOG,			&CKalelView::DisplayConnectDialog)				// Display dialog connection
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
	ON_MESSAGE(UWM_EXCHANGESTATESPECIFIC,			&CKalelView::OnInstrumentButtonConfirmed)		// Calls to update a specific button pair and associated display on a manual message
	ON_MESSAGE(UWM_EXCHANGE_EXPERIMENTSETTINGS,		&CKalelView::OnExchangeExperimentSettings)		// Callback to notify of received ExperimetnSettings
	ON_MESSAGE(UWM_EXCHANGEDATA,					&CKalelView::OnExchangeData)					// Callback to notify of incoming ExperimentData array
	ON_MESSAGE(UWM_EXCHANGELOGS,					&CKalelView::OnExchangeLogs)					// Callback to notify of incoming ExperimentData array
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


	// timer for update of the values
	ON_WM_TIMER()					

END_MESSAGE_MAP()

// CKalelView construction/destruction

CKalelView::CKalelView()
	: CFormView(CKalelView::IDD)
	, buttonStates(*this)
{
}


CKalelView::~CKalelView()
{

}

// Liaising between variables and controls
void CKalelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX,		IDC_LANCER,				m_ButtonLancer			);
	DDX_Control(pDX,		IDC_ARRETER,			m_ButtonArreter			);

	DDX_Control(pDX,		IDC_EDIT_MESSAGES,		pEditMessages			);
	DDX_Text(pDX,			IDC_EDIT_MESSAGES,		m_StrEditMessages		);

	DDX_Control(pDX,		IDC_EDIT_MESURES,		pEditMesures			);
	DDX_Text(pDX,			IDC_EDIT_MESURES,		m_StrEditMesures		);

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


BOOL CKalelView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}


void CKalelView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	// Get a pointer to the app itself for access to the menu availability
	pApp = static_cast<CKalelApp *>(AfxGetApp());

	// Pass window handle to comm handler
	commHandler.SetHandle(GetSafeHwnd());

	// Then connect to the server if the address exists
	commHandler.Connect(savedParams.GetServerAddress());

	// Pass stored array to graph
	GetDocument()->GraphSetArray(dataCollection);
	
	// Initial button set-up
	UpdateButtons();
	
	// Set the timers for the window update
	refrashTimer = SetTimer(1, savedParams.GetDataRefreshInterval(), NULL);
	graphTimer = SetTimer(1, savedParams.GetGraphRefreshInterval(), NULL);
}



// CKalelView drawing

void CKalelView::OnDraw(CDC* /*pDC*/)
{
	CKalelDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}



// CKalelView diagnostics

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
//	Non-standard members
**********************************************************************************************************************************/

void CKalelView::OnTimer(UINT_PTR nIDEvent)
{
	if (!dataCollection.empty()) {

		//*****
		// Refresh data timer
		//*****
		if (nIDEvent == refrashTimer)
		{
			// Write textbox values
			DisplayTextboxValues(dataCollection.back());

			// Write the current step
			DisplayStepProgress(dataCollection.back());

			if (pApp->serverConnected)
			{
				commHandler.GetData(dataCollection.back()->timestamp);
				commHandler.GetLog(logCollection.rbegin()->first);
			}
		}
		
		//*****
		// Refresh graph timer
		//*****
		if (nIDEvent == graphTimer)
		{
			// Write in measurement box
			DiplayMeasurements(dataCollection.back());

			// Write graph
			GetDocument()->UpdateAllViews(this);
		}
	}

	CFormView::OnTimer(nIDEvent);	// Call base class handler.
}

// Copy all data from a property sheet dialog to the local object
void CKalelView::GetExperimentData(ExperimentPropertySheet * pDialogExperimentProperties, bool initialRequest) {

	if (initialRequest)
	{
		// Copy data across
		ReplaceExperimentSettings(pDialogExperimentProperties);
	}

	else
	{
		// Must check if everything is the same

		bool modified = false;

		if (pDialogExperimentProperties->adsorptionTabs.size() != experimentSettings->dataAdsorption.size()
			&& pDialogExperimentProperties->desorptionTabs.size() != experimentSettings->dataDesorption.size())
		{
			modified = true;
		}
		else
		{
			if (pDialogExperimentProperties->m_general.allSettings != experimentSettings->dataGeneral)
			{
				modified = true;
			}

			if (pDialogExperimentProperties->m_divers.allSettings != experimentSettings->dataDivers)
			{
				modified = true;
			}

			for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
			{
				if (pDialogExperimentProperties->adsorptionTabs[i]->allSettings != experimentSettings->dataAdsorption[i])
				{
					modified = true;
				}
			}
			for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
			{
				if (pDialogExperimentProperties->desorptionTabs[i]->allSettings != experimentSettings->dataDesorption[i])
				{
					modified = true;
				}
			}
		}

		if (modified)
		{
			// Copy data across
			ReplaceExperimentSettings(pDialogExperimentProperties);

			// Raise the flag for data modified
			commHandler.SetExperimentSettings(experimentSettings);
		}
	}
}

void CKalelView::ReplaceExperimentSettings(ExperimentPropertySheet* pDialogExperimentProperties)
{
	experimentSettings->dataGeneral = pDialogExperimentProperties->m_general.allSettings;

	if (experimentSettings->experimentType == EXPERIMENT_TYPE_AUTO)
	{
		experimentSettings->dataDivers = pDialogExperimentProperties->m_divers.allSettings;

		experimentSettings->dataAdsorption.clear();
		for (size_t i = 0; i < pDialogExperimentProperties->adsorptionTabs.size(); i++)
		{
			experimentSettings->dataAdsorption.push_back(pDialogExperimentProperties->adsorptionTabs[i]->allSettings);
		}
		experimentSettings->dataDesorption.clear();
		for (size_t i = 0; i < pDialogExperimentProperties->desorptionTabs.size(); i++)
		{
			experimentSettings->dataDesorption.push_back(pDialogExperimentProperties->desorptionTabs[i]->allSettings);
		}
	}
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

LRESULT CKalelView::DisplayPortDialog(WPARAM, LPARAM)
{
	if (pApp->serverConnected)
	{
		ApparatusParameters apparatusParameters;
		apparatusParameters.PassSettings(machineSettings.get());
		if (apparatusParameters.DoModal() == IDOK) {
			if (apparatusParameters.Changed())
			{
				tempSettings = apparatusParameters.GetSettings();			// Save it separately to prevent extra communication
				commHandler.SetMachineSettings(tempSettings);
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
		ConnectionPort m_connection_ports;
		m_connection_ports.PassSettings(machineSettings.get());
		if (m_connection_ports.DoModal() == IDOK) {
			if (m_connection_ports.Changed())
			{
				tempSettings = m_connection_ports.GetSettings();			// Save it separately to prevent extra communication
				commHandler.SetMachineSettings(tempSettings);
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

				// the start button is blocked
				GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);
				// the stop button is activated
				GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);

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

				// the start button is blocked
				GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);
				// the stop button is activated
				GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);

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

				// the start button is blocked
				GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);
				// the stop button is activated
				GetDlgItem(IDC_ARRETER)->EnableWindow(TRUE);

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
	commHandler.SaveAddress(savedParams.GetServerAddress());
	commHandler.Sync();
	return 0;
}

LRESULT CKalelView::OnServerDisconnected(WPARAM, LPARAM)
{
	pApp->serverConnected = false;
	return 0;
}


LRESULT CKalelView::OnSync(WPARAM, LPARAM)
{
	// unlock the menu
	pApp->menuIsAvailable = true;

	return 0;
}

LRESULT CKalelView::OnSetMachineSettings(WPARAM, LPARAM)
{
	// Make the local version official
	machineSettings = tempSettings;
	tempSettings.reset();

	return 0;
}

LRESULT CKalelView::OnExchangeMachineSettings(WPARAM, LPARAM incomingMachineSettings)
{
	// Get the incoming pointer
	machineSettings.reset(reinterpret_cast<MachineSettings*>(incomingMachineSettings));

	return 0;
}

LRESULT CKalelView::OnExchangeExperimentSettings(WPARAM wParam, LPARAM incomingExperimentSettings)
{
	// Get the incoming pointer
	experimentSettings.reset(reinterpret_cast<ExperimentSettings*>(incomingExperimentSettings));

	return 0;
}

LRESULT CKalelView::OnExchangeInstrumentState(WPARAM wParam, LPARAM incomingInstrumentState) {

	// Cast the parameters object and take ownership
	std::auto_ptr<ControlInstrumentState> maParam(reinterpret_cast<ControlInstrumentState*>(incomingInstrumentState));

	// Update buttons
	buttonStates.Update(*maParam);

	return 0;
}

LRESULT CKalelView::OnInstrumentButtonConfirmed(WPARAM wParam, LPARAM incomingInstrumentState) {

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
	auto newData = reinterpret_cast<std::deque<std::shared_ptr<ExperimentData>>*>(incomingExperimentData);
	dataCollection.insert(dataCollection.end(), newData->begin(), newData->end());

	// Delete the useless vector now
	delete newData;

	return 0;
}

LRESULT CKalelView::OnExchangeLogs(WPARAM, LPARAM incomingLogs)
{
	// Get the incoming vector and add it to the local logs
	std::map<std::wstring, std::wstring> * newLogs = reinterpret_cast<std::map<std::wstring, std::wstring>*>(incomingLogs);
	logCollection.insert(newLogs->begin(), newLogs->end());

	// Display logs
	for (std::map<std::wstring, std::wstring>::iterator i = newLogs->begin(); i != newLogs->end(); i++)
	{
		CString time = CString(i->first.c_str());
		CString log = CString(i->second.c_str());
		CString * temp = new CString();
		*temp = time + log;
		AffichageMessages(NULL, (LPARAM)temp);
	}
	
	//lastLog = newLogs->end()->first;

	// Delete the useless vector now
	delete newLogs;

	return 0;
}


LRESULT CKalelView::OnAutoExperimentFinished(WPARAM, LPARAM) {

	experimentSettings->ResetData();

	// Signal that this is the experiment end
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);
	
	return 0;
}

// When the experiment is signalled as cancelled from the thread or it times out
LRESULT CKalelView::CancelBeforeStarting(WPARAM, LPARAM)
{
	// Signal that this is the experiment end
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);

	return 0;
}

LRESULT CKalelView::MessageBoxAlert(WPARAM wParam, LPARAM lParam)
{
	// Get the incoming pointer and cast it as a smart pointer
	std::auto_ptr<CString> message(reinterpret_cast<CString*>(lParam));
	std::auto_ptr<UINT> nType(reinterpret_cast<UINT*>(wParam));
	
	return AfxMessageBox(*message, *nType);
}

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
				commHandler.SetUserContinue();
				continuer = false;
			}
		}
	} while (continuer);
	return result;
}