// Connection_port.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kalel.h"
#include "Connection_port.h"

// Boîte de dialogue CConnection_port

IMPLEMENT_DYNAMIC(CConnection_port, CDialog)

CConnection_port::CConnection_port(CWnd* pParent /*=NULL*/)
	: CDialog(CConnection_port::IDD, pParent)
	, m_nIndexTypeInstrument1(INDEX_AUCUN)
	, m_nIndexTypeInstrument2(INDEX_AUCUN)
	, m_nIndexTypeInstrument3(INDEX_AUCUN)
	, m_nPortInstrument1(0)
	, m_nPortInstrument2(0)
	, m_nPortInstrument3(0)
	, m_bInstrument1KeithleyVoie1(FALSE)
	, m_bInstrument1KeithleyVoie2(FALSE)
	, m_bInstrument2KeithleyVoie1(FALSE)
	, m_bInstrument2KeithleyVoie2(FALSE)
	, m_bInstrument3KeithleyVoie1(FALSE)
	, m_bInstrument3KeithleyVoie2(FALSE)
	, m_nInstrument1Mensor(0)
	, m_nInstrument2Mensor(0)
	, m_nInstrument3Mensor(0)
	, m_nIndexPortVannes(0)
	, m_nIndexPortTemperatures(0)
{
}

CConnection_port::~CConnection_port()
{
}

void CConnection_port::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT1, m_CBTypeInstrument1);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT2, m_CBTypeInstrument2);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT3, m_CBTypeInstrument3);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT1, m_nIndexTypeInstrument1);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT2, m_nIndexTypeInstrument2);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT3, m_nIndexTypeInstrument3);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT1, m_CBPortInstrument1);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT2, m_CBPortInstrument2);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT3, m_CBPortInstrument3);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT1, m_nPortInstrument1);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT2, m_nPortInstrument2);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT3, m_nPortInstrument3);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, m_CheckInstrument1KeithleyVoie1);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, m_CheckInstrument1KeithleyVoie2);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE1, m_CheckInstrument2KeithleyVoie1);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, m_CheckInstrument2KeithleyVoie2);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE1, m_CheckInstrument3KeithleyVoie1);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, m_CheckInstrument3KeithleyVoie2);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, m_bInstrument1KeithleyVoie1);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, m_bInstrument1KeithleyVoie2);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE1, m_bInstrument2KeithleyVoie1);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, m_bInstrument2KeithleyVoie2);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE1, m_bInstrument3KeithleyVoie1);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, m_bInstrument3KeithleyVoie2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, m_CBInstrument1KeithleyVoie2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT2_KEITHLEY_VOIE2, m_CBInstrument2KeithleyVoie2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT3_KEITHLEY_VOIE2, m_CBInstrument3KeithleyVoie2);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, m_nIndexInstrument1KeithleyVoie2);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT2_KEITHLEY_VOIE2, m_nIndexInstrument2KeithleyVoie2);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT3_KEITHLEY_VOIE2, m_nIndexInstrument3KeithleyVoie2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT1_MENSOR, m_CBInstrument1Mensor);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT2_MENSOR, m_CBInstrument2Mensor);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT3_MENSOR, m_CBInstrument3Mensor);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT1_MENSOR, m_nInstrument1Mensor);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT2_MENSOR, m_nInstrument2Mensor);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT3_MENSOR, m_nInstrument3Mensor);
	DDX_Control(pDX, IDC_PORT_VANNES, m_CBPortVannes);
	DDX_CBIndex(pDX, IDC_PORT_VANNES, m_nIndexPortVannes);
	DDX_Control(pDX, IDC_PORT_TEMPERATURES, m_CBPortTemperatures);
	DDX_CBIndex(pDX, IDC_PORT_TEMPERATURES, m_nIndexPortTemperatures);
}

BOOL CConnection_port::OnCommand(WPARAM wParam, LPARAM lParam)
{
	return CDialog::OnCommand(wParam, lParam);
}

