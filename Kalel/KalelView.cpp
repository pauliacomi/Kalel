
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
#include "Com Classes\ManualActionParam.h"
#include "ListOfInstrumentButtons.h"								// The class containing a list of the instruments' buttons ID's

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

	// Server requests
	ON_MESSAGE(UWM_FUNC_VACUUM_SAMPLE, &CKalelView::OnMsvAmpoule)							// Request sample under vacuum
	ON_MESSAGE(UWM_FUNC_VACUUM_BOTTLE, &CKalelView::OnMsvBouteille)							// Request bottle under vacuum
	ON_MESSAGE(UWM_FUNC_CHANGE_BOTTLE, &CKalelView::OnChangementBouteille)					// Request bottle change procedure
	ON_MESSAGE(UWM_THREAD_START, &CKalelView::BackgroundThreadStart)						// Request thread shutdown
	ON_MESSAGE(UWM_THREAD_STOP, &CKalelView::BackgroundThreadStop)							// Request thread stop
	ON_MESSAGE(UWM_THREAD_RESTART, &CKalelView::BackgroundThreadRestart)					// Request thread restart

	// Server callbacks:
	ON_MESSAGE(UWM_SIGNAL_SERVER_CONNECTED, &CKalelView::OnServerConnected)					// Callback to notify of successful server connection
	ON_MESSAGE(UWM_SYNCED, &CKalelView::OnSetMachineSettings)												// Modifies the global ??????????????
	ON_MESSAGE(UWM_GOT_MACHINE_SETTINGS, &CKalelView::OnGetMachineSettings)					// Callback to notify of received MachineSettings
	ON_MESSAGE(UWM_EXCHANGEDATA, &CKalelView::OnExchangeData)								// Callback to notify of incoming ExperimentData array
	ON_MESSAGE(UWM_THREADFINISHEDREG, &CKalelView::OnAutoExperimentFinished)				// Calls when manual functionality ends
	ON_MESSAGE(UWM_DISPLAYMESSAGE, &CKalelView::AffichageMessages)							// Callback to display a message from the automation thread
	ON_MESSAGE(UWM_DISPLAYMESSAGEBOX, &CKalelView::MessageBoxAlert)							// Displays an messageBox to alert user of something
	ON_MESSAGE(UWM_DISPLAYMESSAGEBOXCONF, &CKalelView::MessageBoxConfirmation)				// Displays an messageBox to or ask user for confirmation
	ON_MESSAGE(UWM_CANCELEXPERIMENT, &CKalelView::CancelBeforeStarting)						
	ON_MESSAGE(UWM_UPDATEBUTTONS, &CKalelView::OnThreadRequestButtonUpdate)					// Calls to update a specific button pair and associated display on a manual message
																							
	// Menu messages:
	ON_MESSAGE(UWM_DISP_CONNECTS_DIALOG, &CKalelView::DisplayConnectDialog)					// Display dialog connection
	ON_MESSAGE(UWM_DISP_PORT_DIALOG, &CKalelView::DisplayPortDialog)						// Display dialog ports
	ON_MESSAGE(UWM_DISP_DEVSETTINGS_DIALOG, &CKalelView::DisplayApparatusSettingsDialog)	// Display dialog machine parameters
	
	//************************************
	// Standard messages
	//************************************

	// Buttons which are used for automatic/advanced functionality
	ON_BN_CLICKED(IDC_LANCER, &CKalelView::OnBnClickedLancer)
	ON_BN_CLICKED(IDC_ARRETER, &CKalelView::OnBnClickedArreter)
	ON_BN_CLICKED(IDC_REPRISE, &CKalelView::OnBnClickedReprise)
	ON_BN_CLICKED(IDC_ARRET_SOUS_VIDE, &CKalelView::OnBnClickedArretSousVide)
	ON_BN_CLICKED(IDC_PAUSE, &CKalelView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_PROCHAINE_COMMANDE, &CKalelView::OnBnClickedProchaineCommande)
	ON_BN_CLICKED(IDC_PROCHAINE_DOSE, &CKalelView::OnBnClickedProchaineDose)
	ON_BN_CLICKED(IDC_PROCHAINE_ETAPE, &CKalelView::OnBnClickedProchaineEtape)
	ON_BN_CLICKED(IDC_BUTTON_PARAMETRES_EXPERIENCE, &CKalelView::OnBnClickedButtonParametresExperience)
	
	// Messages for UI buttons used for simple instrument manipulation
	ON_BN_CLICKED(IDC_OUVRIR1, &CKalelView::OnBnClickedOuvrir1)
	ON_BN_CLICKED(IDC_OUVRIR2, &CKalelView::OnBnClickedOuvrir2)
	ON_BN_CLICKED(IDC_OUVRIR3, &CKalelView::OnBnClickedOuvrir3)
	ON_BN_CLICKED(IDC_OUVRIR4, &CKalelView::OnBnClickedOuvrir4)
	ON_BN_CLICKED(IDC_OUVRIR5, &CKalelView::OnBnClickedOuvrir5)
	ON_BN_CLICKED(IDC_OUVRIR6, &CKalelView::OnBnClickedOuvrir6)
	ON_BN_CLICKED(IDC_OUVRIR7, &CKalelView::OnBnClickedOuvrir7)
	ON_BN_CLICKED(IDC_OUVRIR8, &CKalelView::OnBnClickedOuvrir8)
	ON_BN_CLICKED(IDC_FERMER1, &CKalelView::OnBnClickedFermer1)
	ON_BN_CLICKED(IDC_FERMER2, &CKalelView::OnBnClickedFermer2)
	ON_BN_CLICKED(IDC_FERMER3, &CKalelView::OnBnClickedFermer3)
	ON_BN_CLICKED(IDC_FERMER4, &CKalelView::OnBnClickedFermer4)
	ON_BN_CLICKED(IDC_FERMER5, &CKalelView::OnBnClickedFermer5)
	ON_BN_CLICKED(IDC_FERMER6, &CKalelView::OnBnClickedFermer6)
	ON_BN_CLICKED(IDC_FERMER7, &CKalelView::OnBnClickedFermer7)
	ON_BN_CLICKED(IDC_FERMER8, &CKalelView::OnBnClickedFermer8)
	ON_BN_CLICKED(IDC_ACTIVER_EV1, &CKalelView::OnBnClickedActiverEV1)
	ON_BN_CLICKED(IDC_DESACTIVER_EV1, &CKalelView::OnBnClickedDesactiverEV1)
	ON_BN_CLICKED(IDC_ACTIVER_EV2, &CKalelView::OnBnClickedActiverEV2)
	ON_BN_CLICKED(IDC_DESACTIVER_EV2, &CKalelView::OnBnClickedDesactiverEV2)
	ON_BN_CLICKED(IDC_ACTIVER_POMPE, &CKalelView::OnBnClickedActiverPompe)
	ON_BN_CLICKED(IDC_DESACTIVER_POMPE, &CKalelView::OnBnClickedDesactiverPompe)


	// timer for update of the values
	ON_WM_TIMER()					

