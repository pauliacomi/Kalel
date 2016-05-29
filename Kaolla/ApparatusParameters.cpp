// Parametres_appareil.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "ApparatusParameters.h"

#include "Parametres.h"



// Boîte de dialogue ApparatusParameters

IMPLEMENT_DYNAMIC(ApparatusParameters, CDialog)

ApparatusParameters::ApparatusParameters(CWnd* pParent /*=NULL*/)
	: CDialog(ApparatusParameters::IDD, pParent)
	, Fichier_parametres(_T("./Parametres.ini"))
	, m_StrNomCalo(_T(""))
	, m_StrEnteteFichier(_T(""))

{
	m_fSensibiliteCalo = GetSensibiliteCalo();
	m_fSensibiliteCapteurBassePression = GetSensibiliteCapteurBassePression();
	m_fSensibiliteCapteurHautePression = GetSensibiliteCapteurHautePression();
	m_bSecurite = GetActivationSecurite();
	
	m_fPressionSecuriteBassePression = GetPressionSecuriteBassePression();
	m_fPressionSecuriteHautePression = GetPressionSecuriteHautePression();
	m_fPressionLimiteVide = GetPressionLimiteVide();

	m_bTuyere = GetPresenceTuyereSonique();

	m_fVolumeRef = GetVolumeRef();
	m_fVolumeP6 = GetVolumeP6();
}

ApparatusParameters::~ApparatusParameters()
{
}

BOOL ApparatusParameters::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CDialog::OnCommand(wParam, lParam);
}

void ApparatusParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT_SENSIBILITE_CALO, m_fSensibiliteCalo);
	DDX_Control(pDX, IDC_SPIN_SENSIBILITE_CALO, m_SpinSensibiliteCalo);

	DDX_Text(pDX, IDC_EDIT_SENSIBILITE_CAPTEUR_BASSE_PRESSION, m_fSensibiliteCapteurBassePression);
	DDX_Control(pDX, IDC_SPIN_SENSIBILITE_CAPTEUR_BASSE_PRESSION, m_SpinSensibiliteCapteurBassePression);

	DDX_Text(pDX, IDC_EDIT_SENSIBILITE_CAPTEUR_HAUTE_PRESSION, m_fSensibiliteCapteurHautePression);
	DDX_Control(pDX, IDC_SPIN_SENSIBILITE_CAPTEUR_HAUTE_PRESSION, m_SpinSensibiliteCapteurHautePression);

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

	m_SpinSensibiliteCalo.SetRange(0,100000);
	m_SpinSensibiliteCalo.SetPos(m_fSensibiliteCalo);
	m_SpinSensibiliteCalo.SetInc(-0.00000001);
	m_SpinSensibiliteCalo.SetFormat("%.8f");
	m_SpinSensibiliteCalo.UpdateBuddy();

	m_SpinSensibiliteCapteurBassePression.SetRange(0,100000);
	m_SpinSensibiliteCapteurBassePression.SetPos(m_fSensibiliteCapteurBassePression);
	m_SpinSensibiliteCapteurBassePression.SetInc(-0.01);
	m_SpinSensibiliteCapteurBassePression.SetFormat("%1.2f");
	m_SpinSensibiliteCapteurBassePression.UpdateBuddy();

	m_SpinSensibiliteCapteurHautePression.SetRange(0,1000000);
	m_SpinSensibiliteCapteurHautePression.SetPos(m_fSensibiliteCapteurHautePression);
	m_SpinSensibiliteCapteurHautePression.SetInc(-0.01);
	m_SpinSensibiliteCapteurHautePression.SetFormat("%1.2f");
	m_SpinSensibiliteCapteurHautePression.UpdateBuddy();

	m_SpinPressionSecuriteBassePression.SetRange(0,100000);
	m_SpinPressionSecuriteBassePression.SetPos(m_fPressionSecuriteBassePression);
	m_SpinPressionSecuriteBassePression.SetInc(-0.0001);
	m_SpinPressionSecuriteBassePression.SetFormat("%1.4f");
	m_SpinPressionSecuriteBassePression.UpdateBuddy();

	m_SpinPressionSecuriteHautePression.SetRange(0,1000000);
	m_SpinPressionSecuriteHautePression.SetPos(m_fPressionSecuriteHautePression);
	m_SpinPressionSecuriteHautePression.SetInc(-0.0001);
	m_SpinPressionSecuriteHautePression.SetFormat("%1.4f");
	m_SpinPressionSecuriteHautePression.UpdateBuddy();

	m_SpinPressionLimiteVide.SetRange(0,100000);
	m_SpinPressionLimiteVide.SetPos(m_fPressionLimiteVide);
	m_SpinPressionLimiteVide.SetInc(-0.001);
	m_SpinPressionLimiteVide.SetFormat("%.6f");
	m_SpinPressionLimiteVide.UpdateBuddy();

	m_SpinVolumeRef.SetRange(0,10000000);
	m_SpinVolumeRef.SetPos(m_fVolumeRef);
	m_SpinVolumeRef.SetInc(-0.01);
	m_SpinVolumeRef.SetFormat("%1.2f");
	m_SpinVolumeRef.UpdateBuddy();

	m_SpinVolumeP6.SetRange(0,100000000);
	m_SpinVolumeP6.SetPos(m_fVolumeP6);
	m_SpinVolumeP6.SetInc(-0.01);
	m_SpinVolumeP6.SetFormat("%1.2f");
	m_SpinVolumeP6.UpdateBuddy();

	m_StrNomCalo.Format(_T("%s"),GetNomCalo().c_str());
	m_StrEnteteFichier.Format(_T("%s"),GetEnteteCalo().c_str());

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
}



BEGIN_MESSAGE_MAP(ApparatusParameters, CDialog)
	ON_BN_CLICKED(IDOK, &ApparatusParameters::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ApparatusParameters::OnBnClickedCancel)
END_MESSAGE_MAP()


void ApparatusParameters::OnBnClickedOk()
{
	UpdateData(TRUE);

	SetSensibiliteCalo(m_fSensibiliteCalo);
	SetSensibiliteCapteurBassePression(m_fSensibiliteCapteurBassePression);
	SetSensibiliteCapteurHautePression(m_fSensibiliteCapteurHautePression);
	SetActivationSecurite(m_bSecurite);
	SetPressionSecuriteBassePression(m_fPressionSecuriteBassePression);
	SetPressionSecuriteHautePression(m_fPressionSecuriteHautePression);
	SetPressionLimiteVide(m_fPressionLimiteVide);
	SetPresenceTuyereSonique(m_bTuyere);
	SetVolumeRef(m_fVolumeRef);
	SetVolumeP6(m_fVolumeP6);
	SetNomCalo(m_StrNomCalo.GetBuffer());		
	SetEnteteCalo(m_StrEnteteFichier.GetBuffer());

	OnOK();
}

void ApparatusParameters::OnBnClickedCancel()
{
	OnCancel();
}
