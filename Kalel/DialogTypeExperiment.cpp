// Dialogue_TypeExperience.cpp : implementation
//

#include "stdafx.h"
#include "DialogTypeExperiment.h"

#include "../Kalel Shared/Com Classes/MachineSettings.h"		// Accessing the settings
#include "../Kalel Shared/Resources/DefineStages.h"				// Experiment types
#include "../Kalel Shared/Resources/DefineInstruments.h"		// Instrument types

// Dialog box DialogTypeExperiment

IMPLEMENT_DYNAMIC(DialogTypeExperiment, CDialog)

// Constructor and destructor
DialogTypeExperiment::DialogTypeExperiment(CWnd* pParent /*=NULL*/)
	: CDialog(DialogTypeExperiment::IDD, pParent)
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;
}

DialogTypeExperiment::DialogTypeExperiment(const MachineSettings & ms, CWnd* pParent /*=NULL*/)
	: CDialog(DialogTypeExperiment::IDD, pParent)
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;

	auto exists = [&ms](int type) {
		bool is_here = false;
		for (auto pair : ms.readers) {
			if (pair.second.type == type) return true;
		}
		return false;
	};

	if (!exists(READER_CALO) && !exists(READER_PRESSURE))
	{
		instruments_exist = false;
	}
}


DialogTypeExperiment::~DialogTypeExperiment()
{
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
	if(!instruments_exist)
	{
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_AUTO)->EnableWindow(FALSE);
		GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(FALSE);
		GetDlgItem(IDOK)->EnableWindow(FALSE);
		return TRUE;
	}

	// Enable the buttons
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_AUTO)->EnableWindow(TRUE);
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);	

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
