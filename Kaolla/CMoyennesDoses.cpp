#include "stdafx.h"
#include "Kaolla.h"
#include "CMoyennesDoses.h"


using namespace std;

//////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CMoyennesDoses

IMPLEMENT_DYNAMIC(CMoyennesDoses, CPropertyPage)

CMoyennesDoses::CMoyennesDoses()
	: CPropertyPage(CMoyennesDoses::IDD)
{
	Reinitialisation();
}

CMoyennesDoses::~CMoyennesDoses()
{
}

void CMoyennesDoses::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_CHECK_MOYENNES_DOSES, m_CheckMoyennesDoses);
	DDX_Check(pDX, IDC_CHECK_MOYENNES_DOSES, m_bMoyennesDoses);
	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_MOYENNES_DOSES, m_fDeltaPressionMoyennesDoses);
	//DDV_MinMaxFloat(pDX, m_fDeltaPressionMoyennesDoses, 1.0, 10.0);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_MOYENNES_DOSES, m_SpinDeltaPressionMoyennesDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_MOYENNES_DOSES, m_nTempsVolumeMoyennesDoses);
	//DDV_MinMaxFloat(pDX, m_nTempsVolumeMoyennesDoses, 2, 120);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_MOYENNES_DOSES, m_SpinTempsVolumeMoyennesDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_ADSORPTION_MOYENNES_DOSES, m_nTempsAdsorptionMoyennesDoses);
	//DDV_MinMaxFloat(pDX, m_nTempsAdsorptionMoyennesDoses, 30, 300);
	DDX_Control(pDX, IDC_SPIN_TEMPS_ADSORPTION_MOYENNES_DOSES, m_SpinTempsAdsorptionMoyennesDoses);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_MOYENNES_DOSES, m_fPressionFinaleMoyennesDoses);
	//DDV_MinMaxFloat(pDX, m_fPressionFinaleMoyennesDoses, 2, 80);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_MOYENNES_DOSES, m_SpinPressionFinaleMoyennesDoses);
}

BOOL CMoyennesDoses::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CMoyennesDoses, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_MOYENNES_DOSES, &CMoyennesDoses::OnBnClickedCheckMoyennesDoses)
END_MESSAGE_MAP()


// Gestionnaires de messages de CMoyennesDoses

BOOL CMoyennesDoses::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	//m_SpinDeltaPressionMoyennesDoses.SetRange(1.0,10.0);
	m_SpinDeltaPressionMoyennesDoses.SetRange(0,100000000);
	m_SpinDeltaPressionMoyennesDoses.SetPos(1.000);
	m_SpinDeltaPressionMoyennesDoses.SetInc(-0.001);
	m_SpinDeltaPressionMoyennesDoses.SetFormat("%1.3f");
	m_SpinDeltaPressionMoyennesDoses.UpdateBuddy();

	
	//m_SpinTempsVolumeMoyennesDoses.SetRange(2.0,120.0);
	m_SpinTempsVolumeMoyennesDoses.SetRange(0,1000000);
	m_SpinTempsVolumeMoyennesDoses.SetPos(5);
	m_SpinTempsVolumeMoyennesDoses.SetInc(-1);
	m_SpinTempsVolumeMoyennesDoses.SetFormat("%1.f");
	m_SpinTempsVolumeMoyennesDoses.UpdateBuddy();

	//m_SpinTempsAdsorptionMoyennesDoses.SetRange(30.0,300.0);
	m_SpinTempsAdsorptionMoyennesDoses.SetRange(0,1000000);
	m_SpinTempsAdsorptionMoyennesDoses.SetPos(90);
	m_SpinTempsAdsorptionMoyennesDoses.SetInc(-1);
	m_SpinTempsAdsorptionMoyennesDoses.SetFormat("%1.f");
	m_SpinTempsAdsorptionMoyennesDoses.UpdateBuddy();

	//m_SpinPressionFinaleMoyennesDoses.SetRange(2.0,80.0);
	m_SpinPressionFinaleMoyennesDoses.SetRange(0,10000000);
	m_SpinPressionFinaleMoyennesDoses.SetPos(5);
	m_SpinPressionFinaleMoyennesDoses.SetInc(-0.001);
	m_SpinPressionFinaleMoyennesDoses.SetFormat("%1.3f");
	m_SpinPressionFinaleMoyennesDoses.UpdateBuddy();

	EnableMoyennesDoses(m_bMoyennesDoses);

	ActionCheck_MoyennesDoses();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void CMoyennesDoses::Reinitialisation()
{
	m_bMoyennesDoses = FALSE;
	m_fDeltaPressionMoyennesDoses = 1.0;
	m_nTempsVolumeMoyennesDoses = 5;
	m_nTempsAdsorptionMoyennesDoses = 90;
	m_fPressionFinaleMoyennesDoses = 5.0;

	Enregistrement_donnees_actuelles();

	DegriserCheck_MoyennesDoses();
}



