// Dialogue_TypeExperience.cpp : implementation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "Dialogue_TypeExperience.h"


#include "Parametres.h"			// Accexsing the parameters file 
#include "DefineStages.h"		// Experiment types

// Dialog box CDialogue_TypeExperience

IMPLEMENT_DYNAMIC(CDialogue_TypeExperience, CDialog)

// Constructor and destructor
CDialogue_TypeExperience::CDialogue_TypeExperience(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogue_TypeExperience::IDD, pParent)
	, m_bExperienceAuto(FALSE)
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;
}

CDialogue_TypeExperience::~CDialogue_TypeExperience()
{
}

// Data exchange and message map
void CDialogue_TypeExperience::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TYPE_EXPERIENCE_AUTO, m_bExperienceAuto);
}


BEGIN_MESSAGE_MAP(CDialogue_TypeExperience, CDialog)
END_MESSAGE_MAP()


// Message management of CDialogue_TypeExperience

BOOL CDialogue_TypeExperience::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Disable the buttons if there is no mention of available instruments in the parameters file
	if(!GetMesureCalo() && !GetMesureBassePression() && !GetMesureHautePression())
	{
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_AUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return TRUE;
	}

	// Enable the buttons
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_AUTO)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);

	// I have no clue why this is duplicated ---------------------->
	if(!GetMesureHautePression())
	{
		m_bExperienceAuto = FALSE;
		UpdateData(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(FALSE);
		return TRUE;
	}
	
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
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
