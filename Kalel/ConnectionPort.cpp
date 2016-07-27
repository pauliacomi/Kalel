// Connection_port.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kalel.h"
#include "ConnectionPort.h"

#include "Parametres.h"

// Boîte de dialogue ConnectionPort

IMPLEMENT_DYNAMIC(ConnectionPort, CDialog)

ConnectionPort::ConnectionPort(CWnd* pParent /*=NULL*/)
	: CDialog(ConnectionPort::IDD, pParent)

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

ConnectionPort::~ConnectionPort()
{
}

void ConnectionPort::DoDataExchange(CDataExchange* pDX)
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



////////////////////////////////////////////////////////////
//
// Message map + functions
//
////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(ConnectionPort, CDialog)
	ON_BN_CLICKED(IDOK, &ConnectionPort::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ConnectionPort::OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INSTRUMENT1, &ConnectionPort::OnCbnSelchangeComboTypeInstrument1)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INSTRUMENT2, &ConnectionPort::OnCbnSelchangeComboTypeInstrument2)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE_INSTRUMENT3, &ConnectionPort::OnCbnSelchangeComboTypeInstrument3)
	ON_BN_CLICKED(IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, &ConnectionPort::OnBnClickedCheckInstrument1KeithleyVoie2)
	ON_BN_CLICKED(IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, &ConnectionPort::OnBnClickedCheckInstrument2KeithleyVoie2)
	ON_BN_CLICKED(IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, &ConnectionPort::OnBnClickedCheckInstrument3KeithleyVoie2)
END_MESSAGE_MAP()


// Gestionnaires de messages de ConnectionPort

BOOL ConnectionPort::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitConnection_port();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


void ConnectionPort::OnBnClickedOk()
{
	UpdateData(TRUE);


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
		CString StrMessageBox;
		
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

	CString message;
	message.Format(CONTINUE_ANYWAY);
	StrMessageWarning += message;
	
	if(AfxMessageBox(StrMessageWarning,MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
	{
		EnregistrementConnection_port();
		EnregistrementVerifications();
		OnOK();
	}
}

void ConnectionPort::OnBnClickedCancel()
{
	InitConnection_port();
	OnCancel();
}


void ConnectionPort::OnCbnSelchangeComboTypeInstrument1()
{
	UpdateData(TRUE);
	
	// Show instrument 1
	ShowItem(m_nIndexTypeInstrument1,
		m_bInstrument1KeithleyVoie2,
		&m_CBPortInstrument1,
		&m_CheckInstrument1KeithleyVoie1,
		&m_CheckInstrument1KeithleyVoie2,
		&m_CBInstrument1KeithleyVoie2,
		&m_CBInstrument1Mensor);

	UpdateData(FALSE);
}

void ConnectionPort::OnCbnSelchangeComboTypeInstrument2()
{
	UpdateData(TRUE);

	// Show instrument 2
	ShowItem(m_nIndexTypeInstrument2,
		m_bInstrument2KeithleyVoie2,
		&m_CBPortInstrument2,
		&m_CheckInstrument2KeithleyVoie1,
		&m_CheckInstrument2KeithleyVoie2,
		&m_CBInstrument2KeithleyVoie2,
		&m_CBInstrument2Mensor);

	UpdateData(FALSE);
}

void ConnectionPort::OnCbnSelchangeComboTypeInstrument3()
{
	UpdateData(TRUE);

	// Show instrument 3
	ShowItem(m_nIndexTypeInstrument3,
		m_bInstrument3KeithleyVoie2,
		&m_CBPortInstrument3,
		&m_CheckInstrument3KeithleyVoie1,
		&m_CheckInstrument3KeithleyVoie2,
		&m_CBInstrument3KeithleyVoie2,
		&m_CBInstrument3Mensor);

	UpdateData(FALSE);
}

void ConnectionPort::OnBnClickedCheckInstrument1KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrument1KeithleyVoie2.EnableWindow (m_bInstrument1KeithleyVoie2);
	UpdateData(FALSE);
}

void ConnectionPort::OnBnClickedCheckInstrument2KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrument2KeithleyVoie2.EnableWindow (m_bInstrument2KeithleyVoie2);
	UpdateData(FALSE);
}

void ConnectionPort::OnBnClickedCheckInstrument3KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrument3KeithleyVoie2.EnableWindow (m_bInstrument3KeithleyVoie2);
	UpdateData(FALSE);
}



////////////////////////////////////////////////////////////
//
// Functions
//
////////////////////////////////////////////////////////////