BEGIN_MESSAGE_MAP(CConnection_port, CDialog)
	ON_BN_CLICKED(IDOK, &CConnection_port::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CConnection_port::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INSTRUMENT1, &CConnection_port::OnCbnSelchangeComboTypeInstrument1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INSTRUMENT2, &CConnection_port::OnCbnSelchangeComboTypeInstrument2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INSTRUMENT3, &CConnection_port::OnCbnSelchangeComboTypeInstrument3)
	ON_BN_CLICKED(IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, &CConnection_port::OnBnClickedCheckInstrument1KeithleyVoie2)
	ON_BN_CLICKED(IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, &CConnection_port::OnBnClickedCheckInstrument2KeithleyVoie2)
	ON_BN_CLICKED(IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, &CConnection_port::OnBnClickedCheckInstrument3KeithleyVoie2)
END_MESSAGE_MAP()


// Gestionnaires de messages de CConnection_port

BOOL CConnection_port::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitConnection_port();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}



void CConnection_port::OnBnClickedOk()
{
	UpdateData(TRUE);

	CString StrMessageBox;

	Verifications();

	if (!bPbm && !bWarning)
	{
		EnregistrementConnection_port();
		EnregistrementVerifications();
		OnOK();
		return;
	}
	if(bPbm)
	{
		if(bPbmCalo)
			StrMessageErreur += StrPbmCalo + _T("\t\n");
		if(bPbmHP)
			StrMessageErreur += StrPbmBP + _T("\t\n");
		if(bPbmBP)
			StrMessageErreur += StrPbmHP + _T("\t\n");

		StrMessageBox = StrMessageErreur;
		if(bWarning)
		{
			StrMessageBox += _T("\n-------------------------------\t\n");
			StrMessageBox += StrMessageWarning;
		}
		AfxMessageBox(StrMessageBox,MB_OK|MB_ICONERROR);
		return;
	}


	StrMessageWarning += _T("\nVoulez-vous tout de même continuer ?\t\n");
	if(AfxMessageBox(StrMessageWarning,MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
	{
		EnregistrementConnection_port();
		EnregistrementVerifications();
		OnOK();
	}
}

void CConnection_port::OnBnClickedCancel()
{
	InitConnection_port();
	OnCancel();
}



void CConnection_port::OnCbnSelchangeComboTypeInstrument1()
{
	UpdateData(TRUE);
	ShowItemInstrument1();
	UpdateData(FALSE);
}

void CConnection_port::OnCbnSelchangeComboTypeInstrument2()
{
	UpdateData(TRUE);
	ShowItemInstrument2();
	UpdateData(FALSE);
}

void CConnection_port::OnCbnSelchangeComboTypeInstrument3()
{
	UpdateData(TRUE);
	ShowItemInstrument3();
	UpdateData(FALSE);
}

void CConnection_port::OnBnClickedCheckInstrument1KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrument1KeithleyVoie2.EnableWindow (m_bInstrument1KeithleyVoie2);
	UpdateData(FALSE);
}

void CConnection_port::OnBnClickedCheckInstrument2KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrument2KeithleyVoie2.EnableWindow (m_bInstrument2KeithleyVoie2);
	UpdateData(FALSE);
}

void CConnection_port::OnBnClickedCheckInstrument3KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrument3KeithleyVoie2.EnableWindow (m_bInstrument3KeithleyVoie2);
	UpdateData(FALSE);
}

//-----------------------


void CConnection_port::EnregistrementConnection_port()
{
	EnregistrementParametresInstrument1();
	EnregistrementParametresInstrument2();
	EnregistrementParametresInstrument3();

	SetPortVannes(m_nIndexPortVannes + 1);
	SetPortTemperatures(m_nIndexPortTemperatures + 1);
}


void CConnection_port::InitConnection_port()
{
	InitDonneesInstrument1();
	InitDonneesInstrument2();
	InitDonneesInstrument3();

	m_nIndexPortVannes = GetPortVannes() - 1;
	m_nIndexPortTemperatures = GetPortTemperatures() - 1;

	UpdateData(FALSE);

	ShowItemInstrument1();
	ShowItemInstrument2();
	ShowItemInstrument3();


}



