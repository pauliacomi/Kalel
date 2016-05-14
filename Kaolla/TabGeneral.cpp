// TabGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "Kaolla.h"
#include "TabGeneral.h"
#include "afxdialogex.h"

#include "Dialogue_experimentateur.h"		// User selection dialog
#include "DialogGas.h"						// Gas selection dialog
#include "XSBrowseFolder.h"					// Browse for folder custom class
#include "Parametres.h"						// For reading the parameters file
#include "ParserXML.h"						// XML builder


// TabGeneral dialog

IMPLEMENT_DYNAMIC(TabGeneral, CMFCPropertyPage)

TabGeneral::TabGeneral()
	: CMFCPropertyPage(TabGeneral::IDD)
{
	Reinitialisation();
}

TabGeneral::~TabGeneral()
{
}

void TabGeneral::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_NOM_FICHIER, m_FileName);
	DDX_Text(pDX, IDC_NOM_CHEMIN, m_Path);
	DDX_Text(pDX, IDC_NOM_ECHANTILLON, m_SampleName);
	DDX_Text(pDX, IDC_MASSE_ECHANTILLON, m_SampleMass);
	DDX_Text(pDX, IDC_TEMPERATURE_EXPERIENCE, m_ExperimentTemperature);
	DDX_Text(pDX, IDC_COMMENTAIRES, m_Comments);

	DDX_Control(pDX, IDC_SPIN_MASSE_ECHANTILLON, m_SpinSampleMass);
	DDX_Control(pDX, IDC_COMBO_GAZ, m_CBGas);
	DDX_CBIndex(pDX, IDC_COMBO_GAZ, m_GasIndex);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_EXPERIENCE, m_SpinTemperatureExperiment);
	DDX_Control(pDX, IDC_COMMENTAIRES, m_EditComments);
	DDX_Control(pDX, IDC_COMBO_EXPERIMENTATEUR, m_CBUser);
	DDX_CBIndex(pDX, IDC_COMBO_EXPERIMENTATEUR, m_UserIndex);
}


BOOL TabGeneral::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	// If there's no configuration, initialise the XML file
	if (!ConfigsExists()){
		Initialisation_XML();
	}

	gasArray = GetGazs();
	userArray = GetExperimentateurs();
	for (unsigned int i = 0; i<gasArray.size(); i++)
	{
		CString symbol;
		symbol.Format(_T("%s"), gasArray[i].symbole.c_str());
		m_CBGas.InsertString(-1, symbol);
	}

	for (unsigned int i = 0; i<userArray.size(); i++)
	{
		CString user;
		user.Format(_T("%s"), userArray[i].surnom.c_str());
		m_CBUser.InsertString(-1, user);
	}

	m_SpinSampleMass.SetRange(0, 1000000);
	m_SpinSampleMass.SetPos(1);
	m_SpinSampleMass.SetInc(-0.0001);
	m_SpinSampleMass.SetFormat("%1.4f");
	m_SpinSampleMass.UpdateBuddy();

	m_SpinTemperatureExperiment.SetRange(0, 300);
	m_SpinTemperatureExperiment.SetPos(30);
	m_SpinTemperatureExperiment.SetInc(-1);
	m_SpinTemperatureExperiment.SetFormat("%1.f");
	m_SpinTemperatureExperiment.UpdateBuddy();

	return TRUE;  // return TRUE unless you set the focus to a control
}

// Oveerride the function to keep the Apply button on all the time
BOOL TabGeneral::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CMFCPropertyPage::OnCommand(wParam, lParam);
}

BOOL TabGeneral::OnApply()
{
	WriteData();
	return CMFCPropertyPage::OnApply();
}

