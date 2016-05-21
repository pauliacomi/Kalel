
// KaollaView.cpp : implementation of the CKaollaView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Kaolla.h"
#endif

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "DefinePostMessages.h"				// Message definitions
#include "ExperimentPropertySheet.h"		// The dialog asking the user to input the experiment parameters

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CKaollaApp *pApp;

// CKaollaView

IMPLEMENT_DYNCREATE(CKaollaView, CFormView)

BEGIN_MESSAGE_MAP(CKaollaView, CFormView)
	// Custom messages for threads
	ON_MESSAGE(WM_EXCHANGEDATA, &CKaollaView::ExchangeData)							// Calls to save the incoming data from the thread
	ON_MESSAGE(WM_THREADFINISHEDREG, &CKaollaView::OnRegularThreadFinished)			// Calls when manual functionality ends
	ON_MESSAGE(WM_UPDATEBUTTONS, &CKaollaView::OnThreadRequestButtonUpdate)			// Calls to update a specific button pair and associated display
	ON_MESSAGE(WM_DISPLAYMESSAGE, &CKaollaView::AffichageMessages)					// Displays a message from the automation thread 
	ON_MESSAGE(WM_DISPLAYMESSAGEBOX, &CKaollaView::MessageBoxAlert)					// Displays an messageBOX to alert user of something
	ON_MESSAGE(WM_DISPLAYMESSAGEBOXCONF, &CKaollaView::MessageBoxConfirmation)		// Displays an messageBOX to or ask user for confirmation

	// Messages from the manip class
	//ON_MESSAGE(WM_GRAPHRESET, &CKaollaDoc::InitializeGraph)
	//ON_MESSAGE(WM_GRAPHADDMESUREMENT, &CKaollaDoc::GraphAddMeasurement)
	//ON_MESSAGE(WM_GRAPHSETTITLE, &CKaollaDoc::GraphSetTitle)
	//ON_MESSAGE(WM_GRAPHDATAAUTOSTEP, &CKaollaDoc::Graph)
	//ON_MESSAGE(WM_DISPLAYMEASUREMENT, &CKaollaView::AffichageMesures)
	//ON_MESSAGE(WM_DISPLAYSTEP, &CKaollaView::AffichageEtape)
	
	ON_MESSAGE(WM_DISPLAYADDMESSAGE, &CKaollaView::RajoutAffichageMessages)
	ON_MESSAGE(WM_DISPLAYADDSTEP, &CKaollaView::RajoutAffichageEtape)
	ON_MESSAGE(WM_UPDATEDISPLAY, &CKaollaView::MiseAJour)
	ON_MESSAGE(WM_UNLOCKMENU, &CKaollaView::DebloqueMenu)
	ON_MESSAGE(WM_ENABLESTARTBUTTON, &CKaollaView::UnlockStartButton)
	ON_MESSAGE(WM_CANCELEXPERIMENT, &CKaollaView::Annuler)

	// Messages for UI buttons used for simple instrument manipulation
	ON_BN_CLICKED(IDC_OUVRIR1, &CKaollaView::OnBnClickedOuvrir1)
	ON_BN_CLICKED(IDC_OUVRIR2, &CKaollaView::OnBnClickedOuvrir2)
	ON_BN_CLICKED(IDC_OUVRIR3, &CKaollaView::OnBnClickedOuvrir3)
	ON_BN_CLICKED(IDC_OUVRIR4, &CKaollaView::OnBnClickedOuvrir4)
	ON_BN_CLICKED(IDC_OUVRIR5, &CKaollaView::OnBnClickedOuvrir5)
	ON_BN_CLICKED(IDC_OUVRIR6, &CKaollaView::OnBnClickedOuvrir6)
	ON_BN_CLICKED(IDC_OUVRIR7, &CKaollaView::OnBnClickedOuvrir7)
	ON_BN_CLICKED(IDC_OUVRIR8, &CKaollaView::OnBnClickedOuvrir8)
	ON_BN_CLICKED(IDC_FERMER1, &CKaollaView::OnBnClickedFermer1)
	ON_BN_CLICKED(IDC_FERMER2, &CKaollaView::OnBnClickedFermer2)
	ON_BN_CLICKED(IDC_FERMER3, &CKaollaView::OnBnClickedFermer3)
	ON_BN_CLICKED(IDC_FERMER4, &CKaollaView::OnBnClickedFermer4)
	ON_BN_CLICKED(IDC_FERMER5, &CKaollaView::OnBnClickedFermer5)
	ON_BN_CLICKED(IDC_FERMER6, &CKaollaView::OnBnClickedFermer6)
	ON_BN_CLICKED(IDC_FERMER7, &CKaollaView::OnBnClickedFermer7)
	ON_BN_CLICKED(IDC_FERMER8, &CKaollaView::OnBnClickedFermer8)
	ON_BN_CLICKED(IDC_ACTIVER_EV1, &CKaollaView::OnBnClickedActiverEV1)
	ON_BN_CLICKED(IDC_DESACTIVER_EV1, &CKaollaView::OnBnClickedDesactiverEV1)
	ON_BN_CLICKED(IDC_ACTIVER_EV2, &CKaollaView::OnBnClickedActiverEV2)
	ON_BN_CLICKED(IDC_DESACTIVER_EV2, &CKaollaView::OnBnClickedDesactiverEV2)
	ON_BN_CLICKED(IDC_ACTIVER_POMPE, &CKaollaView::OnBnClickedActiverPompe)
	ON_BN_CLICKED(IDC_DESACTIVER_POMPE, &CKaollaView::OnBnClickedDesactiverPompe)

	// Buttons which are used for automatic/advanced functionality
	ON_BN_CLICKED(IDC_LANCER, &CKaollaView::OnBnClickedLancer)
	ON_BN_CLICKED(IDC_ARRETER, &CKaollaView::OnBnClickedArreter)
	ON_BN_CLICKED(IDC_REPRISE, &CKaollaView::OnBnClickedReprise)
	ON_BN_CLICKED(IDC_ARRET_SOUS_VIDE, &CKaollaView::OnBnClickedArretSousVide)
	ON_BN_CLICKED(IDC_PAUSE, &CKaollaView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_PROCHAINE_COMMANDE, &CKaollaView::OnBnClickedProchaineCommande)
	ON_BN_CLICKED(IDC_PROCHAINE_DOSE, &CKaollaView::OnBnClickedProchaineDose)
	ON_BN_CLICKED(IDC_PROCHAINE_ETAPE, &CKaollaView::OnBnClickedProchaineEtape)

	ON_BN_CLICKED(IDC_BUTTON_PARAMETRES_EXPERIENCE, &CKaollaView::OnBnClickedButtonParametresExperience)

	ON_WM_TIMER()					// timer for update of the values
