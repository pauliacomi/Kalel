#include "stdafx.h"
#include "Kalel.h"
#include "CDivers.h"

#include <sstream>

using namespace std;



//////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CDivers

IMPLEMENT_DYNAMIC(CDivers, CPropertyPage)

CDivers::CDivers()
	: CPropertyPage(CDivers::IDD)
	, m_DialogueCellule("")
{
	Reinitialisation();
}

CDivers::~CDivers()
{
}

void CDivers::DoDataExchange(CDataExchange* pDX)
{
	TRACE("Entering CDivers::DoDataExchange -- %d\n",
		pDX->m_bSaveAndValidate);
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_CELLULE, m_CBCellule);
	//DDX_CBIndex(pDX, IDC_COMBO_CELLULE, m_nNumCellule);
	DDX_CBIndex(pDX, IDC_COMBO_CELLULE, m_IndexCellule);
	//DDX_Control(pDX, IDC_BUTTON1, m_ButtonCellule);
	DDX_Control(pDX, IDC_EDIT_VOLUME_TOTAL, m_EditVolumeTotal);
	DDX_Text(pDX, IDC_EDIT_VOLUME_TOTAL, m_fVolumeTotal);
	DDX_Control(pDX, IDC_EDIT_VOLUME_CALO, m_EditVolumeCalo);
	DDX_Text(pDX, IDC_EDIT_VOLUME_CALO, m_fVolumeCalo);
	DDX_Text(pDX, IDC_EDIT_TEMPS_LIGNE_BASE, m_nTempsLigneBase);
	//DDV_MinMaxFloat(pDX, m_nTempsLigneBase, 2, 120);
	//DDX_Control(pDX, IDC_EDIT_TEMPS_LIGNE_BASE, m_EditTempsLigneBase);
	DDX_Control(pDX, IDC_CHECK_MISE_SOUS_VIDE, m_CheckMiseSousVide);
	DDX_Check(pDX, IDC_CHECK_MISE_SOUS_VIDE, m_bMiseSousVide);
	DDX_Control(pDX, IDC_SPIN_TEMPS_LIGNE_BASE, m_SpinTempsLigneBase);	
	DDX_Control(pDX, IDC_EDIT_TEMPS_VIDE, m_EditTempsVide);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VIDE, m_nTempsVide);
	//DDV_MinMaxFloat(pDX, m_nTempsLigneBase, 5, 360);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VIDE, m_SpinTempsVide);

}

BOOL CDivers::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam,lParam);
}

BEGIN_MESSAGE_MAP(CDivers, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_CELLULE, &CDivers::OnCbnSelchangeComboCellule)
	ON_BN_CLICKED(IDC_CHECK_MISE_SOUS_VIDE, &CDivers::OnBnClickedCheckMiseSousVide)
	ON_BN_CLICKED(IDC_BUTTON_CELLULE, &CDivers::OnBnClickedButtonCellule)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDivers

BOOL CDivers::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	list_cellule=GetCellules();
	if (list_cellule.size()==0)
	{
		GetDlgItem (IDC_COMBO_CELLULE)->EnableWindow ( false ) ;
		m_IndexCellule=-1;
	}

	for(UINT i=0;i<list_cellule.size();i++)
	{
		CString StrNumero;
		//StrNumero.Format(_T("%d"),list_cellule[i].numero);
		StrNumero.Format(_T("%s"),list_cellule[i].numero.c_str());
		m_CBCellule.InsertString(-1,StrNumero);
	}


	//m_SpinTempsLigneBase.SetRange(2,120);
	m_SpinTempsLigneBase.SetRange(0,1000000);
	m_SpinTempsLigneBase.SetPos(15);
	m_SpinTempsLigneBase.SetInc(-1);
	m_SpinTempsLigneBase.SetFormat("%1.f");
	m_SpinTempsLigneBase.UpdateBuddy();


	//m_SpinTempsVide.SetRange(5,360);
	m_SpinTempsVide.SetRange(0,10000000);
	m_SpinTempsVide.SetPos(90);
	m_SpinTempsVide.SetInc(-1);
	m_SpinTempsVide.SetFormat("%1.f");
	m_SpinTempsVide.UpdateBuddy();

	EnableMiseSousVide(m_bMiseSousVide);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

