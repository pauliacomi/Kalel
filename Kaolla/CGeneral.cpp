#include "stdafx.h"
#include "Kaolla.h"
#include "CGeneral.h"


#include <sstream>

#include "time.h"

using namespace std;


///////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CGeneral

IMPLEMENT_DYNAMIC(CGeneral, CPropertyPage)

CGeneral::CGeneral()
	: CPropertyPage(CGeneral::IDD)
	, m_DialogueExperimentateur("")
	, m_DialogueGaz("")
	, StrDate(_T(""))
{
	Reinitialisation();
}

CGeneral::~CGeneral()
{
}

void CGeneral::DoDataExchange(CDataExchange* pDX)
{
	//TRACE("Entering CGeneral::DoDataExchange -- %d\n",
		//pDX->m_bSaveAndValidate);

	CPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_NOM_FICHIER, m_strNomFichier);
	DDX_Text(pDX, IDC_NOM_CHEMIN, m_strChemin);
	DDX_Text(pDX, IDC_NOM_ECHANTILLON, m_strNomEchantillon);
	DDX_Text(pDX, IDC_MASSE_ECHANTILLON, m_fMasseEchantillon);
	DDX_Text(pDX, IDC_TEMPERATURE_EXPERIENCE, m_nTemperatureExperience);
	DDX_Text(pDX, IDC_COMMENTAIRES, m_StrCommentaires);

	DDX_Control(pDX, IDC_SPIN_MASSE_ECHANTILLON, pSpinMasseEchantillon);
	DDX_Control(pDX, IDC_COMBO_GAZ, m_CBGaz);
	DDX_CBIndex(pDX, IDC_COMBO_GAZ, m_nIndexGaz);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_EXPERIENCE, pSpinTemperatureExperience);
	DDX_Control(pDX, IDC_COMMENTAIRES, m_EditCommentaires);
	DDX_Control(pDX, IDC_COMBO_EXPERIMENTATEUR, m_CBExperimentateur);
	DDX_CBIndex(pDX, IDC_COMBO_EXPERIMENTATEUR, m_nIndexExperimentateur);
}

BOOL CGeneral::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam,lParam);
}

BEGIN_MESSAGE_MAP(CGeneral, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_GAZ, &CGeneral::OnCbnSelchangeComboGaz)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPERIMENTATEUR, &CGeneral::OnCbnSelchangeComboExperimentateur)
	ON_EN_CHANGE(IDC_NOM_ECHANTILLON, &CGeneral::OnEnChangeNomEchantillon)
	ON_BN_CLICKED(IDC_BUTTON_EXPERIMENTATEUR, &CGeneral::OnBnClickedButtonExperimentateur)
	ON_BN_CLICKED(IDC_BUTTON_GAZ, &CGeneral::OnBnClickedButtonGaz)
	ON_BN_CLICKED(IDC_PARCOURIR_DOSSIER, &CGeneral::OnBnClickedParcourirDossier)
END_MESSAGE_MAP()


// Gestionnaires de messages de CGeneral

BOOL CGeneral::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	ActualiserDate();
	m_strNomFichier=_T("Nouveau_fichier_") + StrDate;

	UpdateData(FALSE);

	StrCalo = _T(GetEnteteCalo().c_str());

	if(!ConfigsExists())
		Initialisation_XML();

	list_gaz=GetGazs();
	list_experimentateur=GetExperimentateurs();
	for(unsigned int i=0;i<list_gaz.size();i++)
	{
		CString StrSymbole;
		
		StrSymbole.Format(_T("%s"),list_gaz[i].symbole.c_str());
		m_CBGaz.InsertString(-1,StrSymbole);
	}

	for(unsigned int i=0;i<list_experimentateur.size();i++)
	{
		CString StrSurnom;
		StrSurnom.Format(_T("%s"),list_experimentateur[i].surnom.c_str());
		m_CBExperimentateur.InsertString(-1,StrSurnom);
	}


	pSpinMasseEchantillon.SetRange(0,1000000);
	pSpinMasseEchantillon.SetPos(1);
	pSpinMasseEchantillon.SetInc(-0.0001);
	pSpinMasseEchantillon.SetFormat("%1.4f");
	pSpinMasseEchantillon.UpdateBuddy();

	pSpinTemperatureExperience.SetRange(0,300);
	pSpinTemperatureExperience.SetPos(30);
	pSpinTemperatureExperience.SetInc(-1);
	pSpinTemperatureExperience.SetFormat("%1.f");
	pSpinTemperatureExperience.UpdateBuddy();


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void CGeneral::ActualiserDate(void)
{
	time_t timestamp;
	struct tm t;

	timestamp = time(NULL);
    localtime_s(&t, &timestamp);

	StrDate.Format(_T("%d_%02d_%02d_%02dh%02d"),
		1900 + t.tm_year,t.tm_mon+1,t.tm_mday,t.tm_hour,t.tm_min);

	char date_char[255];
	sprintf_s(date_char,"%02d/%02d/%d %02dh%02d",t.tm_mon+1, t.tm_mday, 
			1900 + t.tm_year, t.tm_hour, t.tm_min);

	Date_Experience = date_char;
}



