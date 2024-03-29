// TabGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "TabGeneral.h"
#include "afxdialogex.h"

#include "DialogUser.h"													// User selection dialog
#include "DialogGas.h"													// Gas selection dialog

#include "XSBrowseFolder.h"												// Browse for folder custom class
#include "ParserXML.h"													// XML builder

#include "../Kalel Shared/Com Classes/MachineSettings.h"				// Accessing the settings
#include "StringTable.h"												// Error message strings

// TabGeneral dialog

IMPLEMENT_DYNAMIC(TabGeneral, CMFCPropertyPage)

TabGeneral::TabGeneral()
	: CMFCPropertyPage(TabGeneral::IDD)
	, settings{ nullptr }
{
}

void TabGeneral::PassSettings(const MachineSettings * machineSettings)
{
	settings = machineSettings;
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
	DDX_Text(pDX, IDC_MASSE_ECHANTILLON,m_SampleMass);
	DDX_Control(pDX, IDC_SPIN_MASSE_ECHANTILLON, m_SpinSampleMass);
	DDX_Text(pDX, IDC_TEMPERATURE_EXPERIENCE, m_ExperimentTemperature);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_EXPERIENCE, m_SpinTemperatureExperiment);
	DDX_Text(pDX, IDC_TEMPERATURE_EXPERIENCE2, m_ExperimentTemperatureMargin);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_EXPERIENCE2, m_SpinTemperatureMarginExperiment);
	DDX_Text(pDX, IDC_COMMENTAIRES, m_Comments);

	DDX_Control(pDX, IDC_COMBO_GAZ, m_CBGas);
	DDX_CBIndex(pDX, IDC_COMBO_GAZ, m_GasIndex);
	DDX_Control(pDX, IDC_COMMENTAIRES, m_EditComments);
	DDX_Control(pDX, IDC_COMBO_EXPERIMENTATEUR, m_CBUser);
	DDX_CBIndex(pDX, IDC_COMBO_EXPERIMENTATEUR, m_UserIndex);
}


BOOL TabGeneral::OnInitDialog()
{
	// If there's no configuration, initialise the XML file storing the gasses
	if (!ConfigsExists()) {
		Initialisation_XML();
	}

	// Get settings from storage
	m_Path = allSettings.chemin.c_str();
	m_Comments = allSettings.commentaires.c_str();
	experimentDate = allSettings.date_experience.c_str();
	m_FileName = allSettings.fichier.c_str();
	m_SampleMass = allSettings.masse_echantillon;
	m_SampleName = allSettings.nom_echantillon.c_str();
	m_ExperimentTemperature = allSettings.temperature_experience;
	m_ExperimentTemperatureMargin = allSettings.temperature_range_initial_check;
	gasExp = allSettings.gas;
	userExp = allSettings.user;

	StrCalo = settings->CaloPrefix.c_str();
	StrSurnom = userExp.surnom.c_str();
	StrEchantillon = m_SampleName;
	StrGaz = gasExp.symbole.c_str();

	// Get array and find if there'a an already existing index
	gasArray = GetGazs();
	m_GasIndex = -1;
	for (size_t i = 0; i < gasArray.size(); i++)
	{
		if (gasArray[i].nom == allSettings.gas.nom)
		{
			m_GasIndex = i;
			break;
		}
	}

	// Get array and find if there'a an already existing index
	userArray = GetExperimentateurs();
	m_UserIndex = -1;
	for (size_t i = 0;  i< userArray.size(); i++)
	{
		if (userArray[i].surnom == allSettings.user.surnom)
		{
			m_UserIndex = i;
			break;
		}
	}

	// Initialize dialog
	CMFCPropertyPage::OnInitDialog();

	// Populate list with values
	for (size_t i = 0; i < gasArray.size(); i++)
	{
		CString symbol;
		symbol.Format(_T("%s"), gasArray[i].symbole.c_str());
		m_CBGas.InsertString(-1, symbol);
	}

	for (size_t i = 0; i < userArray.size(); i++)
	{
		CString user;
		user.Format(_T("%s"), userArray[i].surnom.c_str());
		m_CBUser.InsertString(-1, user);
	}

	// Set indexes which was taken earlier
	if (m_GasIndex != -1)
	{
		m_CBGas.SetTopIndex(m_GasIndex);
		UpdateData(false);
	}

	if (m_UserIndex != -1)
	{
		m_CBUser.SetTopIndex(m_UserIndex);
		UpdateData(false);
	}

	m_SpinSampleMass.SetRange(0, 1000000);
	m_SpinSampleMass.SetInc(-0.0001);
	m_SpinSampleMass.SetFormat(_T("%1.4f"));
	m_SpinSampleMass.UpdateBuddy();

	m_SpinTemperatureExperiment.SetRange(-300, 300);
	m_SpinTemperatureExperiment.SetInc(-1);
	m_SpinTemperatureExperiment.SetFormat(_T("%1.f"));
	m_SpinTemperatureExperiment.UpdateBuddy();

	m_SpinTemperatureMarginExperiment.SetRange(-50, 50);
	m_SpinTemperatureMarginExperiment.SetInc(-1);
	m_SpinTemperatureMarginExperiment.SetFormat(_T("%1.f"));
	m_SpinTemperatureMarginExperiment.UpdateBuddy();

	ToggleGreyOut();

	// Fill in intial filename:
	UpdateDate();
	m_FileName = StrCalo + _T("_") + StrSurnom + _T("_") + m_SampleName + _T("_") + StrGaz + _T("_") + StrDate;

	// Finish
	UpdateData(FALSE);

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
	CMFCPropertyPage::OnCancel();
}