void CDivers::Reinitialisation()
{
	m_IndexCellule = -1;
	m_fVolumeTotal = 0;
	m_fVolumeCalo = 0;
	m_nTempsLigneBase = 15;
	m_nTempsVide = 90;
	m_bMiseSousVide = FALSE;

	cellule_exp.numero = "";
	cellule_exp.volume_calo = 0;
	cellule_exp.volume_total = 0;

	Enregistrement_donnees_actuelles();
}

void CDivers::OnCbnSelchangeComboCellule()
{
	UpdateData(TRUE);
	cellule_exp=list_cellule[m_IndexCellule];

	m_fVolumeTotal=cellule_exp.volume_total;
	m_fVolumeCalo=cellule_exp.volume_calo;

	UpdateData(FALSE);
}


void CDivers::OnBnClickedCheckMiseSousVide()
{
	UpdateData(TRUE);
	EnableMiseSousVide(m_bMiseSousVide);
	UpdateData(FALSE);
}

void CDivers::EnableMiseSousVide(BOOL active)
{
	GetDlgItem (IDC_STATIC_MISE_SOUS_VIDE)->EnableWindow (active) ;
	GetDlgItem (IDC_STATIC_MISE_SOUS_VIDE2)->EnableWindow (active) ;
	GetDlgItem (IDC_EDIT_TEMPS_VIDE)->EnableWindow (active) ;
	GetDlgItem (IDC_SPIN_TEMPS_VIDE)->EnableWindow (active) ;
}



void CDivers::OnBnClickedButtonCellule()
{
	m_DialogueCellule.DoModal();

	m_CBCellule.ResetContent();
	list_cellule.clear();

	list_cellule=GetCellules();
	if (list_cellule.size()==0)
	{
		GetDlgItem (IDC_COMBO_CELLULE)->EnableWindow ( false ) ;
		m_IndexCellule=-1;
	}

	for(UINT i=0;i<list_cellule.size();i++)
	{
		CString StrNumero;
		//StrNumero.Format(_T("%d"),list_cellule[i].numero);
		StrNumero.Format(_T("%s"),list_cellule[i].numero.c_str());
		m_CBCellule.InsertString(-1,StrNumero);
	}

}


BOOL CDivers::OnApply()
{
	Enregistrement_donnees_actuelles();

	return CPropertyPage::OnApply();
}

void CDivers::OnCancel()
{
	// remettre le bon index de la cellule
	m_IndexCellule=-1;
	for(UINT i=0;i<list_cellule.size();i++)
	{
		if (list_cellule[i].numero == donnees_actuelles.cellule.numero)
		{
			m_IndexCellule = i;
			break;
		}
	}
	// Soit on trouve l'index et on arrête la boucle
	// Soit on passe toute la boucle sans trouver la cellule et on met l'index à -1
	if (m_IndexCellule != -1)
	{
		cellule_exp=list_cellule[m_IndexCellule];

		m_fVolumeTotal=cellule_exp.volume_total;
		m_fVolumeCalo=cellule_exp.volume_calo;
	}
	else
	{
		m_fVolumeTotal=0;
		m_fVolumeCalo=0;
	}

	m_bMiseSousVide = donnees_actuelles.mise_sous_vide_fin_experience;

	m_nTempsLigneBase = donnees_actuelles.temps_ligne_base;
	m_SpinTempsLigneBase.SetPos(m_nTempsLigneBase);

	m_nTempsVide = donnees_actuelles.temps_vide;
	m_SpinTempsVide.SetPos(m_nTempsVide);

	CPropertyPage::OnCancel();
}

void CDivers::OnOK()
{
	Enregistrement_donnees_actuelles();

	CPropertyPage::OnOK();
}

void CDivers::Enregistrement_donnees_actuelles()
{
	donnees_actuelles.cellule = cellule_exp;
	donnees_actuelles.mise_sous_vide_fin_experience = m_bMiseSousVide;
	donnees_actuelles.temps_ligne_base = m_nTempsLigneBase;
	donnees_actuelles.temps_vide = m_nTempsVide;
}