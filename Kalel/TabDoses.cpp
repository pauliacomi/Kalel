// TabDoses.cpp : implementation file
//

#include "stdafx.h"
#include "Kalel.h"
#include "TabDoses.h"
#include "afxdialogex.h"

#include "General.h"			// For some definitions
#include "DefinePostMessages.h"		// For custom message definitions

// TabDoses dialog

IMPLEMENT_DYNAMIC(TabDoses, CMFCPropertyPage)

TabDoses::TabDoses(int number)
	: CMFCPropertyPage(TabDoses::IDD)
{
	// Set title from its initialisation
	Rename(number);

	Reinitialisation();
}

TabDoses::~TabDoses()
{
}

void TabDoses::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_DOSES, m_CheckDoses);
	DDX_Check(pDX, IDC_CHECK_DOSES, m_bDoses);
	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_DOSES, m_fDeltaPressureDoses);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_DOSES, m_SpinDeltaPressureDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_DOSES, m_nTimeVolumeDoses);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_DOSES, m_SpinTimeVolumeDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_ADSORPTION_DOSES, m_nTimeAdsorptionDoses);
	DDX_Control(pDX, IDC_SPIN_TEMPS_ADSORPTION_DOSES, m_SpinTimeAdsorptionDoses);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_DOSES, m_fFinalPressureDoses);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_DOSES, m_SpinFinalPressureDoses);
}

BOOL TabDoses::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CMFCPropertyPage::OnCommand(wParam, lParam);
}

BOOL TabDoses::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_SpinDeltaPressureDoses.SetRange(0, 1000000);
	m_SpinDeltaPressureDoses.SetPos(0.100);
	m_SpinDeltaPressureDoses.SetInc(-0.001);
	m_SpinDeltaPressureDoses.SetFormat("%1.3f");
	m_SpinDeltaPressureDoses.UpdateBuddy();

	m_SpinTimeVolumeDoses.SetRange(0, 100000000);
	m_SpinTimeVolumeDoses.SetPos(5);
	m_SpinTimeVolumeDoses.SetInc(-1);
	m_SpinTimeVolumeDoses.SetFormat("%1.f");
	m_SpinTimeVolumeDoses.UpdateBuddy();

	m_SpinTimeAdsorptionDoses.SetRange(0, 10000000);
	m_SpinTimeAdsorptionDoses.SetPos(90);
	m_SpinTimeAdsorptionDoses.SetInc(-1);
	m_SpinTimeAdsorptionDoses.SetFormat("%1.f");
	m_SpinTimeAdsorptionDoses.UpdateBuddy();

	m_SpinFinalPressureDoses.SetRange(0, 100000000);
	m_SpinFinalPressureDoses.SetPos(1);
	m_SpinFinalPressureDoses.SetInc(-0.001);
	m_SpinFinalPressureDoses.SetFormat("%1.3f");
	m_SpinFinalPressureDoses.UpdateBuddy();

	EnableDoses(m_bDoses);
	ActionCheck_Doses();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL TabDoses::OnApply()
{
	WriteData();
	return CMFCPropertyPage::OnApply();
}

void TabDoses::OnCancel()
{
	m_bDoses = allSettings.a_effectuer;
	m_fDeltaPressureDoses = allSettings.delta_pression;
	m_fFinalPressureDoses = allSettings.pression_finale;
	m_nTimeAdsorptionDoses = allSettings.temps_adsorption;
	m_nTimeVolumeDoses = allSettings.temps_volume;

	CMFCPropertyPage::OnCancel();
}

void TabDoses::OnOK()
{
	WriteData();
	CMFCPropertyPage::OnOK();
}

void TabDoses::Reinitialisation()
{
	m_bDoses = FALSE;
	m_fDeltaPressureDoses = 0.1f;
	m_nTimeVolumeDoses = 5;
	m_nTimeAdsorptionDoses = 90;
	m_fFinalPressureDoses = 1.0f;

	checkDoses = UN_GREY_OUT;

	WriteData();
}

void TabDoses::WriteData()
{
	allSettings.a_effectuer = m_bDoses;
	allSettings.delta_pression = m_fDeltaPressureDoses;
	allSettings.pression_finale = m_fFinalPressureDoses;
	allSettings.temps_adsorption = m_nTimeAdsorptionDoses;
	allSettings.temps_volume = m_nTimeVolumeDoses;
}

void TabDoses::EnableDoses(BOOL active)
{
	GetDlgItem(IDC_STATIC_DPPMY_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_DPPMY_BAR)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_TEVR_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_TEVR_MIN)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_TEA_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_TEA_MIN)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_PF_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_PF_BAR)->EnableWindow(active);

	GetDlgItem(IDC_EDIT_DELTA_PRESSION_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_DELTA_PRESSION_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_TEMPS_VOLUME_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_TEMPS_VOLUME_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_TEMPS_ADSORPTION_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_TEMPS_ADSORPTION_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_PRESSION_FINALE_DOSES)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_PRESSION_FINALE_DOSES)->EnableWindow(active);
}

void TabDoses::GreyOut()
{
	GetDlgItem(IDC_CHECK_DOSES)->EnableWindow(FALSE);
}

void TabDoses::UnGreyOut()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CHECK_DOSES)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

void TabDoses::CheckGreyOut()
{
	checkDoses = GREY_OUT;
}

void TabDoses::CheckUnGreyOut()
{
	checkDoses = UN_GREY_OUT;
}

void TabDoses::ActionCheck_Doses()
{
	if (checkDoses == GREY_OUT)
		GreyOut();
	else
		UnGreyOut();
}

void TabDoses::Rename(int number) {

	position = number;

	// Set title from its initialisation
	CString tabtitle;
	tabtitle.Format(_T("Adsorption %d"), position);

	m_caption = tabtitle;
	m_psp.pszTitle = m_caption;
	m_psp.dwFlags |= PSP_USETITLE;
}

BEGIN_MESSAGE_MAP(TabDoses, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_DOSES, &TabDoses::OnBnClickedCheckDoses)
	ON_BN_CLICKED(IDC_BUTTON_ADS_REMOVE, &TabDoses::DeletePage)
END_MESSAGE_MAP()


// TabDoses message handlers

void TabDoses::OnBnClickedCheckDoses()
{
	UpdateData(TRUE);
	EnableDoses(m_bDoses);
	UpdateData(FALSE);
}

void TabDoses::DeletePage()
{
	::SendMessage(GetParent()->GetSafeHwnd(), WM_PP_ADSORPTION_DELETE, NULL, static_cast<LPARAM>(position));
}

