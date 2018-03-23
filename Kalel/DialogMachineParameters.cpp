// Parametres_appareil.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "DialogMachineParameters.h"

#include "../Kalel Shared/Com Classes/MachineSettings.h"		// AcceSsing the settings
#include "../Kalel Shared/Resources/DefineStages.h"				// Experiment types

// Boîte de dialogue ApparatusParameters

IMPLEMENT_DYNAMIC(ApparatusParameters, CDialog)

ApparatusParameters::ApparatusParameters(CWnd* pParent /*=NULL*/)
	: CDialog(ApparatusParameters::IDD, pParent)
{	
}


void ApparatusParameters::PassSettings(const MachineSettings  &machineSettings)
{
	localSettings = std::make_unique<MachineSettings>(machineSettings);

	// instantiate things that cannot be passed because of mfc restrictions
	m_StrNomCalo							= localSettings->CaloName.c_str();
	m_StrEnteteFichier						= localSettings->CaloPrefix.c_str();

	b_safety								= localSettings->SafetyOn;
	b_nozzle								= localSettings->hasSonicNozzle;
}

ApparatusParameters::~ApparatusParameters()
{
}

bool ApparatusParameters::Changed()
{
	return modified;
}

MachineSettings *ApparatusParameters::GetSettings()
{
	return localSettings.release();
}

BOOL ApparatusParameters::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CDialog::OnCommand(wParam, lParam);
}


void ApparatusParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_NOM_CALO, m_StrNomCalo);
	DDX_Text(pDX, IDC_ENTETE_FICHIER, m_StrEnteteFichier);

	// Bools

	DDX_Check(pDX, IDC_CHECK_ACTIVATION_SECURITE, b_safety);
	DDX_Check(pDX, IDC_CHECK_TUYERE_SONIQUE, b_nozzle);

	// With controls
	// safety

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION, localSettings->PressurePumpVacuum);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION, m_SpinVacuumPump);

	DDX_Text(pDX, IDC_EDIT_PRESSION_LIMITE_VIDE, localSettings->PressureLimitVacuum);
	DDX_Control(pDX, IDC_SPIN_PRESSION_LIMITE_VIDE, m_SpinVacuumLimit);

	// Volume

	DDX_Text(pDX, IDC_EDIT_VOLUME_REF, localSettings->VolumeRef);
	DDX_Control(pDX, IDC_SPIN_VOLUME_REF, m_SpinVolumeRef);

	DDX_Text(pDX, IDC_EDIT_VOLUME_P6, localSettings->VolumeP6);
	DDX_Control(pDX, IDC_SPIN_VOLUME_P6, m_SpinVolumeP6);

	// Injection

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION2, localSettings->InjectionAttemptNumber);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION2, m_SpinInjection[0]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION3, localSettings->InjectionMargin);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION3, m_SpinInjection[1]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION4, localSettings->InjectionMultiplier);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION4, m_SpinInjection[2]);

	// Time

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION5, localSettings->TimeBetweenMeasurement);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION5, m_SpinTime[0]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION6, localSettings->TimeBetweenRecording);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION6, m_SpinTime[1]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION7, localSettings->TimeBetweenAutomation);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION7, m_SpinTime[2]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION8, localSettings->TimeWaitValves);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION8, m_SpinTime[3]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION9, localSettings->TimeWaitValvesShort);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION9, m_SpinTime[4]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION10, localSettings->TimeWaitPump);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION10, m_SpinTime[5]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION11, localSettings->TimeVacuumEmergency);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION11, m_SpinTime[6]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION12, localSettings->TimeVacuumBottle);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION12, m_SpinTime[7]);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION13, localSettings->TimeVacuumEndDefault);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION13, m_SpinTime[8]);
}


BOOL ApparatusParameters::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialog::Create(lpszTemplateName, pParentWnd);
}

// Gestionnaires de messages de ApparatusParameters