END_MESSAGE_MAP()

// CKalelView construction/destruction

CKalelView::CKalelView()
	: CFormView(CKalelView::IDD)

	, experimentSettings{ new ExperimentSettings(INITIAL_ADSORPTION, INITIAL_DESORPTIONS) }
	, machineSettings{ new MachineSettings() }
{
}


CKalelView::~CKalelView()
{

}

// Liaising between variables and controls
void CKalelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LANCER, m_ButtonLancer);
	DDX_Control(pDX, IDC_ARRETER, m_ButtonArreter);

	DDX_Control(pDX, IDC_EDIT_MESSAGES, pEditMessages);
	DDX_Text(pDX, IDC_EDIT_MESSAGES, m_StrEditMessages);

	DDX_Control(pDX, IDC_EDIT_MESURES, pEditMesures);
	DDX_Text(pDX, IDC_EDIT_MESURES, m_StrEditMesures);

	DDX_Text(pDX, IDC_CALO, m_StrCalo);
	DDX_Text(pDX, IDC_BASSE_PRESSION, m_StrBassePression);
	DDX_Text(pDX, IDC_HAUTE_PRESSION, m_StrHautePression);
	DDX_Text(pDX, IDC_TEMPERATURE_CALO, m_StrTemperatureCalo);
	DDX_Text(pDX, IDC_TEMPERATURE_CAGE, m_StrTemperatureCage);
	DDX_Text(pDX, IDC_TEMPERATURE_PIECE, m_StrTemperaturePiece);
	DDX_Text(pDX, IDC_TEMPS, m_StrTemps);
	DDX_Text(pDX, IDC_PRESSION_INITIALE, m_StrPressionInitiale);
	DDX_Text(pDX, IDC_PRESSION_FINALE, m_StrPressionFinale);
	DDX_Text(pDX, IDC_EDIT_ETAPE, m_StrEtape);
	DDX_Text(pDX, IDC_TEMOIN_VANNE1, m_StrTemoinVanne1);
	DDX_Text(pDX, IDC_TEMOIN_VANNE2, m_StrTemoinVanne2);
	DDX_Text(pDX, IDC_TEMOIN_VANNE3, m_StrTemoinVanne3);
	DDX_Text(pDX, IDC_TEMOIN_VANNE4, m_StrTemoinVanne4);
	DDX_Text(pDX, IDC_TEMOIN_VANNE5, m_StrTemoinVanne5);
	DDX_Text(pDX, IDC_TEMOIN_VANNE6, m_StrTemoinVanne6);
	DDX_Text(pDX, IDC_TEMOIN_VANNE7, m_StrTemoinVanne7);
	DDX_Text(pDX, IDC_TEMOIN_VANNE8, m_StrTemoinVanne8);
	DDX_Text(pDX, IDC_TEMOIN_EV1, m_StrTemoinEV1);
	DDX_Text(pDX, IDC_TEMOIN_EV2, m_StrTemoinEV2);
	DDX_Text(pDX, IDC_TEMOIN_POMPE, m_StrTemoinPompe);
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

	// Pass window handle
	commHandler.SetHandle(GetSafeHwnd());

	// Then connect to the server if the address exists
	commHandler.Connect(savedParams.GetServerAddress());

	// Pass stored array to graph
	GetDocument()->GraphSetArray(dataCollection);

	// Deactivate the buttons that should not be available
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER1)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER2)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER3)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER4)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER5)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER6)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER7)->EnableWindow(FALSE);
	GetDlgItem(IDC_FERMER8)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);
	
	// Initial button set-up
	UpdateButtons();
	
	// Set the timer for the window update
	SetTimer(1, 100, NULL);
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

