
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
#include "threads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CKaollaApp *pApp;

//Pour donner la date exacte
const char * NomJourSemaine[] = { "Dimanche", "Lundi", "Mardi", "Mercredi", "Jeudi", "Vendredi", "Samedi" };

const char * NomMois[] = { "janvier", "fevrier", "mars", "avril", "mai","juin",
						"juillet", "aout", "septembre", "octobre", "novembre", "decembre" };


// CKaollaView

IMPLEMENT_DYNCREATE(CKaollaView, CFormView)

BEGIN_MESSAGE_MAP(CKaollaView, CFormView)
	ON_MESSAGE(WM_THREADAFFICHAGE, &CKaollaView::OnThreadAffichage)
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
	ON_BN_CLICKED(IDC_LANCER, &CKaollaView::OnBnClickedLancer)
	ON_BN_CLICKED(IDC_ARRETER, &CKaollaView::OnBnClickedArreter)
	ON_BN_CLICKED(IDC_REPRISE, &CKaollaView::OnBnClickedReprise)
	ON_BN_CLICKED(IDC_ARRET_SOUS_VIDE, &CKaollaView::OnBnClickedArretSousVide)
	ON_BN_CLICKED(IDC_PAUSE, &CKaollaView::OnBnClickedPause)
	ON_BN_CLICKED(IDC_PROCHAINE_COMMANDE, &CKaollaView::OnBnClickedProchaineCommande)
	ON_BN_CLICKED(IDC_PROCHAINE_DOSE, &CKaollaView::OnBnClickedProchaineDose)
	ON_BN_CLICKED(IDC_PROCHAINE_ETAPE, &CKaollaView::OnBnClickedProchaineEtape)

	ON_BN_CLICKED(IDC_BUTTON_PARAMETRES_EXPERIENCE, &CKaollaView::OnBnClickedButtonParametresExperience)

	/*	ON_COMMAND(ID_MSV_AMPOULE, &CKaollaView::OnMsvAmpoule)
	ON_UPDATE_COMMAND_UI(ID_MSV_AMPOULE, &CKaollaView::OnUpdateMsvAmpoule)*/
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
	, TemperatureCalo(0)
	, TemperatureCage(0)
	, TemperaturePiece(0)
	, Calorimetre(0)
	, Basse_Pression(0)
	, Haute_Pression(0)
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
	// On initialise à vide car toutes les variables se rempliront
	// Au fur et à mesure de l'expérience
	m_parametres_appareil = new CParametres_appareil(this);
	m_connection_port = new CConnection_port(this);
}

CKaollaView::~CKaollaView()
{
	// On détruit les boîtes de dialogue
	delete m_parametres_appareil;
	delete m_connection_port;

	DeleteManip();
}

// Liaison entre les variables et les contrôles
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
	//ResizeParentToFit();

	// En attendant que tout soit prêt
	// On bloque les boutons (lancer, arrêter et les 'ouvrir' et 'fermer')
	// Et lors de l'initialisations, on ferment les vannes
	GetDlgItem(IDC_LANCER)->EnableWindow(FALSE);
	for (int i = 1; i <= 8; i++)
	{
		GetDlgItem(idc_fermer[i - 1])->EnableWindow(FALSE);
		GetDlgItem(idc_ouvrir[i - 1])->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);

	GetDlgItem(IDC_ARRETER)->EnableWindow(FALSE);

	VerifParametres();//////////////////////////////////////////////////////////////

	InitialisationManip();

	pApp = static_cast<CKaollaApp *>(AfxGetApp());
	//AfxBeginThread(LancerThreadProc, GetSafeHwnd());
	/*
	AfxBeginThread(ThreadMenuMiseSousVideAmpoule, GetSafeHwnd());
	AfxBeginThread(ThreadMenuMiseSousVideBouteille, GetSafeHwnd());
	*/
	LancementThreadMSVAmpoule();
	LancementThreadMSVBouteille();
	LancementThreadChangementBouteille();

	// Tout est prêt, le bouton "Lancer" et les boutons 'Ouvrir'
	// sont donc accessibles
	for (int i = 1; i <= 8; i++)
	{
		GetDlgItem(idc_ouvrir[i - 1])->EnableWindow(TRUE);
	}
	GetDlgItem(IDC_ACTIVER_EV1)->EnableWindow(TRUE);
	GetDlgItem(IDC_ACTIVER_EV2)->EnableWindow(TRUE);
	GetDlgItem(IDC_ACTIVER_POMPE)->EnableWindow(TRUE);
	GetDlgItem(IDC_LANCER)->EnableWindow(TRUE);
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


// Returns a pointer to the view itself
CKaollaView * CKaollaView::GetView()
{
	CFrameWnd * pFrame = (CFrameWnd *)(AfxGetApp()->m_pMainWnd);

	CView * pView = pFrame->GetActiveView();

	if (!pView)
		return NULL;

	// Fail if view is of wrong kind
	// (this could occur with splitter windows, or additional
	// views on a single document
	if (!pView->IsKindOf(RUNTIME_CLASS(CKaollaView)))
		return NULL;

	return (CKaollaView *)pView;
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


void CKaollaView::ChargerDocument(void)
{
	GetDocument()->UpdateAllViews(this);
}


void CKaollaView::DebloqueMenu(void)
{
	m_mainDocument = CKaollaDoc::GetDocument();
	pApp->disponibilite_menu = !m_mainDocument->experiment_running;
}

void CKaollaView::MiseAJour()
{
	UpdateData(FALSE);
}

void CKaollaView::LancementThreadMSVAmpoule()
{
	AfxBeginThread(ThreadMenuMiseSousVideAmpoule, GetSafeHwnd());
}

void CKaollaView::LancementThreadMSVBouteille()
{
	AfxBeginThread(ThreadMenuMiseSousVideBouteille, GetSafeHwnd());
}

void CKaollaView::LancementThreadChangementBouteille()
{
	AfxBeginThread(ThreadMenuChangementBouteille, GetSafeHwnd());
}