void TabGeneral::OnCancel()
{
	m_Path = _T(allSettings.chemin.c_str());
	m_Comments = _T(allSettings.commentaires.c_str());
	experimentDate = allSettings.date_experience.c_str();
	userExp = allSettings.experimentateur;
	m_FileName = _T(allSettings.fichier.c_str());
	gasExp = allSettings.gaz;
	m_SampleMass = allSettings.masse_echantillon;
	m_SampleName = _T(allSettings.nom_echantillon.c_str());
	m_ExperimentTemperature = allSettings.temperature_experience;

	// remettre le bon index du gaz
	m_GasIndex = -1;
	for (unsigned int i = 0; i<gasArray.size(); i++)
	{
		if (gasArray[i].nom == allSettings.gaz.nom)
		{
			m_GasIndex = i;
			break;
		}
	}

	// remettre le bon index de l'expériementateur
	m_UserIndex = -1;
	for (unsigned int i = 0; i<userArray.size(); i++)
	{
		if (userArray[i].surnom == allSettings.experimentateur.surnom)
		{
			m_UserIndex = i;
			break;
		}
	}

	StrSurnom = _T(userExp.surnom.c_str());
	StrEchantillon = m_SampleName;
	StrGaz = _T(gasExp.symbole.c_str());

	CMFCPropertyPage::OnCancel();
}

void TabGeneral::OnOK()
{
	if (!PathIsDirectory(m_Path))
	{
		AfxMessageBox(ERROR_PATHUNDEF, MB_ICONERROR | MB_OK);
		return;
	}
	WriteData();

	CMFCPropertyPage::OnOK();
}



//------------Custom functions

// Update the date
void TabGeneral::UpdateDate(void)
{
	time_t timestamp;
	struct tm t;

	timestamp = time(NULL);
	localtime_s(&t, &timestamp);

	StrDate.Format(_T("%d_%02d_%02d_%02dh%02d"),
		1900 + t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min);

	char date_char[255];
	sprintf_s(date_char, "%02d/%02d/%d %02dh%02d", t.tm_mon + 1, t.tm_mday,
		1900 + t.tm_year, t.tm_hour, t.tm_min);

	experimentDate = date_char;
}


void TabGeneral::Reinitialisation(void)
{
	UpdateDate();
	m_FileName.Format(TEXT_NEWFILETEXT, StrDate);
	m_Path = _T(GetCheminFichierGeneral().c_str());
	m_SampleName.Format(TEXT_SAMPLE);

	m_SampleMass = 1.0f;
	m_GasIndex = -1;
	m_Comments = _T("");
	m_UserIndex = -1;
	m_ExperimentTemperature = 30;

	StrCalo = _T(GetEnteteCalo().c_str());
	StrSurnom = _T("");
	StrEchantillon = _T("");
	StrGaz = _T("");

	userExp.nom = "";
	userExp.surnom = "";

	gasExp.nom = "";
	gasExp.symbole = "";
	gasExp.masse_moleculaire = 0;
	gasExp.pression_critique = 0;
	gasExp.temperature_critique = 0;
	gasExp.temperature_ebullition = 0;

	/* Reinitialisation of the data with zero values */
	WriteData();
}

void TabGeneral::WriteData()
{
	SetCheminFichierGeneral(m_Path.GetBuffer());

	allSettings.chemin = m_Path.GetBuffer();
	allSettings.commentaires = m_Comments.GetBuffer();
	allSettings.date_experience = experimentDate;
	allSettings.experimentateur = userExp;
	allSettings.fichier = m_FileName.GetBuffer();
	allSettings.gaz = gasExp;
	allSettings.masse_echantillon = m_SampleMass;
	allSettings.nom_echantillon = m_SampleName.GetBuffer();
	allSettings.temperature_experience = m_ExperimentTemperature;
}


BEGIN_MESSAGE_MAP(TabGeneral, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_GAZ, &TabGeneral::OnCbnSelchangeComboGaz)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPERIMENTATEUR, &TabGeneral::OnCbnSelchangeComboExperimentateur)
	ON_EN_CHANGE(IDC_NOM_ECHANTILLON, &TabGeneral::OnEnChangeNomEchantillon)
	ON_BN_CLICKED(IDC_BUTTON_EXPERIMENTATEUR, &TabGeneral::OnBnClickedUserMenu)
	ON_BN_CLICKED(IDC_BUTTON_GAZ, &TabGeneral::OnBnClickedGasMenu)
	ON_BN_CLICKED(IDC_PARCOURIR_DOSSIER, &TabGeneral::OnBnClickedChangeFolder)