void CKalelView::OnTimer(UINT nIDEvent)
{
	if (!dataCollection.empty()) {

		// Write textbox values
		DisplayTextboxValues(dataCollection.back());

		// Write the current step
		DisplayStepProgress(dataCollection.back());

		// Write in measurement box
		DiplayMeasurements(dataCollection.back());

		// Write graph
		GetDocument()->UpdateAllViews(this);

		if (pApp->serverConnected)
		{
			commHandler.GetData(dataCollection.back()->timeStart, dataCollection.back()->measurementsMade);
			//commHandler.GetLog();
		}
	}


	CFormView::OnTimer(nIDEvent);	// Call base class handler.
}


// Copy all data from a property sheet dialog to the local object
void CKalelView::GetExperimentData(ExperimentPropertySheet * pDialogExperimentProperties, bool initialRequest) {

	if (initialRequest)
	{
		// Copy data across
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
			// Raise the flag for data modified
			commHandler.SetExperimentSettings(experimentSettings);

			// Copy data across
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
	}

}


/**********************************************************************************************************************************
// Menu functionality
**********************************************************************************************************************************/

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

				// Raise the flag for data modified
				// commHandler.SetExperimentSettings();
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
				//commHandler.SetExperimentSettings();
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
				//commHandler.SetExperimentSettings();
			}
		}
	}
	else
	{
		AfxMessageBox(ERROR_CONNECTION_STATUS, MB_OK);
	}

	return 0;
}

