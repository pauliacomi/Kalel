// Dialogue_TypeExperience.cpp : implementation
//

#include "stdafx.h"
#include "Kalel.h"
#include "Dialog_TypeExperiment.h"


#include "Parametres.h"			// Accexsing the parameters file 
#include "DefineStages.h"		// Experiment types

// Dialog box Dialog_TypeExperiment

IMPLEMENT_DYNAMIC(Dialog_TypeExperiment, CDialog)

// Constructor and destructor
Dialog_TypeExperiment::Dialog_TypeExperiment(CWnd* pParent /*=NULL*/)
	: CDialog(Dialog_TypeExperiment::IDD, pParent)
	, m_bExperienceAuto(FALSE)
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;
}

Dialog_TypeExperiment::~Dialog_TypeExperiment()
{
}

// Data exchange and message map
void Dialog_TypeExperiment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TYPE_EXPERIENCE_AUTO, m_bExperienceAuto);
}


BEGIN_MESSAGE_MAP(Dialog_TypeExperiment, CDialog)
END_MESSAGE_MAP()


// Message management of Dialog_TypeExperiment

BOOL Dialog_TypeExperiment::OnInitDialog()
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

	// Disable the button for automatic experiment if there is no mention of a high range pressure sensor in the parameters file
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

void Dialog_TypeExperiment::OnOK()
{
	UpdateData(TRUE);
	if (m_bExperienceAuto == FALSE)
		TypeExperience = EXPERIMENT_TYPE_MANUAL;
	else
		TypeExperience = EXPERIMENT_TYPE_AUTO;

	CDialog::OnOK();
}

void Dialog_TypeExperiment::OnCancel()
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;

	CDialog::OnCancel();
}