void ConnectionPort::EnregistrementConnection_port()
{
	// Save instrument 1
	EnregistrementParametresInstrument(1,
		m_nIndexTypeInstrument1,
		m_nPortInstrument1 + 1,
		m_bInstrument1KeithleyVoie1,
		m_bInstrument1KeithleyVoie2,
		m_nIndexInstrument1KeithleyVoie2,
		m_nInstrument1Mensor);

	// Save instrument 2
	EnregistrementParametresInstrument(2,
		m_nIndexTypeInstrument2,
		m_nPortInstrument2 + 1,
		m_bInstrument2KeithleyVoie1,
		m_bInstrument2KeithleyVoie2,
		m_nIndexInstrument2KeithleyVoie2,
		m_nInstrument2Mensor);

	// Save instrument 3
	EnregistrementParametresInstrument(3,
		m_nIndexTypeInstrument3,
		m_nPortInstrument3 + 1,
		m_bInstrument3KeithleyVoie1,
		m_bInstrument3KeithleyVoie2,
		m_nIndexInstrument3KeithleyVoie2,
		m_nInstrument3Mensor);

	// Save valve port
	SetPortVannes(m_nIndexPortVannes + 1);

	// Save temperature port
	SetPortTemperatures(m_nIndexPortTemperatures + 1);
}


void ConnectionPort::InitConnection_port()
{

	// Initiate instrument 1
	InitDonneesInstrument(GetTypeInstrument1(),
		&m_nIndexTypeInstrument1,
		&m_nPortInstrument1,
		&m_bInstrument1KeithleyVoie1,
		&m_bInstrument1KeithleyVoie2,
		&m_CBInstrument1KeithleyVoie2,
		&m_nIndexInstrument1KeithleyVoie2,
		&m_nInstrument1Mensor,
		GetCOMInstrument1() - 1,
		GetFonctionInstrument1());
	
	// Initiate instrument 2
	InitDonneesInstrument(GetTypeInstrument2(),
		&m_nIndexTypeInstrument2,
		&m_nPortInstrument2,
		&m_bInstrument2KeithleyVoie1,
		&m_bInstrument2KeithleyVoie2,
		&m_CBInstrument2KeithleyVoie2,
		&m_nIndexInstrument2KeithleyVoie2,
		&m_nInstrument2Mensor,
		GetCOMInstrument2() - 1,
		GetFonctionInstrument2());

	// Initiate instrument 3
	InitDonneesInstrument(GetTypeInstrument3(),
		&m_nIndexTypeInstrument3,
		&m_nPortInstrument3,
		&m_bInstrument3KeithleyVoie1,
		&m_bInstrument3KeithleyVoie2,
		&m_CBInstrument3KeithleyVoie2,
		&m_nIndexInstrument3KeithleyVoie2,
		&m_nInstrument3Mensor,
		GetCOMInstrument3() - 1,
		GetFonctionInstrument3());

	// Get port valves
	m_nIndexPortVannes = GetPortVannes() - 1;

	// Port temperatures
	m_nIndexPortTemperatures = GetPortTemperatures() - 1;

	UpdateData(FALSE);

	// Show instrument 1
	ShowItem(m_nIndexTypeInstrument1,
		m_bInstrument1KeithleyVoie2,
		&m_CBPortInstrument1,
		&m_CheckInstrument1KeithleyVoie1,
		&m_CheckInstrument1KeithleyVoie2,
		&m_CBInstrument1KeithleyVoie2,
		&m_CBInstrument1Mensor);

	// Show instrument 2
	ShowItem(m_nIndexTypeInstrument2,
		m_bInstrument2KeithleyVoie2,
		&m_CBPortInstrument2,
		&m_CheckInstrument2KeithleyVoie1,
		&m_CheckInstrument2KeithleyVoie2,
		&m_CBInstrument2KeithleyVoie2,
		&m_CBInstrument2Mensor);

	// Show instrument 3
	ShowItem(m_nIndexTypeInstrument3,
		m_bInstrument3KeithleyVoie2,
		&m_CBPortInstrument3,
		&m_CheckInstrument3KeithleyVoie1,
		&m_CheckInstrument3KeithleyVoie2,
		&m_CBInstrument3KeithleyVoie2,
		&m_CBInstrument3Mensor);
}



