
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MainFrm.h"

#include "Kalel.h"
#include "KalelDoc.h"
#include "KalelView.h"

#include "GrapheView.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_UPDATE_COMMAND_UI(IDR_CONNECTED_PANE, &CMainFrame::OnUpdateStatusText)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	IDR_CONNECTED_PANE,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}



int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP 
	//	| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
	//	!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	//{
	//	TRACE0("Failed to create toolbar\n");
	//	return -1;      // fail to create
	//}

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	/*m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);*/

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// Create the splitter and add the views to it
	CRect rect;
	GetClientRect(rect);
	if (!m_wndSplitter.CreateStatic(this, 1, 2)){
		return FALSE;
	}
	if (!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CKalelView), CSize(rect.right / 2, rect.bottom), pContext) ||
		!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CGrapheView), CSize(rect.right / 2, rect.bottom), pContext))
		
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}
	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//cs.style &= ~(LONG)WS_MAXIMIZE;
	cs.cy = ::GetSystemMetrics(SM_CYSCREEN);
	cs.cx = ::GetSystemMetrics(SM_CXSCREEN);

	cs.style &= ~FWS_ADDTOTITLE;
	return CFrameWnd::PreCreateWindow(cs);
}

void CMainFrame::OnUpdateFrameTitle(BOOL /*bAddToTitle*/)
{
	SetWindowText(_T("Kalel"));
}

// CMainFrame diagnostics

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


// CMainFrame message handlers

// Asks the user whether they want to stop the application when an experiment is running
void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		CKalelApp* pApp = static_cast<CKalelApp *>(AfxGetApp());

		if (pApp->experimentRunning)
		{
			if (AfxMessageBox(PROMPT_RUNNINGEXP, MB_YESNO | MB_ICONQUESTION, 0) == IDNO)
			{
				return; // do not close
			}
		}
	}

	CFrameWnd::OnSysCommand(nID, lParam);
}

void CMainFrame::OnUpdateStatusText(CCmdUI *pCmdUI)
{
	pCmdUI->Enable();

	CKalelApp* pApp = static_cast<CKalelApp *>(AfxGetApp());

	if (pApp->serverConnected)
	{
		pCmdUI->SetText(_T("Connected"));
	}
	else
	{
		pCmdUI->SetText(_T("Not Connected!"));
	}
}