BOOL ApparatusParameters::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Vacuums

	m_SpinVacuumPump.SetRange(0,100000);
	m_SpinVacuumPump.SetInc(-0.0001);
	m_SpinVacuumPump.SetFormat(_T("%1.4f"));
	m_SpinVacuumPump.UpdateBuddy();

	m_SpinVacuumLimit.SetRange(0,1000000);
	m_SpinVacuumLimit.SetPos(localSettings->PressureLimitVacuum);
	m_SpinVacuumLimit.SetInc(-0.0001);
	m_SpinVacuumLimit.SetFormat(_T("%1.4f"));
	m_SpinVacuumLimit.UpdateBuddy();

	// Volume

	m_SpinVolumeRef.SetRange(0,10000000);
	m_SpinVolumeRef.SetPos(localSettings->VolumeRef);
	m_SpinVolumeRef.SetInc(-0.01);
	m_SpinVolumeRef.SetFormat(_T("%1.2f"));
	m_SpinVolumeRef.UpdateBuddy();

	m_SpinVolumeP6.SetRange(0,100000000);
	m_SpinVolumeP6.SetPos(localSettings->VolumeP6);
	m_SpinVolumeP6.SetInc(-0.01);
	m_SpinVolumeP6.SetFormat(_T("%1.2f"));
	m_SpinVolumeP6.UpdateBuddy();

	// Injections
	m_SpinInjection[0].SetPos(localSettings->InjectionAttemptNumber);
	m_SpinInjection[0].SetPos(localSettings->InjectionMargin);
	m_SpinInjection[0].SetPos(localSettings->InjectionMultiplier);
	for (size_t i = 0; i < 3; i++)
	{
		m_SpinInjection[i].SetRange(0, 100);
		m_SpinInjection[i].SetInc(-0.01);
		m_SpinInjection[i].SetFormat(_T("%1.2f"));
		m_SpinInjection[i].UpdateBuddy();
	}
	m_SpinInjection[0].SetFormat(_T("%1.f"));
	m_SpinInjection[0].SetInc(-1.0);

	//Time 
	m_SpinTime[0].SetPos(localSettings->TimeBetweenMeasurement);
	m_SpinTime[1].SetPos(localSettings->TimeBetweenRecording);
	m_SpinTime[2].SetPos(localSettings->TimeBetweenAutomation);
	m_SpinTime[3].SetPos(localSettings->TimeWaitValves);
	m_SpinTime[4].SetPos(localSettings->TimeWaitValvesShort);
	m_SpinTime[5].SetPos(localSettings->TimeWaitPump);
	m_SpinTime[6].SetPos(localSettings->TimeVacuumEmergency);
	m_SpinTime[7].SetPos(localSettings->TimeVacuumBottle);
	m_SpinTime[8].SetPos(localSettings->TimeVacuumEndDefault);
	for (size_t i = 0; i < 9; i++)
	{
		m_SpinTime[i].SetRange(0, 1000000);
		m_SpinTime[i].SetInc(-1);
		m_SpinTime[i].SetFormat(_T("%1.f"));
		m_SpinTime[i].UpdateBuddy();
	}

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}



BEGIN_MESSAGE_MAP(ApparatusParameters, CDialog)
	ON_BN_CLICKED(IDOK, &ApparatusParameters::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ApparatusParameters::OnBnClickedCancel)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_SENSIBILITE_CALO, IDC_CHECK_TUYERE_SONIQUE, &ApparatusParameters::OnModified)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_ACTIVATION_SECURITE, IDC_CHECK_TUYERE_SONIQUE, &ApparatusParameters::OnModified)
END_MESSAGE_MAP()


void ApparatusParameters::OnBnClickedOk()
{
	UpdateData(TRUE);
	
	if (modified) {

		localSettings->SafetyOn = b_safety;
		localSettings->hasSonicNozzle = b_nozzle;

		localSettings->CaloName = m_StrNomCalo.GetBuffer();
		localSettings->CaloPrefix = m_StrEnteteFichier.GetBuffer();			 
	}	

	OnOK();
}

void ApparatusParameters::OnBnClickedCancel()
{
	OnCancel();
}


void ApparatusParameters::OnModified(UINT nID)
{
	modified = true;
}