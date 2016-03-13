
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
	// Custom messages for threads
	ON_MESSAGE(WM_THREADAFFICHAGE, &CKaollaView::OnThreadAffichage)
	ON_MESSAGE(WM_THREADFINISHEDREG, &CKaollaView::OnRegularThreadFinished)
	ON_MESSAGE(WM_UPDATEBUTTONS, &CKaollaView::OnThreadRequestButtonUpdate)

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
	/*, TemperatureCalo(0)
	, TemperatureCage(0)
	, TemperaturePiece(0)
	, Calorimetre(0)
	, Basse_Pression(0)
	, Haute_Pression(0)*/
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
	DeleteManip();
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
	for (int i = 1; i <= 8; i++)
	{
		GetDlgItem(idc_fermer[i - 1])->EnableWindow(FALSE);
	}
	GetDlgItem(IDC_DESACTIVER_EV1)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_EV2)->EnableWindow(FALSE);
	GetDlgItem(IDC_DESACTIVER_POMPE)->EnableWindow(FALSE);

	// Check to see whether the parameters file has been created
	VerifParametres();

	// Initialize manipulation class - this needs work
	InitialisationManip();
	InitializeObjects();

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

void CKaollaView::DebloqueMenu(void)
{
	m_mainDocument = CKaollaDoc::GetDocument();
	pApp->menuIsAvailable = !m_mainDocument->experiment_running;
}

void CKaollaView::MiseAJour(void)
{
	UpdateData(FALSE);
}

void CKaollaView::OnMsvAmpoule(void)
{
	MiseSousVideAmpoule(GetSafeHwnd());
}

void CKaollaView::OnMsvBouteille()
{
	MiseSousVideBouteille(GetSafeHwnd());
}

void CKaollaView::OnChangementBouteille()
{
	ChangementBouteille(GetSafeHwnd());
}


// Thread callback commands

LRESULT CKaollaView::OnRegularThreadFinished(WPARAM, LPARAM) {

	GetDocument()->experiment_running = FALSE;
	DebloqueMenu();
	
	return 0;
}