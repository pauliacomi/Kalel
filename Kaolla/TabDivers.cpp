// TabDivers.cpp : implementation file
//

#include "stdafx.h"
#include "Kaolla.h"
#include "TabDivers.h"
#include "afxdialogex.h"

#include "Dialogue_cellule.h"				// For the cell dialog
#include "Parametres.h"						// For a few commonly used functions
#include "ParserXML.h"						// XML builder


IMPLEMENT_DYNAMIC(TabDivers, CPropertyPage)

TabDivers::TabDivers()
	: CPropertyPage(TabDivers::IDD)
{
	Reinitialisation();
}

TabDivers::~TabDivers()
{
}

void TabDivers::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO_CELLULE, m_CBCellule);
	DDX_CBIndex(pDX, IDC_COMBO_CELLULE, m_IndexCellule);
	DDX_Control(pDX, IDC_EDIT_VOLUME_TOTAL, m_EditVolumeTotal);
	DDX_Text(pDX, IDC_EDIT_VOLUME_TOTAL, m_fVolumeTotal);
	DDX_Control(pDX, IDC_EDIT_VOLUME_CALO, m_EditVolumeCalo);
	DDX_Text(pDX, IDC_EDIT_VOLUME_CALO, m_fVolumeCalo);
	DDX_Text(pDX, IDC_EDIT_TEMPS_LIGNE_BASE, m_nTempsLigneBase);
	DDX_Control(pDX, IDC_CHECK_MISE_SOUS_VIDE, m_CheckMiseSousVide);
	DDX_Check(pDX, IDC_CHECK_MISE_SOUS_VIDE, m_bMiseSousVide);
	DDX_Control(pDX, IDC_EDIT_TEMPS_VIDE, m_EditTempsVide);
	DDX_Text(pDX, IDC_EDIT_TEMPS_VIDE, m_nTempsVide);

	DDX_Control(pDX, IDC_SPIN_TEMPS_LIGNE_BASE, m_SpinTempsLigneBase);
	DDX_Control(pDX, IDC_SPIN_TEMPS_VIDE, m_SpinTempsVide);
}

BOOL TabDivers::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	cellList = GetCellules();
	if (cellList.size() == 0)
	{
		GetDlgItem(IDC_COMBO_CELLULE)->EnableWindow(false);
		m_IndexCellule = -1;
	}

	for (UINT i = 0; i<cellList.size(); i++)
	{
		CString StrNumero;
		StrNumero.Format(_T("%s"), cellList[i].numero.c_str());
		m_CBCellule.InsertString(-1, StrNumero);
	}

	m_SpinTempsLigneBase.SetRange(0, 1000000);
	m_SpinTempsLigneBase.SetPos(15);
	m_SpinTempsLigneBase.SetInc(-1);
	m_SpinTempsLigneBase.SetFormat("%1.f");
	m_SpinTempsLigneBase.UpdateBuddy();

	m_SpinTempsVide.SetRange(0, 10000000);
	m_SpinTempsVide.SetPos(90);
	m_SpinTempsVide.SetInc(-1);
	m_SpinTempsVide.SetFormat("%1.f");
	m_SpinTempsVide.UpdateBuddy();

	EnableMiseSousVide(m_bMiseSousVide);

	return TRUE;  // return TRUE unless you set the focus to a control
}

BOOL TabDivers::OnCommand(WPARAM wParam, LPARAM lParam)
{
	SetModified(TRUE);
	return CPropertyPage::OnCommand(wParam, lParam);
}

BOOL TabDivers::OnApply()
{
	WriteData();
	return CPropertyPage::OnApply();
}

void TabDivers::OnCancel()
{
	// remettre le bon index de la cellule
	m_IndexCellule = -1;
	for (UINT i = 0; i<cellList.size(); i++)
	{
		if (cellList[i].numero == allSettings.cellule.numero)
		{
			m_IndexCellule = i;
			break;
		}
	}
	// Soit on trouve l'index et on arrête la boucle
	// Soit on passe toute la boucle sans trouver la cellule et on met l'index à -1
	if (m_IndexCellule != -1)
	{
		cellExp = cellList[m_IndexCellule];

		m_fVolumeTotal = cellExp.volume_total;
		m_fVolumeCalo = cellExp.volume_calo;
	}
	else
	{
		m_fVolumeTotal = 0;
		m_fVolumeCalo = 0;
	}

	m_bMiseSousVide = allSettings.mise_sous_vide_fin_experience;

	m_nTempsLigneBase = allSettings.temps_ligne_base;
	m_SpinTempsLigneBase.SetPos(m_nTempsLigneBase);

	m_nTempsVide = allSettings.temps_vide;
	m_SpinTempsVide.SetPos(m_nTempsVide);

	CPropertyPage::OnCancel();
}

void TabDivers::OnOK()
{
	WriteData();
	CPropertyPage::OnOK();
}

void TabDivers::Reinitialisation()
{
	m_IndexCellule = -1;
	m_fVolumeTotal = 0;
	m_fVolumeCalo = 0;
	m_nTempsLigneBase = 15;
	m_nTempsVide = 90;
	m_bMiseSousVide = FALSE;

	cellExp.numero = "";
	cellExp.volume_calo = 0;
	cellExp.volume_total = 0;

	WriteData();
}

void TabDivers::WriteData()
{
	allSettings.cellule = cellExp;
	allSettings.mise_sous_vide_fin_experience = m_bMiseSousVide;
	allSettings.temps_ligne_base = m_nTempsLigneBase;
	allSettings.temps_vide = m_nTempsVide;
}

void TabDivers::EnableMiseSousVide(BOOL active)
{
	GetDlgItem(IDC_STATIC_MISE_SOUS_VIDE)->EnableWindow(active);
	GetDlgItem(IDC_STATIC_MISE_SOUS_VIDE2)->EnableWindow(active);
	GetDlgItem(IDC_EDIT_TEMPS_VIDE)->EnableWindow(active);
	GetDlgItem(IDC_SPIN_TEMPS_VIDE)->EnableWindow(active);
}


BEGIN_MESSAGE_MAP(TabDivers, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_CELLULE, &TabDivers::OnCbnSelchangeComboCellule)
	ON_BN_CLICKED(IDC_CHECK_MISE_SOUS_VIDE, &TabDivers::OnBnClickedCheckMiseSousVide)
	ON_BN_CLICKED(IDC_BUTTON_CELLULE, &TabDivers::OnBnClickedButtonCellule)
END_MESSAGE_MAP()


// TabDivers message handlers


void TabDivers::OnCbnSelchangeComboCellule()
{
	UpdateData(TRUE);
	cellExp = cellList[m_IndexCellule];

	m_fVolumeTotal = cellExp.volume_total;
	m_fVolumeCalo = cellExp.volume_calo;

	UpdateData(FALSE);
}


void TabDivers::OnBnClickedCheckMiseSousVide()
{
	UpdateData(TRUE);
	EnableMiseSousVide(m_bMiseSousVide);
	UpdateData(FALSE);
}


void TabDivers::OnBnClickedButtonCellule()
{
	CDialogueCellule cellDialog("");
	cellDialog.DoModal();

	m_CBCellule.ResetContent();
	cellList.clear();

	cellList = GetCellules();
	if (cellList.size() == 0)
	{
		GetDlgItem(IDC_COMBO_CELLULE)->EnableWindow(false);
		m_IndexCellule = -1;
	}

	for (UINT i = 0; i<cellList.size(); i++)
	{
		CString StrNumero;
		StrNumero.Format(_T("%s"), cellList[i].numero.c_str());
		m_CBCellule.InsertString(-1, StrNumero);
	}

}