// TabContinuousAdsorption.cpp : implementation file
//

#include "stdafx.h"
#include "TabContinuousAdsorption.h"

// TabContinuousAdsorption

IMPLEMENT_DYNAMIC(TabContinuousAdsorption, CMFCPropertyPage)

TabContinuousAdsorption::TabContinuousAdsorption() 
	: CMFCPropertyPage(TabContinuousAdsorption::IDD)
{
	Reinitialisation();
}

TabContinuousAdsorption::~TabContinuousAdsorption()
{
}

void TabContinuousAdsorption::DoDataExchange(CDataExchange* pDX)
{
	CMFCPropertyPage::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_TEMPS_ETALONNAGE_DEBIT, m_nTempsEtalonnageDebit);
	DDX_Control(pDX, IDC_SPIN_TEMPS_ETALONNAGE_DEBIT, m_SpinTempsEtalonnageDebit);
	DDX_Text(pDX, IDC_EDIT_TEMPS_ETALONNAGE_VOLUME_INTER, m_nTempsEtalonnageVolumeInter);
	DDX_Control(pDX, IDC_SPIN_TEMPS_ETALONNAGE_VOLUME_INTER, m_SpinTempsEtalonnageVolumeInter);
	DDX_Text(pDX, IDC_EDIT_TEMPS_EQUILIBRE_CONTINUE, m_nTempsEquilibreContinue);
	DDX_Control(pDX, IDC_SPIN_TEMPS_EQUILIBRE_CONTINUE, m_SpinTempsEquilibreContinue);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_ADSORPTION_CONTINUE, m_fPressionFinaleAdsorptionContinue);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_ADSORPTION_CONTINUE, m_SpinPressionFinaleAdsorptionContinue);
	DDX_Text(pDX, IDC_EDIT_TEMPS_FINAL_EQUILIBRE, m_nTempsFinalEquilibre);
	DDX_Control(pDX, IDC_SPIN_TEMPS_FINAL_EQUILIBRE, m_SpinTempsFinalEquilibre);
}

BOOL TabContinuousAdsorption::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CMFCPropertyPage::OnCommand(wParam, lParam);
}

BOOL TabContinuousAdsorption::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	m_SpinTempsEtalonnageDebit.SetRange(0, 100000);
	m_SpinTempsEtalonnageDebit.SetPos(45);
	m_SpinTempsEtalonnageDebit.SetInc(-1);
	m_SpinTempsEtalonnageDebit.SetFormat(_T("%1.f"));
	m_SpinTempsEtalonnageDebit.UpdateBuddy();

	m_SpinTempsEtalonnageVolumeInter.SetRange(0, 1000000);
	m_SpinTempsEtalonnageVolumeInter.SetPos(45);
	m_SpinTempsEtalonnageVolumeInter.SetInc(-1);
	m_SpinTempsEtalonnageVolumeInter.SetFormat(_T("%1.f"));
	m_SpinTempsEtalonnageVolumeInter.UpdateBuddy();

	m_SpinTempsEquilibreContinue.SetRange(0, 1000000);
	m_SpinTempsEquilibreContinue.SetPos(15);
	m_SpinTempsEquilibreContinue.SetInc(-1);
	m_SpinTempsEquilibreContinue.SetFormat(_T("%1.f"));
	m_SpinTempsEquilibreContinue.UpdateBuddy();

	m_SpinTempsFinalEquilibre.SetRange(0, 1000000);
	m_SpinTempsFinalEquilibre.SetPos(10);
	m_SpinTempsFinalEquilibre.SetInc(-1);
	m_SpinTempsFinalEquilibre.SetFormat(_T("%1.f"));
	m_SpinTempsFinalEquilibre.UpdateBuddy();

	m_SpinPressionFinaleAdsorptionContinue.SetRange(0.0, 1000000);
	m_SpinPressionFinaleAdsorptionContinue.SetPos(0.5);
	m_SpinPressionFinaleAdsorptionContinue.SetInc(-0.001);
	m_SpinPressionFinaleAdsorptionContinue.SetFormat(_T("%1.3f"));
	m_SpinPressionFinaleAdsorptionContinue.UpdateBuddy();

	EnableAdsorptionContinue();

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL TabContinuousAdsorption::OnApply()
{
	WriteData();
	return CPropertyPage::OnApply();
}

void TabContinuousAdsorption::OnCancel()
{
	m_fPressionFinaleAdsorptionContinue = allSettings.pression_finale_adsorption_continue;
	m_nTempsEquilibreContinue = allSettings.temps_equilibre_continue;
	m_nTempsEtalonnageDebit = allSettings.temps_etalonnage_debit;
	m_nTempsEtalonnageVolumeInter = allSettings.temps_etalonnage_volume_inter;
	m_nTempsFinalEquilibre = allSettings.temps_final_equilibre;

	CPropertyPage::OnCancel();
}

void TabContinuousAdsorption::OnOK()
{
	WriteData();
	CPropertyPage::OnOK();
}

void TabContinuousAdsorption::Reinitialisation()
{
	m_nTempsEtalonnageDebit = 45;
	m_nTempsEtalonnageVolumeInter = 45;
	m_nTempsEquilibreContinue = 15;
	m_fPressionFinaleAdsorptionContinue = 0.5;
	m_nTempsFinalEquilibre = 10;

	WriteData();
}

void TabContinuousAdsorption::WriteData()
{
	allSettings.pression_finale_adsorption_continue = m_fPressionFinaleAdsorptionContinue;
	allSettings.temps_equilibre_continue = m_nTempsEquilibreContinue;
	allSettings.temps_etalonnage_debit = m_nTempsEtalonnageDebit;
	allSettings.temps_etalonnage_volume_inter = m_nTempsEtalonnageVolumeInter;
	allSettings.temps_final_equilibre = m_nTempsFinalEquilibre;
}

void TabContinuousAdsorption::EnableAdsorptionContinue(void)
{
}


BEGIN_MESSAGE_MAP(TabContinuousAdsorption, CMFCPropertyPage)
END_MESSAGE_MAP()

// TabContinuousAdsorption message handlers

void TabContinuousAdsorption::OnBnClickedCheckAdsorptionContinue()
{
	UpdateData(TRUE);
	EnableAdsorptionContinue();
	UpdateData(FALSE);
}
