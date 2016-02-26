// Dialogue_TypeExperience.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "Dialogue_TypeExperience.h"

////remove once done
#include "Parametres.h"
////


// Boîte de dialogue CDialogue_TypeExperience

IMPLEMENT_DYNAMIC(CDialogue_TypeExperience, CDialog)

CDialogue_TypeExperience::CDialogue_TypeExperience(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue_TypeExperience::IDD, pParent)
	, m_bExperienceAuto(FALSE)
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;
}

CDialogue_TypeExperience::~CDialogue_TypeExperience()
{
}

void CDialogue_TypeExperience::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TYPE_EXPERIENCE, m_bExperienceAuto);
}


BEGIN_MESSAGE_MAP(CDialogue_TypeExperience, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDialogue_TypeExperience

BOOL CDialogue_TypeExperience::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	if(!GetMesureCalo() && !GetMesureBassePression() && !GetMesureHautePression())
	{
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return TRUE;
	}

	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	if(!GetMesureHautePression())
	{
		m_bExperienceAuto = FALSE;
		UpdateData(FALSE);
		GetDlgItem(IDC_RADIO2)->EnableWindow(FALSE);
		return TRUE;
	}
	
	GetDlgItem(IDC_RADIO2)->EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

void CDialogue_TypeExperience::OnOK()
{
	UpdateData(TRUE);
	if (m_bExperienceAuto == FALSE)
		TypeExperience = EXPERIMENT_TYPE_MANUAL;
	else
		TypeExperience = EXPERIMENT_TYPE_AUTO;

	CDialog::OnOK();
}

void CDialogue_TypeExperience::OnCancel()
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;

	CDialog::OnCancel();
}