END_MESSAGE_MAP()

// CKaollaView construction/destruction

CKaollaView::CKaollaView()
	: CFormView(CKaollaView::IDD)
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
{
	
}


CKaollaView::~CKaollaView()
{
	delete threadManager;
	delete experimentSettings;
	delete dialogExperimentProperties;
}

// Liaising between variables and controls
void CKaollaView::DoDataExchange(CDataExchange* pDX)
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

BOOL CKaollaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CKaollaView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();

	// Get a pointer to the app itself for access to the menu availability
	pApp = static_cast<CKaollaApp *>(AfxGetApp());

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

	// Create the experiment parameter window
	dialogExperimentProperties = new ExperimentPropertySheet(_T(""));
	experimentSettings = new ExperimentSettings();						// Create a new experiment storage
	experimentSettings->GUIhandle = GetSafeHwnd();							// Save the window handle
	threadManager = new ThreadManager(GetSafeHwnd(), experimentSettings);;  // the class dealing with managing threads

	// Set the timer for the window update
	SetTimer(1, 100, NULL);
}


// CKaollaView drawing

void CKaollaView::OnDraw(CDC* /*pDC*/)
{
	CKaollaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CKaollaView diagnostics

#ifdef _DEBUG
void CKaollaView::AssertValid() const
{
	CFormView::AssertValid();
}

void CKaollaView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CKaollaDoc* CKaollaView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKaollaDoc)));
	return (CKaollaDoc*)m_pDocument;
}
#endif //_DEBUG


// Returns a pointer to the view itself or NULL if it fails
CKaollaView * CKaollaView::GetView()
{
	CDocTemplate* pTemplate;
	POSITION pos = pApp->GetFirstDocTemplatePosition();
	while (pos != NULL)
	{
		pTemplate = pApp->GetNextDocTemplate(pos);
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
					if (pView->IsKindOf(RUNTIME_CLASS(CKaollaView)))
					{
						CKaollaView* pKV = static_cast<CKaollaView *>(pView);
						return pKV;
					}
				}
			}
		}
	}

	return NULL;
}

// CKaollaView message handlers

void CKaollaView::DoEvents(void)
{
	MSG msg;

	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

LRESULT CKaollaView::DebloqueMenu(WPARAM, LPARAM lParam) // pretty useless, must centralize the experiment running flag
{
	m_mainDocument = CKaollaDoc::GetDocument();
	pApp->menuIsAvailable = !m_mainDocument->experiment_running;

	return 0;
}

LRESULT CKaollaView::MiseAJour(WPARAM wParam, LPARAM lParam)
{
	UpdateData(FALSE);

	return 0;
}

LRESULT CKaollaView::UnlockStartButton(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);

	return 0;
}

void CKaollaView::OnMsvAmpoule(void)
{
	ASSERT(0);
	//MiseSousVideAmpoule(GetSafeHwnd());
}

void CKaollaView::OnMsvBouteille()
{
	ASSERT(0);
	//MiseSousVideBouteille(GetSafeHwnd());
}

void CKaollaView::OnChangementBouteille()
{
	ASSERT(0);
	//ChangementBouteille(GetSafeHwnd());
}


/**********************************************************************************************************************************
// Thread callback commands
**********************************************************************************************************************************/

LRESULT CKaollaView::OnRegularThreadFinished(WPARAM, LPARAM) {

	GetDocument()->experiment_running = FALSE;
	DebloqueMenu(NULL, NULL);
	
	return 0;
}

// When the experiment is signalled as cancelled from the thread or it times out
LRESULT CKaollaView::Annuler(WPARAM, LPARAM)
{
	// Signal that this is the experiment end
	m_mainDocument = CKaollaDoc::GetDocument();
	m_mainDocument->experiment_running = FALSE;  // FALSE : expérience en cours

												 // Unblock the menu
	DebloqueMenu(NULL, NULL);

	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);

	return 0;
}
