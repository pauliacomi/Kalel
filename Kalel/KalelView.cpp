
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

#include "DefinePostMessages.h"				// Message definitions
#include "ExperimentPropertySheet.h"		// The dialog asking the user to input the experiment parameters

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKalelView

IMPLEMENT_DYNCREATE(CKalelView, CFormView)

BEGIN_MESSAGE_MAP(CKalelView, CFormView)
	// Custom messages for threads
	ON_MESSAGE(WM_EXCHANGEDATA, &CKalelView::ExchangeData)							// Calls to save the incoming data from the thread
	ON_MESSAGE(WM_THREADFINISHEDREG, &CKalelView::OnRegularThreadFinished)			// Calls when manual functionality ends
	ON_MESSAGE(WM_UPDATEBUTTONS, &CKalelView::OnThreadRequestButtonUpdate)			// Calls to update a specific button pair and associated display
	ON_MESSAGE(WM_DISPLAYMESSAGE, &CKalelView::AffichageMessages)					// Displays a message from the automation thread 
	ON_MESSAGE(WM_DISPLAYMESSAGEBOX, &CKalelView::MessageBoxAlert)					// Displays an messageBOX to alert user of something
	ON_MESSAGE(WM_DISPLAYMESSAGEBOXCONF, &CKalelView::MessageBoxConfirmation)		// Displays an messageBOX to or ask user for confirmation
	ON_MESSAGE(WM_DISPLAYMESSAGEBOXSTOPEX, &CKalelView::MessageBoxStopExperiment)		// Displays an messageBOX to or ask user for confirmation
	//ON_MESSAGE(WM_GRAPHRESET, &CKalelDoc::GraphReset)
	//ON_MESSAGE(WM_GRAPHDATAAUTOSTEP, &CKalelDoc::Graph)
	//ON_MESSAGE(WM_DISPLAYMEASUREMENT, &CKalelView::AffichageMesures)
	//ON_MESSAGE(WM_DISPLAYSTEP, &CKalelView::AffichageEtape)
	//ON_MESSAGE(WM_UNLOCKMENU, &CKalelView::DebloqueMenu)
	//ON_MESSAGE(WM_UPDATEDISPLAY, &CKalelView::MiseAJour)
	//ON_MESSAGE(WM_ENABLESTARTBUTTON, &CKalelView::UnlockStartButton)
	
	ON_MESSAGE(WM_DISPLAYADDMESSAGE, &CKalelView::RajoutAffichageMessages)
	ON_MESSAGE(WM_DISPLAYADDSTEP, &CKalelView::RajoutAffichageEtape)
	ON_MESSAGE(WM_CANCELEXPERIMENT, &CKalelView::Annuler)

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

	ON_WM_TIMER()					// timer for update of the values

	ON_COMMAND(ID_BACKGROUNDTHREAD_START, &CKalelView::OnBackgroundthreadStart)
	ON_COMMAND(ID_BACKGROUNDTHREAD_STOP, &CKalelView::OnBackgroundthreadStop)
	ON_COMMAND(ID_BACKGROUNDTHREAD_RESTART, &CKalelView::OnBackgroundthreadRestart)
END_MESSAGE_MAP()

// CKalelView construction/destruction

CKalelView::CKalelView()
	: CFormView(CKalelView::IDD)
	, m_StrEditMessages(_T(""))
	, m_StrCalo(_T(""))
	, m_StrBassePression(_T(""))
	, m_StrHautePression(_T(""))
	, m_StrTemperatureCalo(_T(""))
	, m_StrTemperatureCage(_T(""))
	, m_StrTemperaturePiece(_T(""))
	, m_StrTemps(_T(""))
	, m_StrEditMesures(_T(""))
	, m_StrPressionInitiale(_T(""))
	, m_StrPressionFinale(_T(""))
	, m_StrEtape(_T(""))
	, m_StrTemoinVanne1(_T(""))
	, m_StrTemoinVanne2(_T(""))
	, m_StrTemoinVanne3(_T(""))
	, m_StrTemoinVanne4(_T(""))
	, m_StrTemoinVanne5(_T(""))
	, m_StrTemoinVanne6(_T(""))
	, m_StrTemoinVanne7(_T(""))
	, m_StrTemoinVanne8(_T(""))
	, m_StrTemoinEV1(_T(""))
	, m_StrTemoinEV2(_T(""))
	, m_StrTemoinPompe(_T(""))
	, experimentData(NULL)
{
}