void TabGeneral::OnOK()
{
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
	// Get settings from storage
	m_Path = allSettings.chemin.c_str();
	m_Comments = allSettings.commentaires.c_str();
	experimentDate = allSettings.date_experience.c_str();
	m_FileName = allSettings.fichier.c_str();
	m_SampleMass = allSettings.masse_echantillon;
	m_SampleName = allSettings.nom_echantillon.c_str();
	m_ExperimentTemperature = allSettings.temperature_experience;
	m_ExperimentTemperatureMargin = allSettings.temperature_range_initial_check;

	gasExp = allSettings.gas;
	// remettre le bon index du gaz
	m_GasIndex = -1;
	for (unsigned int i = 0; i<gasArray.size(); i++)
	{
		if (gasArray[i].nom == allSettings.gas.nom)
		{
			m_GasIndex = i;
			break;
		}
	}

	userExp = allSettings.user;
	// remettre le bon index de l'expériementateur
	m_UserIndex = -1;
	for (unsigned int i = 0; i<userArray.size(); i++)
	{
		if (userArray[i].surnom == allSettings.user.surnom)
		{
			m_UserIndex = i;
			break;
		}
	}

	StrCalo = settings->CaloPrefix.c_str();
	StrSurnom = userExp.surnom.c_str();
	StrEchantillon = m_SampleName;
	StrGaz = gasExp.symbole.c_str();
}

void TabGeneral::WriteData()
{
	MachineSettings newSettings;
	newSettings.DefaultPath = m_Path.GetString();

	allSettings.chemin = m_Path.GetBuffer();
	allSettings.commentaires = m_Comments.GetBuffer();
	allSettings.date_experience = experimentDate;
	allSettings.user = userExp;
	allSettings.fichier = m_FileName.GetBuffer();
	allSettings.gas = gasExp;
	allSettings.masse_echantillon = m_SampleMass;
	allSettings.nom_echantillon = m_SampleName.GetBuffer();
	allSettings.temperature_experience = m_ExperimentTemperature;
	allSettings.temperature_range_initial_check = m_ExperimentTemperatureMargin;
}


void TabGeneral::GreyOut(BOOL active)
{
	GetDlgItem(IDC_COMBO_GAZ)->EnableWindow(active);
	GetDlgItem(IDC_COMBO_EXPERIMENTATEUR)->EnableWindow(active);

	GetDlgItem(IDC_BUTTON_EXPERIMENTATEUR)->EnableWindow(active);
	GetDlgItem(IDC_BUTTON_GAZ)->EnableWindow(active);

	GetDlgItem(IDC_NOM_FICHIER)->EnableWindow(active);
	GetDlgItem(IDC_NOM_CHEMIN)->EnableWindow(active);
	GetDlgItem(IDC_NOM_ECHANTILLON)->EnableWindow(active);
	GetDlgItem(IDC_MASSE_ECHANTILLON)->EnableWindow(active);
	GetDlgItem(IDC_TEMPERATURE_EXPERIENCE)->EnableWindow(active);
	GetDlgItem(IDC_COMMENTAIRES)->EnableWindow(active);

	GetDlgItem(IDC_SPIN_MASSE_ECHANTILLON)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_TEMPERATURE_EXPERIENCE)->EnableWindow(active);
}


void TabGeneral::ToggleGreyOut()
{
	if (checkGeneral == true)
		GreyOut(FALSE); 
	else
		GreyOut(TRUE);
}



// Message map


BEGIN_MESSAGE_MAP(TabGeneral, CMFCPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_GAZ, &TabGeneral::OnCbnSelchangeComboGaz)
	ON_CBN_SELCHANGE(IDC_COMBO_EXPERIMENTATEUR, &TabGeneral::OnCbnSelchangeComboExperimentateur)
	ON_EN_CHANGE(IDC_NOM_ECHANTILLON, &TabGeneral::OnEnChangeNomEchantillon)
	ON_BN_CLICKED(IDC_BUTTON_EXPERIMENTATEUR, &TabGeneral::OnBnClickedUserMenu)
	ON_BN_CLICKED(IDC_BUTTON_GAZ, &TabGeneral::OnBnClickedGasMenu)
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
	CDialogueExperimentateur m_DialogueExperimentateur(_T(""));
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
	CDialogueGaz m_DialogGas(_T(""));
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

// TODO : not used
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
	wchar_t path[MAX_PATH];

	// Display the dalog and check the return code
	CString message;
	switch (DialogBoxBrowse.Show(GetSafeHwnd(), path)) {

		// Success
	case CXSBrowseFolder::RET_OK:
		m_Path = path;
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