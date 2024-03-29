// TabDesorption.cpp : implementation file
//

#include "stdafx.h"
#include "TabDesorption.h"

#include "DefineDialogMessages.h"		// For custom message definitions

// TabDesorption

IMPLEMENT_DYNAMIC(TabDesorption, CMFCPropertyPage)

TabDesorption::TabDesorption(int number)
	: CMFCPropertyPage(TabDesorption::IDD)
{
	// Set title from its initialisation
	Rename(number);

	// Variable for greying out
	checkDesorption = false;
}

TabDesorption::~TabDesorption()
{
}

void TabDesorption::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_DESORPTION, m_fDeltaPressionDesorption);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_DESORPTION, m_SpinDeltaPressionDesorption);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_DESORPTION, m_nTempsVolumeDesorption);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_DESORPTION, m_SpinTempsVolumeDesorption);
	DDX_Text(pDX, IDC_EDIT_TEMPS_DESORPTION, m_nTempsDesorption);
	DDX_Control(pDX, IDC_SPIN_TEMPS_DESORPTION, m_SpinTempsDesorption);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_DESORPTION, m_fPressionFinaleDesorption);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_DESORPTION, m_SpinPressionFinaleDesorption);
}


BOOL TabDesorption::OnInitDialog()
{
	// Get settings from storage
	m_fDeltaPressionDesorption = allSettings.delta_pression;
	m_fPressionFinaleDesorption = allSettings.pression_finale;
	m_nTempsDesorption = allSettings.temps_desorption;
	m_nTempsVolumeDesorption = allSettings.temps_volume;

	// Initialize dialog
	CMFCPropertyPage::OnInitDialog();

	m_SpinDeltaPressionDesorption.SetRange(0, 10000000);
	m_SpinDeltaPressionDesorption.SetInc(-0.001);
	m_SpinDeltaPressionDesorption.SetFormat(_T("%1.3f"));
	m_SpinDeltaPressionDesorption.UpdateBuddy();

	m_SpinTempsVolumeDesorption.SetRange(0, 100000000);
	m_SpinTempsVolumeDesorption.SetInc(-1);
	m_SpinTempsVolumeDesorption.SetFormat(_T("%1.f"));
	m_SpinTempsVolumeDesorption.UpdateBuddy();

	m_SpinTempsDesorption.SetRange(0, 1000000000);
	m_SpinTempsDesorption.SetInc(-1);
	m_SpinTempsDesorption.SetFormat(_T("%1.f"));
	m_SpinTempsDesorption.UpdateBuddy();

	m_SpinPressionFinaleDesorption.SetRange(0.0, 100000);
	m_SpinPressionFinaleDesorption.SetInc(-0.001);
	m_SpinPressionFinaleDesorption.SetFormat(_T("%1.3f"));
	m_SpinPressionFinaleDesorption.UpdateBuddy();

	ToggleGreyOut();

	return TRUE;  // return TRUE unless you set the focus to a control
}


BOOL TabDesorption::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CMFCPropertyPage::OnCommand(wParam, lParam);
}

BOOL TabDesorption::OnApply()
{
	WriteData();
	return CMFCPropertyPage::OnApply();
}

void TabDesorption::OnCancel()
{
	CMFCPropertyPage::OnCancel();
}

void TabDesorption::OnOK()
{
	WriteData();
	CMFCPropertyPage::OnOK();
}

void TabDesorption::Reinitialisation()
{
	m_fDeltaPressionDesorption = allSettings.delta_pression;
	m_fPressionFinaleDesorption = allSettings.pression_finale;
	m_nTempsDesorption = allSettings.temps_desorption;
	m_nTempsVolumeDesorption = allSettings.temps_volume;
	
	UpdateData(false);
}

void TabDesorption::WriteData()
{
	allSettings.delta_pression = m_fDeltaPressionDesorption;
	allSettings.pression_finale = m_fPressionFinaleDesorption;
	allSettings.temps_desorption = m_nTempsDesorption;
	allSettings.temps_volume = m_nTempsVolumeDesorption;
}

void TabDesorption::GreyOut(BOOL active)
{
	GetDlgItem(IDC_STATIC_D_DPD_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_DPD_BAR)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_TEVR_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_TEVR_MIN)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_TED_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_TED_MIN)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_PF_TEXT)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_D_PF_BAR)->EnableWindow(active);

	GetDlgItem(IDC_EDIT_DELTA_PRESSION_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_DELTA_PRESSION_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_TEMPS_VOLUME_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_TEMPS_VOLUME_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_TEMPS_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_TEMPS_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_PRESSION_FINALE_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_PRESSION_FINALE_DESORPTION)->EnableWindow(active);
	GetDlgItem(IDC_BUTTON_DES_REMOVE)->EnableWindow(active);
}


void TabDesorption::ToggleGreyOut()
{
	if (checkDesorption == true)
		GreyOut(FALSE);
	else
		GreyOut(TRUE);
}

void TabDesorption::Rename(int number) {

	position = number;

	CString tabtitle;
	tabtitle.Format(_T("Desorption %d"), position);

	m_caption = tabtitle;
	m_psp.pszTitle = m_caption;
	m_psp.dwFlags |= PSP_USETITLE;
}


BEGIN_MESSAGE_MAP(TabDesorption, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_BUTTON_DES_REMOVE, &TabDesorption::DeletePage)
END_MESSAGE_MAP()

// TabDesorption message handlers


void TabDesorption::DeletePage()
{
	::SendMessage(GetParent()->GetSafeHwnd(), UWM_PP_DESORPTION_DELETE, NULL, static_cast<LPARAM>(position));
}