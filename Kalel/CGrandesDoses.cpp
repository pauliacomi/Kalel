#include "stdafx.h"
#include "Kalel.h"
#include "CGrandesDoses.h"


using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CGrandesDoses

IMPLEMENT_DYNAMIC(CGrandesDoses, CPropertyPage)

CGrandesDoses::CGrandesDoses()
	: CPropertyPage(CGrandesDoses::IDD)
{
	Reinitialisation();
}

CGrandesDoses::~CGrandesDoses()
{
}

void CGrandesDoses::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_GRANDES_DOSES, m_CheckGrandesDoses);
	DDX_Check(pDX, IDC_CHECK_GRANDES_DOSES, m_bGrandesDoses);
	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_GRANDES_DOSES, m_fDeltaPressionGrandesDoses);
	//DDV_MinMaxFloat(pDX, m_fDeltaPressionGrandesDoses, 1.0, 10.0);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_GRANDES_DOSES, m_SpinDeltaPressionGrandesDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_GRANDES_DOSES, m_nTempsVolumeGrandesDoses);
	//DDV_MinMaxFloat(pDX, m_nTempsVolumeGrandesDoses, 2, 120);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_GRANDES_DOSES, m_SpinTempsVolumeGrandesDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_ADSORPTION_GRANDES_DOSES, m_nTempsAdsorptionGrandesDoses);
	//DDV_MinMaxFloat(pDX, m_nTempsAdsorptionGrandesDoses, 30, 300);
	DDX_Control(pDX, IDC_SPIN_TEMPS_ADSORPTION_GRANDES_DOSES, m_SpinTempsAdsorptionGrandesDoses);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_GRANDES_DOSES, m_fPressionFinaleGrandesDoses);
	//DDV_MinMaxFloat(pDX, m_fPressionFinaleGrandesDoses, 2, 80);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_GRANDES_DOSES, m_SpinPressionFinaleGrandesDoses);
}

BOOL CGrandesDoses::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CGrandesDoses, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_GRANDES_DOSES, &CGrandesDoses::OnBnClickedCheckGrandesDoses)
END_MESSAGE_MAP()


// Gestionnaires de messages de CGrandesDoses

BOOL CGrandesDoses::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//m_SpinDeltaPressionGrandesDoses.SetRange(1.0,10.0);
	m_SpinDeltaPressionGrandesDoses.SetRange(0,100000000);
	m_SpinDeltaPressionGrandesDoses.SetPos(1.000);
	m_SpinDeltaPressionGrandesDoses.SetInc(-0.001);
	m_SpinDeltaPressionGrandesDoses.SetFormat("%1.3f");
	m_SpinDeltaPressionGrandesDoses.UpdateBuddy();

	
	//m_SpinTempsVolumeGrandesDoses.SetRange(2.0,120.0);
	m_SpinTempsVolumeGrandesDoses.SetRange(0,1000000);
	m_SpinTempsVolumeGrandesDoses.SetPos(5);
	m_SpinTempsVolumeGrandesDoses.SetInc(-1);
	m_SpinTempsVolumeGrandesDoses.SetFormat("%1.f");
	m_SpinTempsVolumeGrandesDoses.UpdateBuddy();

	//m_SpinTempsAdsorptionGrandesDoses.SetRange(30.0,300.0);
	m_SpinTempsAdsorptionGrandesDoses.SetRange(0,1000000);
	m_SpinTempsAdsorptionGrandesDoses.SetPos(90);
	m_SpinTempsAdsorptionGrandesDoses.SetInc(-1);
	m_SpinTempsAdsorptionGrandesDoses.SetFormat("%1.f");
	m_SpinTempsAdsorptionGrandesDoses.UpdateBuddy();

	//m_SpinPressionFinaleGrandesDoses.SetRange(2.0,80.0);
	m_SpinPressionFinaleGrandesDoses.SetRange(0,10000000);
	m_SpinPressionFinaleGrandesDoses.SetPos(5);
	m_SpinPressionFinaleGrandesDoses.SetInc(-0.001);
	m_SpinPressionFinaleGrandesDoses.SetFormat("%1.3f");
	m_SpinPressionFinaleGrandesDoses.UpdateBuddy();

	EnableGrandesDoses(m_bGrandesDoses);

	ActionCheck_GrandesDoses();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void CGrandesDoses::Reinitialisation()
{
	m_bGrandesDoses = FALSE;
	m_fDeltaPressionGrandesDoses = 1.0;
	m_nTempsVolumeGrandesDoses = 5;
	m_nTempsAdsorptionGrandesDoses = 90;
	m_fPressionFinaleGrandesDoses = 5.0;

	Enregistrement_donnees_actuelles();

	DegriserCheck_GrandesDoses();
}