END_MESSAGE_MAP()


// TabGeneral message handlers

void TabGeneral::OnCbnSelchangeComboGaz()
{
	UpdateData(TRUE);
	gasExp = gasArray[m_GasIndex];

	StrGaz.Format(_T("%s"), gasExp.symbole.c_str());

	UpdateDate();

	m_FileName = StrCalo + _T("_") + StrSurnom + _T("_") + m_SampleName
		+ _T("_") + StrGaz + _T("_") + StrDate;

	UpdateData(FALSE);
}


void TabGeneral::OnCbnSelchangeComboExperimentateur()
{
	UpdateData(TRUE);
	userExp = userArray[m_UserIndex];

	StrSurnom.Format(_T("%s"), userExp.surnom.c_str());

	UpdateDate();

	m_FileName = StrCalo + _T("_") + StrSurnom + _T("_") + m_SampleName
		+ _T("_") + StrGaz + _T("_") + StrDate;

	UpdateData(FALSE);
}

void TabGeneral::OnEnChangeNomEchantillon()
{
	UpdateData(TRUE);

	UpdateDate();

	m_FileName = StrCalo + _T("_") + StrSurnom + _T("_") + m_SampleName
		+ _T("_") + StrGaz + _T("_") + StrDate;

	UpdateData(FALSE);
}

void TabGeneral::OnBnClickedUserMenu()
{
	CDialogueExperimentateur m_DialogueExperimentateur("");
	m_DialogueExperimentateur.DoModal();

	// Reset the array
	m_CBUser.ResetContent();

	userArray.clear();
	userArray = GetExperimentateurs();
	for (unsigned int i = 0; i<userArray.size(); i++)
	{
		CString name;
		name.Format(_T("%s"), userArray[i].surnom.c_str());
		m_CBUser.InsertString(-1, name);
	}
}


void TabGeneral::OnBnClickedGasMenu()
{
	CDialogueGaz m_DialogGas("");
	m_DialogGas.DoModal();

	// Reset the array
	m_CBGas.ResetContent();

	gasArray.clear();
	gasArray = GetGazs();
	for (unsigned int i = 0; i<gasArray.size(); i++)
	{
		CString symbol;
		symbol.Format(_T("%s"), gasArray[i].symbole.c_str());
		m_CBGas.InsertString(-1, symbol);
	}
}


void TabGeneral::OnBnClickedChangeFolder()
{
	// Create class
	CXSBrowseFolder DialogBoxBrowse;

	// Use the new style
	DialogBoxBrowse.ModifyStyle(BIF_NEWDIALOGSTYLE);

	// Set the dialog's title text
	CString title;
	title.Format(TITLE_BROWSEHOMEFOLDER);
	DialogBoxBrowse.SetTitle(title.GetBuffer(title.GetLength()));

	// Buffer for the returned path
	char path[MAX_PATH];

	// Display the dalog and check the return code
	CString message;
	switch (DialogBoxBrowse.Show(GetSafeHwnd(), path)) {

		// Success
	case CXSBrowseFolder::RET_OK:
		m_Path = _T(path);
		UpdateData(FALSE);
		break;

		// User clicked cancel
	case CXSBrowseFolder::RET_CANCEL:
		AfxMessageBox(TEXT_OPCANCELLED, MB_ICONINFORMATION | MB_TOPMOST);
		break;

		// The shell did not return a path for the selection
	case CXSBrowseFolder::RET_NOPATH:
		AfxMessageBox(TEXT_FOLDERNOTSELECTED, MB_ICONSTOP | MB_TOPMOST);
		break;
	}
}