void ConnectionPort::InitDonneesInstrument(int TypeInstr, int* m_nIndex, int* m_nPortInstrument, BOOL* m_bInstrumentKeithleyVoie1, BOOL* m_bInstrumentKeithleyVoie2, CComboBox* m_CBInstrumentKeithleyVoie2, int* m_nIndexInstrumentKeithleyVoie2, int* m_nInstrumentMensor, int GetPort, int GetFonction)
{
	switch (TypeInstr)
	{
	case INSTRUMENT_NONE:
	case INSTRUMENT_UNDEF:
	case INSTRUMENT_INEXIST:
		*m_nIndex = INDEX_AUCUN;
		break;

	case INSTRUMENT_KEITHLEY:
		*m_nIndex = INDEX_KEITHLEY;
		*m_nPortInstrument = GetPort;
		switch (GetFonction)
		{
		case CALO_V1_BP_V2_KEITHLEY:
			*m_bInstrumentKeithleyVoie1 = TRUE;
			*m_bInstrumentKeithleyVoie2 = TRUE;
			m_CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*m_nIndexInstrumentKeithleyVoie2 = READ_LP;
			break;
		case CALO_V1_HP_V2_KEITHLEY:
			*m_bInstrumentKeithleyVoie1 = TRUE;
			*m_bInstrumentKeithleyVoie2 = TRUE;
			m_CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*m_nIndexInstrumentKeithleyVoie2 = READ_HP;
			break;
		case CALO_V1_KEITHLEY:
			*m_bInstrumentKeithleyVoie1 = TRUE;
			*m_bInstrumentKeithleyVoie2 = FALSE;
			m_CBInstrumentKeithleyVoie2->EnableWindow(FALSE);
			break;
		case INSTRUMENT_KEYTHLEY_LP_V2:
			*m_bInstrumentKeithleyVoie1 = FALSE;
			*m_bInstrumentKeithleyVoie2 = TRUE;
			m_CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*m_nIndexInstrumentKeithleyVoie2 = READ_LP;
			break;
		case INSTRUMENT_KEYTHLEY_HP_V2:
			*m_bInstrumentKeithleyVoie1 = FALSE;
			*m_bInstrumentKeithleyVoie2 = TRUE;
			m_CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*m_nIndexInstrumentKeithleyVoie2 = READ_HP;
			break;
		default:
			break;
		}
		break;

	case INSTRUMENT_MENSOR:
		*m_nIndex = INDEX_MENSOR;
		*m_nPortInstrument = GetPort;
		if (GetFonction == INSTRUMENT_MENSOR_LP)
			*m_nInstrumentMensor = READ_LP;
		else if (GetFonction == INSTRUMENT_MENSOR_HP)
			*m_nInstrumentMensor = READ_HP;
		break;
	default:
		break;
	}
}


void ConnectionPort::EnregistrementParametresInstrument(int num_instr, int m_nIndex, int COMInstrument, bool m_bInstrumentKeithleyVoie1, bool m_bInstrumentKeithleyVoie2, int m_nIndexInstrumentKeithleyVoie2, int m_nInstrumentMensor)
{
	int fonction = FUNCTION_NONE;
	switch (m_nIndex)
	{

	case INDEX_AUCUN:
		SetInstrument(num_instr, INSTRUMENT_NONE, -1, FUNCTION_NONE);
		break;

	case INDEX_KEITHLEY:
		if (m_bInstrumentKeithleyVoie1 && m_bInstrumentKeithleyVoie2)
		{
			if (m_nIndexInstrumentKeithleyVoie2 == READ_LP)
				fonction = CALO_V1_BP_V2_KEITHLEY;
			else if (m_nIndexInstrumentKeithleyVoie2 == READ_HP)
				fonction = CALO_V1_HP_V2_KEITHLEY;
		}
		else if (m_bInstrumentKeithleyVoie1)
			fonction = CALO_V1_KEITHLEY;
		else if (m_bInstrumentKeithleyVoie2)
		{
			if (m_nIndexInstrumentKeithleyVoie2 == READ_LP)
				fonction = INSTRUMENT_KEYTHLEY_LP_V2;
			else if (m_nIndexInstrumentKeithleyVoie2 == READ_HP)
				fonction = INSTRUMENT_KEYTHLEY_HP_V2;
		}
		else
		{
			// Problème
			fonction = FUNCTION_NONE;
		}
		SetInstrument(num_instr, INSTRUMENT_KEITHLEY, COMInstrument, fonction);
		break;

	case INDEX_MENSOR:
		if (m_nInstrumentMensor == 0)
			fonction = INSTRUMENT_MENSOR_LP;
		else //(m_nInstrument1Mensor == 1)
			fonction = INSTRUMENT_MENSOR_HP;
		SetInstrument(num_instr, INSTRUMENT_MENSOR, COMInstrument, fonction);
	default:
		break;
	}
}


