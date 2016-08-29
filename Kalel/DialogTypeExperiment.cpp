// Dialogue_TypeExperience.cpp : implementation
//
#include "stdafx.h"
#include "DialogTypeExperiment.h"

#include "../Kalel Shared/Com Classes/MachineSettings.h"		// Accessing the settings
#include "../Kalel Shared/Resources/DefineStages.h"				// Experiment types

// Dialog box DialogTypeExperiment

IMPLEMENT_DYNAMIC(DialogTypeExperiment, CDialog)

// Constructor and destructor
DialogTypeExperiment::DialogTypeExperiment(CWnd* pParent /*=NULL*/)
	: CDialog(DialogTypeExperiment::IDD, pParent)
	, m_bExperienceAuto(FALSE)
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;
}

DialogTypeExperiment::~DialogTypeExperiment()
{
}

void DialogTypeExperiment::PassSettings(const MachineSettings & machineSettings)
{
	 * settings = machineSettings;
}

// Data exchange and message map
void DialogTypeExperiment::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TYPE_EXPERIENCE_AUTO, m_bExperienceAuto);
}


BEGIN_MESSAGE_MAP(DialogTypeExperiment, CDialog)
END_MESSAGE_MAP()


// Message management of DialogTypeExperiment

BOOL DialogTypeExperiment::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Disable the buttons if there is no mention of available instruments in the parameters file
	if(!settings->CaloToMeasure && !settings->LowPressureToMeasure && !settings->HighPressureToMeasure)
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
	if(!settings->HighPressureToMeasure)
	{
		m_bExperienceAuto = FALSE;
		UpdateData(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(FALSE);
		return TRUE;
	}
	
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void DialogTypeExperiment::OnOK()
{
	UpdateData(TRUE);
	if (m_bExperienceAuto == FALSE)
		TypeExperience = EXPERIMENT_TYPE_MANUAL;
	else
		TypeExperience = EXPERIMENT_TYPE_AUTO;

	CDialog::OnOK();
}

void DialogTypeExperiment::OnCancel()
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;

	CDialog::OnCancel();
}
