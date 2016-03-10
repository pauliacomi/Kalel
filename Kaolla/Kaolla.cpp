// Kaolla.cpp : D�finit les comportements de classe pour l'application.
//

#include "stdafx.h"
#include "Kaolla.h"
#include "MainFrm.h"

#include "KaollaDoc.h"
#include "KaollaView.h"

#include "threads.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/*
CKaollaView* GetKaollaView(void);
CKaollaView* pKaollaView;
*/

// CKaollaApp

BEGIN_MESSAGE_MAP(CKaollaApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CKaollaApp::OnAppAbout)
	// Commandes de fichier standard
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	ON_COMMAND(ID_PARAMATRES_APPAREIL, &CKaollaApp::OnParamatresAppareil)
	ON_COMMAND(ID_CONNECTION_PORTS, &CKaollaApp::OnConnectionPorts)
	ON_UPDATE_COMMAND_UI(ID_PARAMATRES_APPAREIL, &CKaollaApp::OnUpdateParamatresAppareil)
	ON_UPDATE_COMMAND_UI(ID_CONNECTION_PORTS, &CKaollaApp::OnUpdateConnectionPorts)
	ON_COMMAND(ID_DONNEES_EXPERIENCE, &CKaollaApp::OnDonneesExperience)
	ON_UPDATE_COMMAND_UI(ID_DONNEES_EXPERIENCE, &CKaollaApp::OnUpdateDonneesExperience)
	ON_COMMAND(ID_MSV_AMPOULE, &CKaollaApp::OnMsvAmpoule)
	ON_UPDATE_COMMAND_UI(ID_MSV_AMPOULE, &CKaollaApp::OnUpdateMsvAmpoule)
	ON_COMMAND(ID_MSV_BOUTEILLE, &CKaollaApp::OnMsvBouteille)
	ON_UPDATE_COMMAND_UI(ID_MSV_BOUTEILLE, &CKaollaApp::OnUpdateMsvBouteille)
	ON_COMMAND(ID_CHANGEMENT_BOUTEILLE, &CKaollaApp::OnChangementBouteille)
	ON_UPDATE_COMMAND_UI(ID_CHANGEMENT_BOUTEILLE, &CKaollaApp::OnUpdateChangementBouteille)
END_MESSAGE_MAP()


// construction CKaollaApp

CKaollaApp::CKaollaApp()
{
	// TODO�: ajoutez ici du code de construction,
	// Placez toutes les initialisations significatives dans InitInstance
}


// Seul et unique objet CKaollaApp

CKaollaApp theApp;


// initialisation de CKaollaApp

BOOL CKaollaApp::InitInstance()
{
	// InitCommonControlsEx() est requis sur Windows�XP si le manifeste de l'application
	// sp�cifie l'utilisation de ComCtl32.dll version�6 ou ult�rieure pour activer les
	// styles visuels.  Dans le cas contraire, la cr�ation de fen�tres �chouera.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// � d�finir pour inclure toutes les classes de contr�les communs � utiliser
	// dans votre application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialiser les biblioth�ques OLE
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Initialisation standard
	// Si vous n'utilisez pas ces fonctionnalit�s et que vous souhaitez r�duire la taille
	// de votre ex�cutable final, vous devez supprimer ci-dessous
	// les routines d'initialisation sp�cifiques dont vous n'avez pas besoin.
	// Changez la cl� de Registre sous laquelle nos param�tres sont enregistr�s
	// TODO : modifiez cette cha�ne avec des informations appropri�es,
	// telles que le nom de votre soci�t� ou organisation
	SetRegistryKey(_T("Applications locales g�n�r�es par AppWizard"));
	LoadStdProfileSettings(4);  // Charge les options de fichier INI standard (y compris les derniers fichiers utilis�s)
	// Inscrire les mod�les de document de l'application. Ces mod�les
	//  lient les documents, fen�tres frame et vues entre eux
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CKaollaDoc),
		RUNTIME_CLASS(CMainFrame),       // fen�tre frame SDI principale
		RUNTIME_CLASS(CKaollaView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// Analyser la ligne de commande pour les commandes shell standard, DDE, ouverture de fichiers
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// Commandes de dispatch sp�cifi�es sur la ligne de commande. Retournent FALSE si
	// l'application a �t� lanc�e avec /RegServer, /Register, /Unregserver ou /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// La seule fen�tre a �t� initialis�e et peut donc �tre affich�e et mise � jour
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// appelle DragAcceptFiles uniquement s'il y a un suffixe
	//  Dans une application SDI, cet appel doit avoir lieu juste apr�s ProcessShellCommand
	
	//pKaollaView = GetKaollaView();
	disponibilite_menu = TRUE;

	return TRUE;
}



// bo�te de dialogue CAboutDlg utilis�e pour la bo�te de dialogue '� propos de' pour votre application

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Donn�es de bo�te de dialogue
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

// Impl�mentation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// Commande App pour ex�cuter la bo�te de dialogue
void CKaollaApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// gestionnaires de messages pour CKaollaApp



void CKaollaApp::OnParamatresAppareil()
{
	m_parametres_appareil.DoModal();
}

void CKaollaApp::OnConnectionPorts()
{
	m_connection_ports.DoModal();
	ChangementDev(GetPortVannes(),GetPortTemperatures());
}

// Fonction activ�e lors l'utilisateur clique sur le menu 'param�tres'
void CKaollaApp::OnUpdateParamatresAppareil(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(pKaollaView->fin_experience);
	pCmdUI->Enable(disponibilite_menu);
}


void CKaollaApp::OnUpdateConnectionPorts(CCmdUI *pCmdUI)
{
	//pCmdUI->Enable(pKaollaView->fin_experience);
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
	//disponibilite_menu = false;
	MiseSousVideAmpoule();
	//disponibilite_menu = true;
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
