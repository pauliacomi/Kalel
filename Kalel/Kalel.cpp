
// Kalel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"

#include "MainFrm.h"
#include "Kalel.h"

#include "KalelDoc.h"
#include "KalelView.h"

#include "DefineMenuMessages.h"
#include "DefinePostMessages.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKalelApp

BEGIN_MESSAGE_MAP(CKalelApp, CWinAppEx)

	// Standard document commands
	ON_COMMAND(ID_APP_ABOUT, &CKalelApp::OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)

	// New menu based commands to main view
	ON_COMMAND(ID_PARAMATRES_SERVERCONNECT, &CKalelApp::DisplayServerConnect)
	ON_COMMAND(ID_PARAMATRES_APPAREIL, &CKalelApp::DisplayPortDialog)
	ON_COMMAND(ID_CONNECTION_PORTS, &CKalelApp::DisplayApparatusSettingsDialog)
	ON_COMMAND(ID_MSV_AMPOULE, &CKalelApp::OnMsvAmpoule)
	ON_COMMAND(ID_MSV_BOUTEILLE, &CKalelApp::OnMsvBouteille)
	ON_COMMAND(ID_CHANGEMENT_BOUTEILLE, &CKalelApp::OnChangementBouteille)
	ON_COMMAND(ID_BACKGROUNDTHREAD_START, &CKalelApp::OnBackgroundthreadStart)
	ON_COMMAND(ID_BACKGROUNDTHREAD_STOP, &CKalelApp::OnBackgroundthreadStop)
	ON_COMMAND(ID_BACKGROUNDTHREAD_RESTART, &CKalelApp::OnBackgroundthreadRestart)

	// New update based commands
	ON_UPDATE_COMMAND_UI(ID_PARAMATRES_SERVERCONNECT, &CKalelApp::OnUpdateServerConnect)
	ON_UPDATE_COMMAND_UI(ID_PARAMATRES_APPAREIL, &CKalelApp::OnUpdateParamatresAppareil)
	ON_UPDATE_COMMAND_UI(ID_CONNECTION_PORTS, &CKalelApp::OnUpdateConnectionPorts)
	ON_UPDATE_COMMAND_UI(ID_MSV_AMPOULE, &CKalelApp::OnUpdateMsvAmpoule)
	ON_UPDATE_COMMAND_UI(ID_MSV_BOUTEILLE, &CKalelApp::OnUpdateMsvBouteille)
	ON_UPDATE_COMMAND_UI(ID_CHANGEMENT_BOUTEILLE, &CKalelApp::OnUpdateChangementBouteille)
	ON_UPDATE_COMMAND_UI(ID_DONNEES_EXPERIENCE, &CKalelApp::OnUpdateDonneesExperience)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUNDTHREAD_START, &CKalelApp::OnUpdateBackgroundthreadStart)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUNDTHREAD_STOP, &CKalelApp::OnUpdateBackgroundthreadStop)
	ON_UPDATE_COMMAND_UI(ID_BACKGROUNDTHREAD_RESTART, &CKalelApp::OnUpdateBackgroundthreadRestart)
END_MESSAGE_MAP()


// CKalelApp construction

CKalelApp::CKalelApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// Application ID string
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MADIREL.Kalel.1"));

	// TODO: add construction code here,

	menuIsAvailable = true;
	experimentRunning = false;
	serverConnected = false;

	// Place all significant initialization in InitInstance
}

// The one and only CKalelApp object

CKalelApp theApp;


// CKalelApp initialization

BOOL CKalelApp::InitInstance()
{

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// AfxInitRichEdit2() is required to use RichEdit control	
	// AfxInitRichEdit2();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("MADIREL"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKalelDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CKalelView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();
	
	return TRUE;
}

int CKalelApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}


///////////////////////////////
// CKalelApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()

// App command to run the dialog
void CKalelApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CKalelApp::DisplayServerConnect() {
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_DISP_CONNECTS_DIALOG, 0, 0);
}

void CKalelApp::DisplayPortDialog(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_DISP_PORT_DIALOG, 0, 0);
}

void CKalelApp::DisplayApparatusSettingsDialog(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_DISP_DEVSETTINGS_DIALOG, 0, 0);
}


void CKalelApp::OnMsvAmpoule(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_FUNC_VACUUM_SAMPLE, 0, 0);
}

void CKalelApp::OnMsvBouteille(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_FUNC_VACUUM_BOTTLE, 0, 0);
}


void CKalelApp::OnChangementBouteille(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_FUNC_CHANGE_BOTTLE, 0, 0);
}

void CKalelApp::OnBackgroundthreadStart(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_THREAD_START, 0, 0);
}


void CKalelApp::OnBackgroundthreadStop(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_THREAD_STOP, 0, 0);
}


void CKalelApp::OnBackgroundthreadRestart(){
	PostMessage(CKalelView::GetView()->GetSafeHwnd(), UWM_THREAD_RESTART, 0, 0);
}


// Make sure that the functionalities are only available when the experiment is not running

void CKalelApp::OnUpdateServerConnect(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateParamatresAppareil(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateConnectionPorts(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateDonneesExperience(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!menuIsAvailable);
}

void CKalelApp::OnUpdateMsvAmpoule(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateMsvBouteille(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateChangementBouteille(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateBackgroundthreadStart(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!menuIsAvailable);
}

void CKalelApp::OnUpdateBackgroundthreadStop(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}

void CKalelApp::OnUpdateBackgroundthreadRestart(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(menuIsAvailable);
}