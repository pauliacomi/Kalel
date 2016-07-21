// TabDesorption.cpp : implementation file
//

#include "stdafx.h"
#include "Kalel.h"
#include "TabDesorption.h"

#include "General.h"			// For some definitions

// TabDesorption

IMPLEMENT_DYNAMIC(TabDesorption, CMFCPropertyPage)

TabDesorption::TabDesorption(CString i)
	: CMFCPropertyPage(TabDesorption::IDD)
{
	// Set title from its initialisation
	m_caption = i;
	m_psp.pszTitle = m_caption;
	m_psp.dwFlags |= PSP_USETITLE;

	Reinitialisation();
}

TabDesorption::~TabDesorption()
{
}

void TabDesorption::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_DESORPTION, m_CheckDesorption);
	DDX_Check(pDX, IDC_CHECK_DESORPTION, m_bDesorption);
	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_DESORPTION, m_fDeltaPressionDesorption);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_DESORPTION, m_SpinDeltaPressionDesorption);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_DESORPTION, m_nTempsVolumeDesorption);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_DESORPTION, m_SpinTempsVolumeDesorption);
	DDX_Text(pDX, IDC_EDIT_TEMPS_DESORPTION, m_nTempsDesorption);
	DDX_Control(pDX, IDC_SPIN_TEMPS_DESORPTION, m_SpinTempsDesorption);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_DESORPTION, m_fPressionFinaleDesorption);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_DESORPTION, m_SpinPressionFinaleDesorption);
	DDX_Check(pDX, IDC_CHECK_DESORPTION_DERNIERE_ETAPE, m_bDerniereEtapeDesorption);
}


BOOL TabDesorption::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CMFCPropertyPage::OnCommand(wParam, lParam);
}

BOOL TabDesorption::OnInitDialog()
{
	CMFCPropertyPage::OnInitDialog();

	m_SpinDeltaPressionDesorption.SetRange(0, 10000000);
	m_SpinDeltaPressionDesorption.SetPos(1.000);
	m_SpinDeltaPressionDesorption.SetInc(-0.001);
	m_SpinDeltaPressionDesorption.SetFormat("%1.3f");
	m_SpinDeltaPressionDesorption.UpdateBuddy();

	m_SpinTempsVolumeDesorption.SetRange(0, 100000000);
	m_SpinTempsVolumeDesorption.SetPos(5);
	m_SpinTempsVolumeDesorption.SetInc(-1);
	m_SpinTempsVolumeDesorption.SetFormat("%1.f");
	m_SpinTempsVolumeDesorption.UpdateBuddy();

	m_SpinTempsDesorption.SetRange(0, 1000000000);
	m_SpinTempsDesorption.SetPos(90);
	m_SpinTempsDesorption.SetInc(-1);
	m_SpinTempsDesorption.SetFormat("%1.f");
	m_SpinTempsDesorption.UpdateBuddy();

	m_SpinPressionFinaleDesorption.SetRange(0.0, 100000);
	m_SpinPressionFinaleDesorption.SetPos(0.1);
	m_SpinPressionFinaleDesorption.SetInc(-0.001);
	m_SpinPressionFinaleDesorption.SetFormat("%1.3f");
	m_SpinPressionFinaleDesorption.UpdateBuddy();

	EnableDesorption(m_bDesorption);
	ActionCheck_Desorption();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL TabDesorption::OnApply()
{
	WriteData();
	return CMFCPropertyPage::OnApply();
}

void TabDesorption::OnCancel()
{
	m_bDesorption = allSettings.a_effectuer;

	m_fDeltaPressionDesorption = allSettings.delta_pression;
	m_bDerniereEtapeDesorption = allSettings.derniere_etape;
	m_fPressionFinaleDesorption = allSettings.pression_finale;
	m_nTempsDesorption = allSettings.temps_desorption;
	m_nTempsVolumeDesorption = allSettings.temps_volume;

	CMFCPropertyPage::OnCancel();
}

void TabDesorption::OnOK()
{
	WriteData();
	CMFCPropertyPage::OnOK();
}

void TabDesorption::Reinitialisation()
{
	m_bDesorption = FALSE;
	m_fDeltaPressionDesorption = 1.0;
	m_nTempsVolumeDesorption = 5;
	m_nTempsDesorption = 90;
	m_fPressionFinaleDesorption = 0.1;
	m_bDerniereEtapeDesorption = FALSE;

	CheckUnGreyOut();

	WriteData();
}

void TabDesorption::WriteData()
{
	allSettings.a_effectuer = m_bDesorption;
	allSettings.delta_pression = m_fDeltaPressionDesorption;
	allSettings.derniere_etape = m_bDerniereEtapeDesorption;
	allSettings.pression_finale = m_fPressionFinaleDesorption;
	allSettings.temps_desorption = m_nTempsDesorption;
	allSettings.temps_volume = m_nTempsVolumeDesorption;
}

void TabDesorption::EnableDesorption(BOOL active)
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
	GetDlgItem(IDC_CHECK_DESORPTION_DERNIERE_ETAPE)->EnableWindow(active);
}


void TabDesorption::GreyOut()
{
	GetDlgItem(IDC_CHECK_DESORPTION)->EnableWindow(FALSE);
}

void TabDesorption::UnGreyOut()
{
	UpdateData(TRUE);
	GetDlgItem(IDC_CHECK_DESORPTION)->EnableWindow(TRUE);
	UpdateData(FALSE);
}

void TabDesorption::CheckGreyOut()
{
	checkDesorption = GREY_OUT;
}

void TabDesorption::CheckUnGreyOut()
{
	checkDesorption = UN_GREY_OUT;
}

void TabDesorption::ActionCheck_Desorption()
{
	if (checkDesorption == GREY_OUT)
		GreyOut();
	else
		UnGreyOut();
}

BEGIN_MESSAGE_MAP(TabDesorption, CMFCPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_DESORPTION, &TabDesorption::OnBnClickedCheckDesorption)
END_MESSAGE_MAP()

// TabDesorption message handlers

void TabDesorption::OnBnClickedCheckDesorption()
{
	UpdateData(TRUE);
	EnableDesorption(m_bDesorption);
	UpdateData(FALSE);
}
