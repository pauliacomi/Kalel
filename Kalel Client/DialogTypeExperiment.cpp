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
	DDX_Radio(pDX, IDC_RADIO_TYPE_EXPERIENCE_MANUAL, m_bExperience);
}


BEGIN_MESSAGE_MAP(DialogTypeExperiment, CDialog)
END_MESSAGE_MAP()


// Message management of DialogTypeExperiment

BOOL DialogTypeExperiment::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	// Disable the buttons if there is no mention of available instruments in the parameters file
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_MANUAL)->EnableWindow(instruments_exist);
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_AUTOMATIC)->EnableWindow(instruments_exist);
	GetDlgItem(IDC_RADIO_TYPE_EXPERIENCE_AUTO_CONT)->EnableWindow(instruments_exist);
	GetDlgItem(IDOK)->EnableWindow(instruments_exist);

	return TRUE;  // return TRUE unless you set the focus to a control
}

void DialogTypeExperiment::OnOK()
{
	UpdateData(TRUE);
	switch (m_bExperience)
	{
	case 0:
		TypeExperience = EXPERIMENT_TYPE_MANUAL;
		break;
	case 1:
		TypeExperience = EXPERIMENT_TYPE_AUTO;
		break;
	case 2:
		TypeExperience = EXPERIMENT_TYPE_CONTINUOUS;
		break;
	default:
		break;
	}

	CDialog::OnOK();
}

void DialogTypeExperiment::OnCancel()
{
	TypeExperience = EXPERIMENT_TYPE_UNDEF;

	CDialog::OnCancel();
}
