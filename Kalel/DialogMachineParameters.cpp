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
	, m_StrNomCalo(_T(""))
	, m_StrEnteteFichier(_T(""))
	, modified {false}
{	
}


void ApparatusParameters::PassSettings(const MachineSettings  &machineSettings)
{
	localSettings = std::make_unique<MachineSettings>(machineSettings);

	m_bSecurite								= localSettings->SafetyOn;

	m_fPressionLimiteVide					= localSettings->PressureLimitVacuum;

	m_bTuyere								= localSettings->hasSonicNozzle;

	m_fVolumeRef							= localSettings->VolumeRef;
	m_fVolumeP6								= localSettings->VolumeP6;

	m_StrNomCalo.Format(_T("%s"), localSettings->CaloName.c_str());
	m_StrEnteteFichier.Format(_T("%s"), localSettings->CaloPrefix.c_str());
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

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_BASSE_PRESSION, m_fPressionSecuriteBassePression);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_BASSE_PRESSION, m_SpinPressionSecuriteBassePression);

	DDX_Text(pDX, IDC_EDIT_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION, m_fPressionSecuriteHautePression);
	DDX_Control(pDX, IDC_SPIN_PRESSION_SECURITE_CAPTEUR_HAUTE_PRESSION, m_SpinPressionSecuriteHautePression);

	DDX_Text(pDX, IDC_EDIT_PRESSION_LIMITE_VIDE, m_fPressionLimiteVide);
	DDX_Control(pDX, IDC_SPIN_PRESSION_LIMITE_VIDE, m_SpinPressionLimiteVide);

	DDX_Text(pDX, IDC_EDIT_VOLUME_REF, m_fVolumeRef);
	DDX_Control(pDX, IDC_SPIN_VOLUME_REF, m_SpinVolumeRef);

	DDX_Text(pDX, IDC_EDIT_VOLUME_P6, m_fVolumeP6);
	DDX_Control(pDX, IDC_SPIN_VOLUME_P6, m_SpinVolumeP6);

	DDX_Text(pDX, IDC_NOM_CALO, m_StrNomCalo);
	DDX_Text(pDX, IDC_ENTETE_FICHIER, m_StrEnteteFichier);

	DDX_Check(pDX, IDC_CHECK_ACTIVATION_SECURITE, m_bSecurite);
	DDX_Check(pDX, IDC_CHECK_TUYERE_SONIQUE, m_bTuyere);
}


BOOL ApparatusParameters::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
{
	return CDialog::Create(lpszTemplateName, pParentWnd);
}

// Gestionnaires de messages de ApparatusParameters

BOOL ApparatusParameters::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_SpinPressionSecuriteBassePression.SetRange(0,100000);
	m_SpinPressionSecuriteBassePression.SetPos(m_fPressionSecuriteBassePression);
	m_SpinPressionSecuriteBassePression.SetInc(-0.0001);
	m_SpinPressionSecuriteBassePression.SetFormat(_T("%1.4f"));
	m_SpinPressionSecuriteBassePression.UpdateBuddy();

	m_SpinPressionSecuriteHautePression.SetRange(0,1000000);
	m_SpinPressionSecuriteHautePression.SetPos(m_fPressionSecuriteHautePression);
	m_SpinPressionSecuriteHautePression.SetInc(-0.0001);
	m_SpinPressionSecuriteHautePression.SetFormat(_T("%1.4f"));
	m_SpinPressionSecuriteHautePression.UpdateBuddy();

	m_SpinPressionLimiteVide.SetRange(0,100000);
	m_SpinPressionLimiteVide.SetPos(m_fPressionLimiteVide);
	m_SpinPressionLimiteVide.SetInc(-0.001);
	m_SpinPressionLimiteVide.SetFormat(_T("%.6f"));
	m_SpinPressionLimiteVide.UpdateBuddy();

	m_SpinVolumeRef.SetRange(0,10000000);
	m_SpinVolumeRef.SetPos(m_fVolumeRef);
	m_SpinVolumeRef.SetInc(-0.01);
	m_SpinVolumeRef.SetFormat(_T("%1.2f"));
	m_SpinVolumeRef.UpdateBuddy();

	m_SpinVolumeP6.SetRange(0,100000000);
	m_SpinVolumeP6.SetPos(m_fVolumeP6);
	m_SpinVolumeP6.SetInc(-0.01);
	m_SpinVolumeP6.SetFormat(_T("%1.2f"));
	m_SpinVolumeP6.UpdateBuddy();

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

		localSettings->SafetyOn = m_bSecurite;
		localSettings->PressureLimitVacuum = m_fPressionLimiteVide;
		localSettings->hasSonicNozzle = m_bTuyere;
		localSettings->VolumeRef = m_fVolumeRef;
		localSettings->VolumeP6 = m_fVolumeP6;
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