void ConnectionPort::ShowItem(int m_nIndex, bool m_bInstrumentKeithleyVoie2, CComboBox* m_CBPortInstrument, CButton* m_CheckInstrumentKeithleyVoie1, CButton* m_CheckInstrumentKeithleyVoie2, CComboBox* m_CBInstrumentKeithleyVoie2, CComboBox* m_CBInstrumentMensor)
{
	switch (m_nIndex)
	{
	case INDEX_AUCUN:
	case INSTRUMENT_UNDEF:
	case INSTRUMENT_INEXIST:
		m_CBPortInstrument->ShowWindow(SW_HIDE);
		m_CheckInstrumentKeithleyVoie1->ShowWindow(SW_HIDE);
		m_CheckInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		m_CBInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		m_CBInstrumentKeithleyVoie2->EnableWindow(m_bInstrumentKeithleyVoie2);
		m_CBInstrumentMensor->ShowWindow(SW_HIDE);
		break;
	case INDEX_KEITHLEY:
		m_CBPortInstrument->ShowWindow(SW_SHOW);
		m_CheckInstrumentKeithleyVoie1->ShowWindow(SW_SHOW);
		m_CheckInstrumentKeithleyVoie2->ShowWindow(SW_SHOW);
		m_CBInstrumentKeithleyVoie2->ShowWindow(SW_SHOW);
		m_CBInstrumentKeithleyVoie2->EnableWindow(m_bInstrumentKeithleyVoie2);
		m_CBInstrumentMensor->ShowWindow(SW_HIDE);
		break;
	case INDEX_MENSOR:
		m_CBPortInstrument->ShowWindow(SW_SHOW);
		m_CheckInstrumentKeithleyVoie1->ShowWindow(SW_HIDE);
		m_CheckInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		m_CBInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		m_CBInstrumentKeithleyVoie2->EnableWindow(m_bInstrumentKeithleyVoie2);
		m_CBInstrumentMensor->ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}
}



////////////////////////////////////////////////////////////
//
// Checks
//
////////////////////////////////////////////////////////////

void ConnectionPort::Verifications()
{
	index_verifInstrument = 0;
	bPbm = FALSE;
	bWarning = FALSE;
	bPbmCalo = FALSE;
	bPbmHP = FALSE;
	bPbmBP = FALSE;
	StrPbmCalo.Format(ERROR_MORE_INSTRUMENT_CALORIMETRY);
	StrPbmBP.Format(ERROR_MORE_INSTRUMENT_PRESSURE_LOW);
	StrPbmHP.Format(ERROR_MORE_INSTRUMENT_PRESSURE_HIGH);
	StrMessageErreur = _T("Erreur(s) : \t\n");
	StrMessageWarning = _T("Warning(s) : \t\n");
	
	CM_Calo.SetConnectionMesure(-1, -1);
	CM_HP.SetConnectionMesure(-1, -1);
	CM_BP.SetConnectionMesure(-1, -1);

	VerificationPortUSB();

	if (!AucunInstrumentBranche())
	{
		VerifUnInstrument(1,
			m_nIndexTypeInstrument1,
			m_bInstrument1KeithleyVoie1,
			m_bInstrument1KeithleyVoie2,
			m_nIndexInstrument1KeithleyVoie2,
			m_nInstrument1Mensor,
			m_nPortInstrument1 + 1);

		VerifUnInstrument(2,
			m_nIndexTypeInstrument2,
			m_bInstrument2KeithleyVoie1,
			m_bInstrument2KeithleyVoie2,
			m_nIndexInstrument2KeithleyVoie2,
			m_nInstrument2Mensor,
			m_nPortInstrument2 + 1);

		VerifUnInstrument(3,
			m_nIndexTypeInstrument3,
			m_bInstrument3KeithleyVoie1,
			m_bInstrument3KeithleyVoie2,
			m_nIndexInstrument3KeithleyVoie2,
			m_nInstrument3Mensor,
			m_nPortInstrument3 + 1);

		VerificationLectureMesures();
	}

	VerificationPortCOM();

}


void ConnectionPort::VerificationPortUSB()
{
	if (m_nIndexPortVannes == m_nIndexPortTemperatures)
	{
		bPbm = TRUE;
		CString MessageErreurUSB;
		MessageErreurUSB.Format(ERROR_SAME_PORT_USB, m_nIndexPortVannes + 1);
		StrMessageErreur += MessageErreurUSB;
	}
}