void CGrandesDoses::OnBnClickedCheckGrandesDoses()
{
	UpdateData(TRUE);
	EnableGrandesDoses(m_bGrandesDoses);
	UpdateData(FALSE);
}


void CGrandesDoses::EnableGrandesDoses(BOOL active)
{
	GetDlgItem (IDC_STATIC_GD_DPPGD_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_DPPGD_BAR)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_TEVR_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_TEVR_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_TEA_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_TEA_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_PF_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_GD_PF_BAR)->EnableWindow (active) ;

	GetDlgItem (IDC_EDIT_DELTA_PRESSION_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_DELTA_PRESSION_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_VOLUME_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_VOLUME_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_ADSORPTION_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_ADSORPTION_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_PRESSION_FINALE_GRANDES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_PRESSION_FINALE_GRANDES_DOSES)->EnableWindow (active) ;
}

BOOL CGrandesDoses::OnApply()
{
	Enregistrement_donnees_actuelles();

	return CPropertyPage::OnApply();
}

void CGrandesDoses::OnCancel()
{
	m_bGrandesDoses = donnees_actuelles.a_effectuer;

	m_fDeltaPressionGrandesDoses = donnees_actuelles.delta_pression;
	//m_SpinDeltaPressionGrandesDoses.SetPos(m_fDeltaPressionGrandesDoses);

	m_fPressionFinaleGrandesDoses = donnees_actuelles.pression_finale;
	//m_SpinPressionFinaleGrandesDoses.SetPos(m_fPressionFinaleGrandesDoses);

	m_nTempsAdsorptionGrandesDoses = donnees_actuelles.temps_adsorption;
	//m_SpinTempsAdsorptionGrandesDoses.SetPos(m_nTempsAdsorptionGrandesDoses);

	m_nTempsVolumeGrandesDoses = donnees_actuelles.temps_volume;
	//m_SpinTempsVolumeGrandesDoses.SetPos(m_nTempsVolumeGrandesDoses);
	

	CPropertyPage::OnCancel();
}

void CGrandesDoses::OnOK()
{
	Enregistrement_donnees_actuelles();

	CPropertyPage::OnOK();
}


void CGrandesDoses::Enregistrement_donnees_actuelles()
{
	donnees_actuelles.a_effectuer = m_bGrandesDoses;
	donnees_actuelles.delta_pression = m_fDeltaPressionGrandesDoses;
	donnees_actuelles.pression_finale = m_fPressionFinaleGrandesDoses;
	donnees_actuelles.temps_adsorption = m_nTempsAdsorptionGrandesDoses;
	donnees_actuelles.temps_volume = m_nTempsVolumeGrandesDoses;
}

void CGrandesDoses::GriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_GRANDES_DOSES)->EnableWindow (FALSE) ;
}

void CGrandesDoses::DegriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_GRANDES_DOSES)->EnableWindow (TRUE) ;
}

void CGrandesDoses::GriserCheck_GrandesDoses()
{	Check_GrandesDoses = GREY_OUT;}

void CGrandesDoses::DegriserCheck_GrandesDoses()
{	Check_GrandesDoses = UN_GREY_OUT;}

void CGrandesDoses::ActionCheck_GrandesDoses()
{
	if(Check_GrandesDoses == GREY_OUT)
		GriserAEffectuer();
	else
		DegriserAEffectuer();
}