CKalelView::~CKalelView()
{
	delete threadManager;
	delete experimentSettings;
}

// Liaising between variables and controls
void CKalelView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MESSAGES, m_StrEditMessages);
	DDX_Control(pDX, IDC_EDIT_MESSAGES, pEditMessages);
	DDX_Text(pDX, IDC_CALO, m_StrCalo);
	DDX_Text(pDX, IDC_BASSE_PRESSION, m_StrBassePression);
	DDX_Text(pDX, IDC_HAUTE_PRESSION, m_StrHautePression);
	DDX_Text(pDX, IDC_TEMPERATURE_CALO, m_StrTemperatureCalo);
	DDX_Text(pDX, IDC_TEMPERATURE_CAGE, m_StrTemperatureCage);
	DDX_Text(pDX, IDC_TEMPERATURE_PIECE, m_StrTemperaturePiece);
	DDX_Text(pDX, IDC_TEMPS, m_StrTemps);
	DDX_Control(pDX, IDC_LANCER, m_ButtonLancer);
	DDX_Control(pDX, IDC_ARRETER, m_ButtonArreter);
	DDX_Control(pDX, IDC_EDIT_MESURES, pEditMesures);
	DDX_Text(pDX, IDC_EDIT_MESURES, m_StrEditMesures);
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

	// Check to see whether the parameters file has been created
	VerifParametres();
	
	// Initial button set-up
	UpdateButtons();

	// Create the experiment parameter window
	int initialAdsorptions = 3;
	int initialDesorptions = 1;
	experimentSettings = new ExperimentSettings(initialAdsorptions, initialDesorptions);			// Create a new experiment storage
	experimentSettings->GUIhandle = GetSafeHwnd();													// Save the window handle

	threadManager = new ThreadManager(experimentSettings);											// the class dealing with managing threads

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

// CKalelView message handlers

void CKalelView::DoEvents(void)
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void CKalelView::OnMsvAmpoule(void)
{
	ASSERT(0);
	//MiseSousVideAmpoule(GetSafeHwnd());
}

void CKalelView::OnMsvBouteille()
{
	ASSERT(0);
	//MiseSousVideBouteille(GetSafeHwnd());
}

void CKalelView::OnChangementBouteille()
{
	ASSERT(0);
	//ChangementBouteille(GetSafeHwnd());
}

void CKalelView::DisplayPortDialog(void)
{
	ApparatusParameters apparatusParameters;
	apparatusParameters.DoModal();
}

void CKalelView::DisplayApparatusSettingsDialog(void)
{
	ConnectionPort m_connection_ports;
	m_connection_ports.DoModal();
}


/**********************************************************************************************************************************
// Thread callback commands
**********************************************************************************************************************************/

LRESULT CKalelView::OnRegularThreadFinished(WPARAM, LPARAM) {

	EnterCriticalSection(&experimentSettings->criticalSection);
	experimentSettings->ResetData();
	LeaveCriticalSection(&experimentSettings->criticalSection);

	// Signal that this is the experiment end
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);
	
	return 0;
}

// When the experiment is signalled as cancelled from the thread or it times out
LRESULT CKalelView::Annuler(WPARAM, LPARAM)
{
	EnterCriticalSection(&experimentSettings->criticalSection);
	experimentSettings->ResetData();
	LeaveCriticalSection(&experimentSettings->criticalSection);

	// Signal that this is the experiment end
	pApp->experimentRunning = false;
	pApp->menuIsAvailable = true;
	UpdateButtons();

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);

	return 0;
}


void CKalelView::OnBackgroundthreadStart()
{
	threadManager->StartThread();
}


void CKalelView::OnBackgroundthreadStop()
{
	threadManager->ShutdownThread();
}


void CKalelView::OnBackgroundthreadRestart()
{
	threadManager->ShutdownThread();
	threadManager->StartThread();
}
