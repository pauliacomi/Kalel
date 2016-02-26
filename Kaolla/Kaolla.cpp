
// Kaolla.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "MainFrm.h"
#include "Kaolla.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKaollaApp

BEGIN_MESSAGE_MAP(CKaollaApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CKaollaApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// New menu based commands
	ON_COMMAND(ID_PARAMATRES_APPAREIL, &CKaollaApp::OnParamatresAppareil)
	ON_COMMAND(ID_CONNECTION_PORTS, &CKaollaApp::OnConnectionPorts)
	ON_COMMAND(ID_DONNEES_EXPERIENCE, &CKaollaApp::OnDonneesExperience)
	ON_COMMAND(ID_MSV_AMPOULE, &CKaollaApp::OnMsvAmpoule)
	ON_COMMAND(ID_MSV_BOUTEILLE, &CKaollaApp::OnMsvBouteille)
	ON_COMMAND(ID_CHANGEMENT_BOUTEILLE, &CKaollaApp::OnChangementBouteille)
	// New update based commands
	ON_UPDATE_COMMAND_UI(ID_PARAMATRES_APPAREIL, &CKaollaApp::OnUpdateParamatresAppareil)
	ON_UPDATE_COMMAND_UI(ID_CONNECTION_PORTS, &CKaollaApp::OnUpdateConnectionPorts)
	ON_UPDATE_COMMAND_UI(ID_MSV_AMPOULE, &CKaollaApp::OnUpdateMsvAmpoule)
	ON_UPDATE_COMMAND_UI(ID_MSV_BOUTEILLE, &CKaollaApp::OnUpdateMsvBouteille)
	ON_UPDATE_COMMAND_UI(ID_CHANGEMENT_BOUTEILLE, &CKaollaApp::OnUpdateChangementBouteille)
	ON_UPDATE_COMMAND_UI(ID_DONNEES_EXPERIENCE, &CKaollaApp::OnUpdateDonneesExperience)
END_MESSAGE_MAP()


// CKaollaApp construction

CKaollaApp::CKaollaApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// If the application is built using Common Language Runtime support (/clr):
	//     1) This additional setting is needed for Restart Manager support to work properly.
	//     2) In your project, you must add a reference to System.Windows.Forms in order to build.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: replace application ID string below with unique ID string; recommended
	// format for string is CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("MADIREL.a.NoVersion"));

	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

// The one and only CKaollaApp object

CKaollaApp theApp;


// CKaollaApp initialization

BOOL CKaollaApp::InitInstance()
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

	CWinApp::InitInstance();


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
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)


	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKaollaDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CKaollaView));
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
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	disponibilite_menu = TRUE;
	return TRUE;
}

int CKaollaApp::ExitInstance()
{
	//TODO: handle additional resources you may have added
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

// CKaollaApp message handlers


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

// Données de boîte de dialogue
enum { IDD = IDD_ABOUTBOX };

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
void CKaollaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}




// CKaollaApp message handlers

void CKaollaApp::OnParamatresAppareil()
{
	m_parametres_appareil.DoModal();
}

void CKaollaApp::OnConnectionPorts()
{
	m_connection_ports.DoModal();
	ChangementDev(GetPortVannes(), GetPortTemperatures());
}

// Fonction activée lors l'utilisateur clique sur le menu 'paramètres'
void CKaollaApp::OnUpdateParamatresAppareil(CCmdUI *pCmdUI)
{
	///// pCmdUI->Enable(pKaollaView->fin_experience);
	pCmdUI->Enable(disponibilite_menu);
}


void CKaollaApp::OnUpdateConnectionPorts(CCmdUI *pCmdUI)
{
	/////pCmdUI->Enable(pKaollaView->fin_experience);
	pCmdUI->Enable(disponibilite_menu);
}


void CKaollaApp::OnDonneesExperience()
{
	m_donnees_experience.SetStrDonneesExperience(GetDonneesExperience());
	m_donnees_experience.DoModal();
}

void CKaollaApp::OnUpdateDonneesExperience(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(!disponibilite_menu);
}



void CKaollaApp::OnMsvAmpoule()
{
	///disponibilite_menu = false;
	MiseSousVideAmpoule();
	///disponibilite_menu = true;
}

void CKaollaApp::OnUpdateMsvAmpoule(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(disponibilite_menu);
}

void CKaollaApp::OnMsvBouteille()
{
	MiseSousVideBouteille();
}

void CKaollaApp::OnUpdateMsvBouteille(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(disponibilite_menu);
}

void CKaollaApp::OnChangementBouteille()
{
	ChangementBouteille();
}

void CKaollaApp::OnUpdateChangementBouteille(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(disponibilite_menu);
}