void CGeneral::Reinitialisation(void)
{
	ActualiserDate();
	m_strNomFichier = _T("Nouveau_fichier_") + StrDate;

	m_strChemin=_T(GetCheminFichierGeneral().c_str());
	m_strNomEchantillon=_T("Echantillon");
	
	m_fMasseEchantillon = 1;
	m_nIndexGaz = -1;//0;
	m_StrCommentaires = _T("");
	m_nIndexExperimentateur = -1;//0;
	m_nTemperatureExperience = 30;

	StrCalo = _T(GetEnteteCalo().c_str());
	StrSurnom = _T("");
	StrEchantillon = _T("");
	StrGaz = _T("");

	experimentateur_exp.nom = "";
	experimentateur_exp.surnom = "";

	
	gaz_exp.nom = "";
	gaz_exp.symbole = "";
	gaz_exp.masse_moleculaire = 0;
	gaz_exp.pression_critique = 0;
	gaz_exp.temperature_critique = 0;
	gaz_exp.temperature_ebullition = 0;

	/* Réinitialisation des donnees_actuelles */

	Enregistrement_donnees_actuelles();

}




void CGeneral::RajoutSymboleCombo(CString StrSymbole)
{
	m_CBGaz.InsertString(-1,StrSymbole);
}



void CGeneral::OnCbnSelchangeComboGaz()
{
	UpdateData(TRUE);
	gaz_exp=list_gaz[m_nIndexGaz];
	
	StrGaz.Format(_T("%s"), gaz_exp.symbole.c_str());
	ActualiserDate();
	m_strNomFichier = StrCalo + _T("_") + StrSurnom + _T("_") + m_strNomEchantillon
					+ _T("_") + StrGaz  + _T("_") + StrDate;

	UpdateData(FALSE);
}


void CGeneral::OnCbnSelchangeComboExperimentateur()
{
	UpdateData(TRUE);
	experimentateur_exp=list_experimentateur[m_nIndexExperimentateur];

	StrSurnom.Format(_T("%s"), experimentateur_exp.surnom.c_str());

	ActualiserDate();

	m_strNomFichier = StrCalo + _T("_") + StrSurnom + _T("_") + m_strNomEchantillon
					+ _T("_") + StrGaz  + _T("_") + StrDate;


	UpdateData(FALSE);
}

void CGeneral::OnEnChangeNomEchantillon()
{
	UpdateData(TRUE);

	ActualiserDate();

	m_strNomFichier = StrCalo + _T("_") + StrSurnom + _T("_") + m_strNomEchantillon
					+ _T("_") + StrGaz  + _T("_") + StrDate;

	UpdateData(FALSE);
}

BOOL CGeneral::OnApply()
{
	Enregistrement_donnees_actuelles();

	return CPropertyPage::OnApply();

}