void ConnectionPort::VerificationPortCOM()
{
	for (int i = 0; i<index_verifInstrument - 1; i++)
	{
		for (int j = i + 1; j<index_verifInstrument; j++)
		{
			if (verifInstrument[i].COM == verifInstrument[j].COM)
			{
				CString MessageErreurCOM;
				MessageErreurCOM.Format(ERROR_SAME_PORT_SERIAL, verifInstrument[i].numero, verifInstrument[j].numero, verifInstrument[i].COM);
				StrMessageErreur += MessageErreurCOM;
				bPbm = TRUE;
			}
		}
	}
}


void ConnectionPort::VerificationLectureMesures()
{
	if (CM_Calo.index == -1)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_CALORIMETRY);
		StrMessageWarning += message;
	}

	if (CM_BP.index == -1)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_PRESSURE_LOW);
		StrMessageWarning += message;
	}

	if (CM_HP.index == -1)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_PRESSURE_HIGH);
		StrMessageWarning += message;
	}
}

bool ConnectionPort::AucunInstrumentBranche()
{
	// Aucun appareil branché
	if (m_nIndexTypeInstrument1 == INDEX_AUCUN &&
		m_nIndexTypeInstrument2 == INDEX_AUCUN &&
		m_nIndexTypeInstrument3 == INDEX_AUCUN)
	{
		bPbm = TRUE;

		CString message;
		message.Format(ERROR_NO_DEVICE_SIGNALLED);
		StrMessageErreur += message;

		return true;
	}

	return false;
}


void ConnectionPort::VerifUnInstrument(int num_instr, int m_nIndex, BOOL m_bKeithleyVoie1, BOOL m_bKeithleyVoie2, int m_nInstrumentKeithleyVoie2, int m_nMensor, int PortInstr)
{
	switch (m_nIndex)
	{
	case INDEX_AUCUN:
		break;
	case INDEX_KEITHLEY:
		verifInstrument[index_verifInstrument].numero = num_instr;
		verifInstrument[index_verifInstrument].type = INSTRUMENT_KEITHLEY;
		verifInstrument[index_verifInstrument].COM = PortInstr;
		index_verifInstrument++;

		if (!m_bKeithleyVoie1 && !m_bKeithleyVoie2)
		{
			bPbm = TRUE;
			CString temp;
			temp.Format(ERROR_KEITHLEY_FUNCTION_MISSING, num_instr);
			StrMessageErreur += temp;
		}

		if (m_bKeithleyVoie1)
			VerifMesure(&CM_Calo, num_instr, INSTRUMENT_KEYTHLEY_V1, &StrPbmCalo, &bPbmCalo);

		if (m_bKeithleyVoie2)
		{
			if (m_nInstrumentKeithleyVoie2 == READ_LP)
				VerifMesure(&CM_BP, num_instr, INSTRUMENT_KEYTHLEY_V2, &StrPbmBP, &bPbmBP);
			else //if(m_nInstrumentKeithleyVoie2 == READ_HP)
				VerifMesure(&CM_HP, num_instr, INSTRUMENT_KEYTHLEY_V2, &StrPbmHP, &bPbmHP);
		}
		break;
	case INDEX_MENSOR:
		verifInstrument[index_verifInstrument].numero = num_instr;
		verifInstrument[index_verifInstrument].type = INSTRUMENT_MENSOR;
		verifInstrument[index_verifInstrument].COM = PortInstr;
		index_verifInstrument++;

		if (m_nMensor == READ_LP)
			VerifMesure(&CM_BP, num_instr, MENSOR_VOIE, &StrPbmBP, &bPbmBP);

		if (m_nMensor == READ_HP)
			VerifMesure(&CM_HP, num_instr, MENSOR_VOIE, &StrPbmHP, &bPbmHP);
		break;
	default:
		break;
	}
}

void ConnectionPort::VerifMesure(ConnectionMesure* CM, int num_instr, int voie_mesure, CString* StrPbm, BOOL* bPbmMesure)
{
	if (CM->index != -1)
	{
		*bPbmMesure = TRUE;
		bPbm = TRUE;
	}
	else
		CM->SetConnectionMesure(voie_mesure, num_instr);

	CString temp;
	temp.Format(_T(" Instrument %d"), num_instr);
	*StrPbm += temp;
}

void ConnectionPort::EnregistrementVerifications()
{
	if (CM_Calo.index == -1)
		SetMesureCalo(FALSE);
	else
		SetMesureCalo(TRUE);

	if (CM_BP.index == -1)
		SetMesureBassePression(FALSE);
	else
		SetMesureBassePression(TRUE);

	if (CM_HP.index == -1)
		SetMesureHautePression(FALSE);
	else
		SetMesureHautePression(TRUE);
}