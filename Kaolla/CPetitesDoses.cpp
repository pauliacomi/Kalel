#include "stdafx.h"
#include "Kaolla.h"
#include "CPetitesDoses.h"


using namespace std;

////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CPetitesDoses

IMPLEMENT_DYNAMIC(CPetitesDoses, CPropertyPage)

CPetitesDoses::CPetitesDoses()
	: CPropertyPage(CPetitesDoses::IDD)
{
	Reinitialisation();
}

CPetitesDoses::~CPetitesDoses()
{
}

void CPetitesDoses::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_PETITES_DOSES, m_CheckPetitesDoses);
	DDX_Check(pDX, IDC_CHECK_PETITES_DOSES, m_bPetitesDoses);
	DDX_Text(pDX, IDC_EDIT_DELTA_PRESSION_PETITES_DOSES, m_fDeltaPressionPetitesDoses);
	DDX_Control(pDX, IDC_SPIN_DELTA_PRESSION_PETITES_DOSES, m_SpinDeltaPressionPetitesDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VOLUME_PETITES_DOSES, m_nTempsVolumePetitesDoses);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VOLUME_PETITES_DOSES, m_SpinTempsVolumePetitesDoses);
	DDX_Text(pDX, IDC_EDIT_TEMPS_ADSORPTION_PETITES_DOSES, m_nTempsAdsorptionPetitesDoses);
	DDX_Control(pDX, IDC_SPIN_TEMPS_ADSORPTION_PETITES_DOSES, m_SpinTempsAdsorptionPetitesDoses);
	DDX_Text(pDX, IDC_EDIT_PRESSION_FINALE_PETITES_DOSES, m_fPressionFinalePetitesDoses);
	DDX_Control(pDX, IDC_SPIN_PRESSION_FINALE_PETITES_DOSES, m_SpinPressionFinalePetitesDoses);
}


BOOL CPetitesDoses::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CPetitesDoses, CPropertyPage)
	ON_BN_CLICKED(IDC_CHECK_PETITES_DOSES, &CPetitesDoses::OnBnClickedCheckPetitesDoses)
END_MESSAGE_MAP()


// Gestionnaires de messages de CPetitesDoses


BOOL CPetitesDoses::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	m_SpinDeltaPressionPetitesDoses.SetRange(0,1000000);
	m_SpinDeltaPressionPetitesDoses.SetPos(0.100);
	m_SpinDeltaPressionPetitesDoses.SetInc(-0.001);
	m_SpinDeltaPressionPetitesDoses.SetFormat("%1.3f");
	m_SpinDeltaPressionPetitesDoses.UpdateBuddy();

	m_SpinTempsVolumePetitesDoses.SetRange(0,100000000);
	m_SpinTempsVolumePetitesDoses.SetPos(5);
	m_SpinTempsVolumePetitesDoses.SetInc(-1);
	m_SpinTempsVolumePetitesDoses.SetFormat("%1.f");
	m_SpinTempsVolumePetitesDoses.UpdateBuddy();

	m_SpinTempsAdsorptionPetitesDoses.SetRange(0,10000000);
	m_SpinTempsAdsorptionPetitesDoses.SetPos(90);
	m_SpinTempsAdsorptionPetitesDoses.SetInc(-1);
	m_SpinTempsAdsorptionPetitesDoses.SetFormat("%1.f");
	m_SpinTempsAdsorptionPetitesDoses.UpdateBuddy();

	m_SpinPressionFinalePetitesDoses.SetRange(0,100000000);
	m_SpinPressionFinalePetitesDoses.SetPos(1);
	m_SpinPressionFinalePetitesDoses.SetInc(-0.001);
	m_SpinPressionFinalePetitesDoses.SetFormat("%1.3f");
	m_SpinPressionFinalePetitesDoses.UpdateBuddy();

	EnablePetitesDoses(m_bPetitesDoses);
	ActionCheck_PetitesDoses();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void CPetitesDoses::Reinitialisation()
{
	m_bPetitesDoses = FALSE;
	m_fDeltaPressionPetitesDoses = 0.1f;
	m_nTempsVolumePetitesDoses = 5;
	m_nTempsAdsorptionPetitesDoses = 90;
	m_fPressionFinalePetitesDoses = 1.0f;

	Check_PetitesDoses = UN_GREY_OUT;

	Enregistrement_donnees_actuelles();
}


void CPetitesDoses::OnBnClickedCheckPetitesDoses()
{
	UpdateData(TRUE);
	EnablePetitesDoses(m_bPetitesDoses);
	UpdateData(FALSE);
}


void CPetitesDoses::EnablePetitesDoses(BOOL active)
{
	GetDlgItem (IDC_STATIC_PD_DPPPD_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_DPPPD_BAR)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_TEVR_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_TEVR_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_TEA_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_TEA_MIN)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_PF_TEXT)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_PD_PF_BAR)->EnableWindow (active) ;

	GetDlgItem (IDC_EDIT_DELTA_PRESSION_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_DELTA_PRESSION_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_VOLUME_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_VOLUME_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_ADSORPTION_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_ADSORPTION_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_PRESSION_FINALE_PETITES_DOSES)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_PRESSION_FINALE_PETITES_DOSES)->EnableWindow (active) ;
}

BOOL CPetitesDoses::OnApply()
{
	Enregistrement_donnees_actuelles();

	return CPropertyPage::OnApply();
}

void CPetitesDoses::OnCancel()
{
	m_bPetitesDoses = donnees_actuelles.a_effectuer;
	m_fDeltaPressionPetitesDoses = donnees_actuelles.delta_pression;
	m_fPressionFinalePetitesDoses = donnees_actuelles.pression_finale;
	m_nTempsAdsorptionPetitesDoses = donnees_actuelles.temps_adsorption;
	m_nTempsVolumePetitesDoses = donnees_actuelles.temps_volume;

	CPropertyPage::OnCancel();
}

void CPetitesDoses::OnOK()
{
	Enregistrement_donnees_actuelles();

	CPropertyPage::OnOK();
}

void CPetitesDoses::Enregistrement_donnees_actuelles()
{
	donnees_actuelles.a_effectuer = m_bPetitesDoses;
	donnees_actuelles.delta_pression = m_fDeltaPressionPetitesDoses;
	donnees_actuelles.pression_finale = m_fPressionFinalePetitesDoses;
	donnees_actuelles.temps_adsorption = m_nTempsAdsorptionPetitesDoses;
	donnees_actuelles.temps_volume = m_nTempsVolumePetitesDoses;
}

void CPetitesDoses::GriserAEffectuer()
{
	GetDlgItem (IDC_CHECK_PETITES_DOSES)->EnableWindow (FALSE) ;
}

void CPetitesDoses::DegriserAEffectuer()
{
	UpdateData(TRUE);
	GetDlgItem (IDC_CHECK_PETITES_DOSES)->EnableWindow (TRUE) ;
	UpdateData(FALSE);
}

void CPetitesDoses::GriserCheck_PetitesDoses()
{	Check_PetitesDoses = GREY_OUT;}

void CPetitesDoses::DegriserCheck_PetitesDoses()
{	Check_PetitesDoses = UN_GREY_OUT;}

void CPetitesDoses::ActionCheck_PetitesDoses()
{
	if(Check_PetitesDoses == GREY_OUT)
		GriserAEffectuer();
	else
		DegriserAEffectuer();
}