LRESULT CKalelView::DisplayConnectDialog(WPARAM, LPARAM)
{
	DialogConnectServer connectServer;
	if (connectServer.DoModal() == IDOK)
	{
		std::wstring address{ connectServer.GetAddress() };
		
		// First set the address
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
				machineSettings->synced = false;
				tempSettings = apparatusParameters.GetSettings();
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
				machineSettings->synced = false;
				tempSettings = m_connection_ports.GetSettings();
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



/**********************************************************************************************************************************
// Server callback commands
**********************************************************************************************************************************/

LRESULT CKalelView::OnServerConnected(WPARAM wParam, LPARAM lParam)
{
	pApp->serverConnected = true;
	commHandler.SaveAddress(savedParams.GetServerAddress());
	commHandler.Sync();
	return 0;
}

LRESULT CKalelView::OnGetMachineSettings(WPARAM wParam, LPARAM incomingMachineSettings)
{
	// Get the incoming pointer
	machineSettings.reset(reinterpret_cast<MachineSettings*>(incomingMachineSettings));

	return 0;
}

LRESULT CKalelView::OnSetMachineSettings(WPARAM wParam, LPARAM lParam)
{
	if (machineSettings->synced != true) {
		machineSettings = tempSettings;
		tempSettings.reset();
		machineSettings->synced = true;
	}

	return 0;
}

LRESULT CKalelView::OnExchangeData(WPARAM, LPARAM incomingExperimentData)
{
	// Get the incoming vector and add it to the local data
	MeasurementsArray * newData = reinterpret_cast<MeasurementsArray*>(incomingExperimentData);
	dataCollection.insert(dataCollection.end(), newData->begin(), newData->end());

	// Delete the useless vector now
	delete newData;

	return 0;
}

LRESULT CKalelView::OnExchangeLogs(WPARAM, LPARAM incomingLogs)
{
	// Get the incoming vector and add it to the local logs
	std::deque<std::string *> * newLogs = reinterpret_cast<std::deque<std::string *>*>(incomingLogs);
	logCollection.insert(logCollection.end(), newLogs->begin(), newLogs->end());

	for (size_t i = 0; i < newLogs->size(); i++)
	{
		CString * temp = new CString(newLogs->at(i)->c_str());
		AffichageMessages(NULL, (LPARAM)temp);
	}

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



// Single function to update UI when receiving the command that the thread posted before finishing
LRESULT CKalelView::OnThreadRequestButtonUpdate(WPARAM wParam, LPARAM lParam) {

	// Cast the parameters object and take ownership
	std::auto_ptr<ManualActionParam> maParam(reinterpret_cast<ManualActionParam*>(wParam));

	// Create a new list object
	ListOfInstrumentButtons list(maParam->instrumentType, maParam->instrumentNumber, maParam->shouldBeActivated);

	CString message;

	if (lParam) {
		// Disable required button
		GetDlgItem(list.GetButtonID())->EnableWindow(FALSE);

		// Enable required button
		GetDlgItem(list.GetOppositeButtonID())->EnableWindow(TRUE);

		// Write message in the textbox
		SetDlgItemText(list.GetTextboxID(), list.GetTextboxMessage());
	}
	else
	{
		CString * temp = new CString(list.GetTempTextboxMessage());
		AffichageMessages(0, (LPARAM)temp);
	}

	// unlock the menu
	pApp->menuIsAvailable = true;

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