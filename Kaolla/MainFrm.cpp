// MainFrm.cpp : implémentation de la classe CMainFrame
//

#include "stdafx.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "GrapheView.h"

#include "MainFrm.h"

#include "threads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // indicateur de la ligne d'état
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// construction ou destruction de CMainFrame

CMainFrame::CMainFrame()
{
	// TODO : ajoutez ici le code d'une initialisation de membre
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Impossible de créer toolbar\n");
		return -1;      // échec de la création
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Impossible de créer la barre d'état\n");
		return -1;      // échec de la création
	}

	// TODO : supprimez ces trois lignes si vous ne souhaitez pas que la barre d'outils soit ancrable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{

	CRect rect;
	GetClientRect(rect);
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;
	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CGrapheView), CSize(rect.right/2, rect.bottom), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CKaollaView), CSize(rect.right/2, rect.bottom), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;

	
	//return m_wndSplitter.Create(this,
	//	2, 2,               // TODO : ajustez le nombre de lignes, de colonnes
	//	CSize(10, 10),      // TODO : ajustez la taille minimale du volet
	//	pContext);
	
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO : changez ici la classe ou les styles Window en modifiant
	//  CREATESTRUCT cs
	
	//cs.style &= ~(LONG)WS_MAXIMIZE;
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);


	return TRUE;
}


// diagnostics pour CMainFrame

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// gestionnaires de messages pour CMainFrame




void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if(nID==SC_CLOSE)
	{
		CKaollaView *pKV = GetKaollaView();
		if(!pKV->fin_experience)
		{
			if(AfxMessageBox("Une expérience est en cours, voulez-vous tout arrêter ?",
							 MB_YESNO|MB_ICONQUESTION)==IDYES)
			{
				pKV->OnBnClickedArreter();
			}
			return; // pas de fermeture
		}
	}


	CFrameWnd::OnSysCommand(nID, lParam);
}