void CGeneral::OnCancel()
{
	m_strChemin = _T(donnees_actuelles.chemin.c_str());
	m_StrCommentaires = _T(donnees_actuelles.commentaires.c_str());
	Date_Experience = donnees_actuelles.date_experience;
	experimentateur_exp = donnees_actuelles.experimentateur;
	m_strNomFichier = _T(donnees_actuelles.fichier.c_str());
	gaz_exp = donnees_actuelles.gaz;
	m_fMasseEchantillon = donnees_actuelles.masse_echantillon;
	m_strNomEchantillon = _T(donnees_actuelles.nom_echantillon.c_str());
	m_nTemperatureExperience = donnees_actuelles.temperature_experience;



	// remettre le bon index du gaz
	m_nIndexGaz=-1;
	for(unsigned int i=0;i<list_gaz.size();i++)
	{
		if (list_gaz[i].nom == donnees_actuelles.gaz.nom)
		{
			m_nIndexGaz = i;
			break;
		}
	}

	// remettre le bon index de l'expériementateur
	m_nIndexExperimentateur=-1;
	for(unsigned int i=0;i<list_experimentateur.size();i++)
	{
		if (list_experimentateur[i].surnom == donnees_actuelles.experimentateur.surnom)
		{
			m_nIndexExperimentateur = i;
			break;
		}
	}

	StrSurnom = _T(experimentateur_exp.surnom.c_str());
	StrEchantillon = m_strNomEchantillon;
	StrGaz = _T(gaz_exp.symbole.c_str());

	CPropertyPage::OnCancel();
}

void CGeneral::OnOK()
{
	if(!PathIsDirectory(m_strChemin))
	{
		AfxMessageBox(_T("Le chemin demandé est inexistant. Veuilllez rectifier.\t\n"),MB_ICONERROR | MB_OK);
		return;
	}
	Enregistrement_donnees_actuelles();

	CPropertyPage::OnOK();
}


void CGeneral::Enregistrement_donnees_actuelles()
{
	SetCheminFichierGeneral(m_strChemin.GetBuffer());

	donnees_actuelles.chemin = m_strChemin.GetBuffer();
	donnees_actuelles.commentaires = m_StrCommentaires.GetBuffer();
	donnees_actuelles.date_experience = Date_Experience;
	donnees_actuelles.experimentateur = experimentateur_exp;
	donnees_actuelles.fichier = m_strNomFichier.GetBuffer();
	donnees_actuelles.gaz = gaz_exp;
	donnees_actuelles.masse_echantillon = m_fMasseEchantillon;
	donnees_actuelles.nom_echantillon = m_strNomEchantillon.GetBuffer();
	donnees_actuelles.temperature_experience = m_nTemperatureExperience;
}



void CGeneral::OnBnClickedButtonExperimentateur()
{
	m_DialogueExperimentateur.DoModal();

	//effacer m_CBExperimentateur
	m_CBExperimentateur.ResetContent();

	list_experimentateur.clear();
	list_experimentateur=GetExperimentateurs();
	for(unsigned int i=0;i<list_experimentateur.size();i++)
	{
		CString StrSurnom;
		StrSurnom.Format(_T("%s"),list_experimentateur[i].surnom.c_str());
		m_CBExperimentateur.InsertString(-1,StrSurnom);
	}
}


void CGeneral::OnBnClickedButtonGaz()
{
	m_DialogueGaz.DoModal();

	//effacer m_CBGaz
	m_CBGaz.ResetContent();

	list_gaz.clear();
	list_gaz=GetGazs();
	for(unsigned int i=0;i<list_gaz.size();i++)
	{
		CString StrSymbole;
		StrSymbole.Format(_T("%s"),list_gaz[i].symbole.c_str());
		m_CBGaz.InsertString(-1,StrSymbole);
	}
}

void CGeneral::OnBnClickedParcourirDossier()
{
	// Create class
	CXSBrowseFolder BoiteDialogueParcourir;

	// Use the new style
	BoiteDialogueParcourir.ModifyStyle(BIF_NEWDIALOGSTYLE);

	// Set the dialog's title text
	BoiteDialogueParcourir.SetTitle("Sélectionnez le dossier où seront répertoriés tous les résultats des manipulations.");

	// Buffer for the returned path
	char path[MAX_PATH];

	// Display the dalog and check the return code
	switch (BoiteDialogueParcourir.Show(GetSafeHwnd(), path)) {

		// Success
		case CXSBrowseFolder::RET_OK:
		MessageBox(path, "Vous avez sélectionné", MB_ICONINFORMATION | MB_TOPMOST);
		m_strChemin = _T(path);
		UpdateData(FALSE);
		break;

		// User clicked cancel
		case CXSBrowseFolder::RET_CANCEL:
		MessageBox("Operation annulé.", "Info", MB_ICONINFORMATION | MB_TOPMOST);
		break;

		// The shell did not return a path for the selection
		case CXSBrowseFolder::RET_NOPATH:
		MessageBox("Vous n'avez pas sélectionné un dossier", "Uh Oh", MB_ICONSTOP | MB_TOPMOST);
		break;

	}

}
