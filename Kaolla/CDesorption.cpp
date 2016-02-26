#include "stdafx.h"
#include "Kaolla.h"
#include "CDesorption.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CDesorption



IMPLEMENT_DYNAMIC(CDesorption, CPropertyPage)

CDesorption::CDesorption()
	: CPropertyPage(CDesorption::IDD)
{
	Reinitialisation();
}

CDesorption::~CDesorption()
{
}

void CDesorption::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_DESORPTION, m_CheckDesorption);
	DDX_Check(pDX, IDC_CHECK_DESORPTION, m_bDesorption);
	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_DESORPTION, m_fDeltaPressionDesorption);
	//DDV_MinMaxFloat(pDX, m_fDeltaPressionDesorption, 0.5, 10.0);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_DESORPTION, m_SpinDeltaPressionDesorption);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_DESORPTION, m_nTempsVolumeDesorption);
	//DDV_MinMaxFloat(pDX, m_nTempsVolumeDesorption, 2, 120);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_DESORPTION, m_SpinTempsVolumeDesorption);
	DDX_Text(pDX, IDC_EDIT_TEMPS_DESORPTION, m_nTempsDesorption);
	//DDV_MinMaxFloat(pDX, m_nTempsDesorption, 30, 300);
	DDX_Control(pDX, IDC_SPIN_TEMPS_DESORPTION, m_SpinTempsDesorption);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_DESORPTION, m_fPressionFinaleDesorption);
	//DDV_MinMaxFloat(pDX, m_fPressionFinaleDesorption, 0, 2);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_DESORPTION, m_SpinPressionFinaleDesorption);
	DDX_Check(pDX, IDC_CHECK_DESORPTION_DERNIERE_ETAPE, m_bDerniereEtapeDesorption);
}

BOOL CDesorption::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CDesorption, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_DESORPTION, &CDesorption::OnBnClickedCheckDesorption)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDesorption

BOOL CDesorption::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//m_SpinDeltaPressionDesorption.SetRange(0.5,10.0);
	m_SpinDeltaPressionDesorption.SetRange(0,10000000);
	m_SpinDeltaPressionDesorption.SetPos(1.000);
	m_SpinDeltaPressionDesorption.SetInc(-0.001);
	m_SpinDeltaPressionDesorption.SetFormat("%1.3f");
	m_SpinDeltaPressionDesorption.UpdateBuddy();

	
	//m_SpinTempsVolumeDesorption.SetRange(2.0,120.0);
	m_SpinTempsVolumeDesorption.SetRange(0,100000000);
	m_SpinTempsVolumeDesorption.SetPos(5);
	m_SpinTempsVolumeDesorption.SetInc(-1);
	m_SpinTempsVolumeDesorption.SetFormat("%1.f");
	m_SpinTempsVolumeDesorption.UpdateBuddy();

	//m_SpinTempsDesorption.SetRange(30.0,300.0);
	m_SpinTempsDesorption.SetRange(0,1000000000);
	m_SpinTempsDesorption.SetPos(90);
	m_SpinTempsDesorption.SetInc(-1);
	m_SpinTempsDesorption.SetFormat("%1.f");
	m_SpinTempsDesorption.UpdateBuddy();

	//m_SpinPressionFinaleDesorption.SetRange(0.0,2.0);
	m_SpinPressionFinaleDesorption.SetRange(0.0,100000);
	m_SpinPressionFinaleDesorption.SetPos(0.1);
	m_SpinPressionFinaleDesorption.SetInc(-0.001);
	m_SpinPressionFinaleDesorption.SetFormat("%1.3f");
	m_SpinPressionFinaleDesorption.UpdateBuddy();

	EnableDesorption(m_bDesorption);

	ActionCheck_Desorption();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void CDesorption::Reinitialisation()
{
	m_bDesorption = FALSE;
	m_fDeltaPressionDesorption = 1.0;
	m_nTempsVolumeDesorption = 5;
	m_nTempsDesorption = 90;
	m_fPressionFinaleDesorption = 0.1;
	m_bDerniereEtapeDesorption = FALSE;

	DegriserCheck_Desorption();

	Enregistrement_donnees_actuelles();
}



void CDesorption::OnBnClickedCheckDesorption()
{
	UpdateData(TRUE);
	EnableDesorption(m_bDesorption);
	UpdateData(FALSE);
}


void CDesorption::EnableDesorption(BOOL active)
{
	GetDlgItem (IDC_STATIC_D_DPD_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_DPD_BAR)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_TEVR_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_TEVR_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_TED_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_TED_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_PF_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_D_PF_BAR)->EnableWindow (active) ;

	GetDlgItem (IDC_EDIT_DELTA_PRESSION_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_DELTA_PRESSION_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_VOLUME_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_VOLUME_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_PRESSION_FINALE_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_PRESSION_FINALE_DESORPTION)->EnableWindow (active) ;
	GetDlgItem (IDC_CHECK_DESORPTION_DERNIERE_ETAPE)->EnableWindow (active) ;
}


BOOL CDesorption::OnApply()
{
	Enregistrement_donnees_actuelles();

	return CPropertyPage::OnApply();
}

void CDesorption::OnCancel()
{
	m_bDesorption = donnees_actuelles.a_effectuer;

	m_fDeltaPressionDesorption = donnees_actuelles.delta_pression;
	//m_SpinDeltaPressionDesorption.SetPos(m_fDeltaPressionDesorption);

	m_bDerniereEtapeDesorption = donnees_actuelles.derniere_etape;

	m_fPressionFinaleDesorption = donnees_actuelles.pression_finale;
	//m_SpinPressionFinaleDesorption.SetPos(m_fPressionFinaleDesorption);

	m_nTempsDesorption = donnees_actuelles.temps_desorption;
	//m_SpinTempsDesorption.SetPos(m_nTempsDesorption);

	m_nTempsVolumeDesorption = donnees_actuelles.temps_volume;
	//m_SpinTempsVolumeDesorption.SetPos(m_nTempsVolumeDesorption);

	CPropertyPage::OnCancel();
}

void CDesorption::OnOK()
{
	Enregistrement_donnees_actuelles();

	CPropertyPage::OnOK();
}


void CDesorption::Enregistrement_donnees_actuelles()
{
	donnees_actuelles.a_effectuer = m_bDesorption;
	donnees_actuelles.delta_pression = m_fDeltaPressionDesorption;
	donnees_actuelles.derniere_etape = m_bDerniereEtapeDesorption;
	donnees_actuelles.pression_finale = m_fPressionFinaleDesorption;
	donnees_actuelles.temps_desorption = m_nTempsDesorption;
	donnees_actuelles.temps_volume = m_nTempsVolumeDesorption;
}

void CDesorption::GriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_DESORPTION)->EnableWindow (FALSE) ;
}

void CDesorption::DegriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_DESORPTION)->EnableWindow (TRUE) ;
}

void CDesorption::GriserCheck_Desorption()
{	Check_Desorption = GREY_OUT;}

void CDesorption::DegriserCheck_Desorption()
{	Check_Desorption = UN_GREY_OUT;}

void CDesorption::ActionCheck_Desorption()
{
	if(Check_Desorption == GREY_OUT)
		GriserAEffectuer();
	else
		DegriserAEffectuer();
}