void CMoyennesDoses::OnBnClickedCheckMoyennesDoses()
{
	UpdateData(TRUE);
	EnableMoyennesDoses(m_bMoyennesDoses);
	UpdateData(FALSE);
}


void CMoyennesDoses::EnableMoyennesDoses(BOOL active)
{
	GetDlgItem (IDC_STATIC_MY_DPPMY_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_DPPMY_BAR)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_TEVR_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_TEVR_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_TEA_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_TEA_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_PF_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MY_PF_BAR)->EnableWindow (active) ;

	GetDlgItem (IDC_EDIT_DELTA_PRESSION_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_DELTA_PRESSION_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_VOLUME_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_VOLUME_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_ADSORPTION_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_ADSORPTION_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_PRESSION_FINALE_MOYENNES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_PRESSION_FINALE_MOYENNES_DOSES)->EnableWindow (active) ;
}

BOOL CMoyennesDoses::OnApply()
{
	Enregistrement_donnees_actuelles();

	return CPropertyPage::OnApply();
}

void CMoyennesDoses::OnCancel()
{
	m_bMoyennesDoses = donnees_actuelles.a_effectuer;

	m_fDeltaPressionMoyennesDoses = donnees_actuelles.delta_pression;
	//m_SpinDeltaPressionMoyennesDoses.SetPos(m_fDeltaPressionMoyennesDoses);

	m_fPressionFinaleMoyennesDoses = donnees_actuelles.pression_finale;
	//m_SpinPressionFinaleMoyennesDoses.SetPos(m_fPressionFinaleMoyennesDoses);

	m_nTempsAdsorptionMoyennesDoses = donnees_actuelles.temps_adsorption;
	//m_SpinTempsAdsorptionMoyennesDoses.SetPos(m_nTempsAdsorptionMoyennesDoses);

	m_nTempsVolumeMoyennesDoses = donnees_actuelles.temps_volume;
	//m_SpinTempsVolumeMoyennesDoses.SetPos(m_nTempsVolumeMoyennesDoses);
	

	CPropertyPage::OnCancel();
}

void CMoyennesDoses::OnOK()
{
	Enregistrement_donnees_actuelles();

	CPropertyPage::OnOK();
}


void CMoyennesDoses::Enregistrement_donnees_actuelles()
{
	donnees_actuelles.a_effectuer = m_bMoyennesDoses;
	donnees_actuelles.delta_pression = m_fDeltaPressionMoyennesDoses;
	donnees_actuelles.pression_finale = m_fPressionFinaleMoyennesDoses;
	donnees_actuelles.temps_adsorption = m_nTempsAdsorptionMoyennesDoses;
	donnees_actuelles.temps_volume = m_nTempsVolumeMoyennesDoses;
}

void CMoyennesDoses::GriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_MOYENNES_DOSES)->EnableWindow (FALSE) ;
}

void CMoyennesDoses::DegriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_MOYENNES_DOSES)->EnableWindow (TRUE) ;
}

void CMoyennesDoses::GriserCheck_MoyennesDoses()
{	Check_MoyennesDoses = GREY_OUT;}

void CMoyennesDoses::DegriserCheck_MoyennesDoses()
{	Check_MoyennesDoses = UN_GREY_OUT;}

void CMoyennesDoses::ActionCheck_MoyennesDoses()
{
	if(Check_MoyennesDoses == GREY_OUT)
		GriserAEffectuer();
	else
		DegriserAEffectuer();
}