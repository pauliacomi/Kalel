// Connection_port.cpp�: fichier d'impl�mentation
//
#include "stdafx.h"
#include "DialogConnectionPort.h"


#include "../Kalel Shared/Resources/StringTable.h"				// Error message strings
#include "../Kalel Shared/Com Classes/MachineSettings.h"		// Accessing the settings
#include "../Kalel Shared/Resources/DefineStages.h"				// Experiment types

// Bo�te de dialogue ConnectionPort

IMPLEMENT_DYNAMIC(ConnectionPort, CDialog)

ConnectionPort::ConnectionPort(CWnd* pParent /*=NULL*/)
	: CDialog(ConnectionPort::IDD, pParent)

	, m_nIndexPortVannes(0)
	, m_nIndexPortTemperatures(0)
{
	// CURRENTLY ONLY DESIGNED FOR A MAXIMUM OF 3 INSTURMENTS
	for (size_t i = 0; i < NB_OF_INSTRUMENTS; i++)
	{
		// Variable vectors
		m_nIndexTypeInstrument.push_back(INDEX_AUCUN);
		m_nPortInstrument.push_back(0);
		m_nIndexInstrumentKeithleyVoie2.push_back(INDEX_AUCUN);
		m_nInstrumentMensor.push_back(0);
		m_bInstrumentKeithleyVoie1.push_back(FALSE);
		m_bInstrumentKeithleyVoie2.push_back(FALSE);

		// Control Vectors
		m_CBTypeInstrument.emplace_back();
		m_CBPortInstrument.emplace_back();
		m_CheckInstrumentKeithleyVoie1.emplace_back();
		m_CheckInstrumentKeithleyVoie2.emplace_back();
		m_CBInstrumentKeithleyVoie2.emplace_back();
		m_CBInstrumentMensor.emplace_back();
	}
}

void ConnectionPort::PassSettings(const MachineSettings & machineSettings)
{
	*settings = machineSettings;
}

ConnectionPort::~ConnectionPort()
{
}

void ConnectionPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT1, m_CBTypeInstrument[0]);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT2, m_CBTypeInstrument[1]);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT3, m_CBTypeInstrument[2]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT1, m_nIndexTypeInstrument[0]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT2, m_nIndexTypeInstrument[1]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT3, m_nIndexTypeInstrument[2]);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT1, m_CBPortInstrument[0]);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT2, m_CBPortInstrument[1]);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT3, m_CBPortInstrument[2]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT1, m_nPortInstrument[0]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT2, m_nPortInstrument[1]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT3, m_nPortInstrument[2]);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, m_CheckInstrumentKeithleyVoie1[0]);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE1, m_CheckInstrumentKeithleyVoie1[1]);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE1, m_CheckInstrumentKeithleyVoie1[2]);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, m_CheckInstrumentKeithleyVoie2[0]);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, m_CheckInstrumentKeithleyVoie2[1]);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, m_CheckInstrumentKeithleyVoie2[2]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, m_bInstrumentKeithleyVoie1[0]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE1, m_bInstrumentKeithleyVoie1[1]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE1, m_bInstrumentKeithleyVoie1[2]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, m_bInstrumentKeithleyVoie2[0]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, m_bInstrumentKeithleyVoie2[1]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, m_bInstrumentKeithleyVoie2[2]);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, m_CBInstrumentKeithleyVoie2[0]);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT2_KEITHLEY_VOIE2, m_CBInstrumentKeithleyVoie2[1]);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT3_KEITHLEY_VOIE2, m_CBInstrumentKeithleyVoie2[2]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, m_nIndexInstrumentKeithleyVoie2[0]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT2_KEITHLEY_VOIE2, m_nIndexInstrumentKeithleyVoie2[1]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT3_KEITHLEY_VOIE2, m_nIndexInstrumentKeithleyVoie2[2]);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT1_MENSOR, m_CBInstrumentMensor[0]);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT2_MENSOR, m_CBInstrumentMensor[1]);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT3_MENSOR, m_CBInstrumentMensor[2]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT1_MENSOR, m_nInstrumentMensor[0]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT2_MENSOR, m_nInstrumentMensor[1]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT3_MENSOR, m_nInstrumentMensor[2]);
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
	// EXCEPTION�: les pages de propri�t�s OCX devraient retourner FALSE
}


void ConnectionPort::OnBnClickedOk()
{
	UpdateData(TRUE);


	Verifications();

	if (!bPbm && !bWarning)
	{
		MachineSettings newSettings;
		EnregistrementConnection_port(newSettings);
		EnregistrementVerifications(newSettings);
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
		MachineSettings newSettings;
		EnregistrementConnection_port(newSettings);
		EnregistrementVerifications(newSettings);
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
	ShowItem(m_nIndexTypeInstrument[0],
		m_bInstrumentKeithleyVoie2[0],
		&m_CBPortInstrument[0],
		&m_CheckInstrumentKeithleyVoie1[0],
		&m_CheckInstrumentKeithleyVoie2[0],
		&m_CBInstrumentKeithleyVoie2[0],
		&m_CBInstrumentMensor[0]);

	UpdateData(FALSE);
}

void ConnectionPort::OnCbnSelchangeComboTypeInstrument2()
{
	UpdateData(TRUE);

	// Show instrument 2
	ShowItem(m_nIndexTypeInstrument[1],
		m_bInstrumentKeithleyVoie2[1],
		&m_CBPortInstrument[1],
		&m_CheckInstrumentKeithleyVoie1[1],
		&m_CheckInstrumentKeithleyVoie2[1],
		&m_CBInstrumentKeithleyVoie2[1],
		&m_CBInstrumentMensor[1]);

	UpdateData(FALSE);
}

void ConnectionPort::OnCbnSelchangeComboTypeInstrument3()
{
	UpdateData(TRUE);

	// Show instrument 3
	ShowItem(m_nIndexTypeInstrument[2],
		m_bInstrumentKeithleyVoie2[2],
		&m_CBPortInstrument[2],
		&m_CheckInstrumentKeithleyVoie1[2],
		&m_CheckInstrumentKeithleyVoie2[2],
		&m_CBInstrumentKeithleyVoie2[2],
		&m_CBInstrumentMensor[2]);

	UpdateData(FALSE);
}

void ConnectionPort::OnBnClickedCheckInstrument1KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrumentKeithleyVoie2[0].EnableWindow (m_bInstrumentKeithleyVoie2[0]);
	UpdateData(FALSE);
}

void ConnectionPort::OnBnClickedCheckInstrument2KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrumentKeithleyVoie2[1].EnableWindow (m_bInstrumentKeithleyVoie2[1]);
	UpdateData(FALSE);
}

void ConnectionPort::OnBnClickedCheckInstrument3KeithleyVoie2()
{
	UpdateData(TRUE);
	m_CBInstrumentKeithleyVoie2[2].EnableWindow (m_bInstrumentKeithleyVoie2[2]);
	UpdateData(FALSE);
}



////////////////////////////////////////////////////////////
//
// Functions
//
////////////////////////////////////////////////////////////


void ConnectionPort::EnregistrementConnection_port(MachineSettings& newSettings)
{
	// Save instruments
	for (size_t instrument = 0; instrument < NB_OF_INSTRUMENTS; instrument++)
	{
		EnregistrementParametresInstrument(1,
			m_nIndexTypeInstrument[instrument],
			m_nPortInstrument[instrument] + 1,
			m_bInstrumentKeithleyVoie1[instrument],
			m_bInstrumentKeithleyVoie2[instrument],
			m_nIndexInstrumentKeithleyVoie2[instrument],
			m_nInstrumentMensor[instrument],
			newSettings
		);
	}

	// Save valve port
	newSettings.PortVannes = m_nIndexPortVannes + 1;

	// Save temperature port
	newSettings.PortTemperatures = m_nIndexPortTemperatures + 1;
}


void ConnectionPort::InitConnection_port()
{
	// Initiate instruments
	for (size_t instrument = 0; instrument < NB_OF_INSTRUMENTS; instrument++)
	{
		InitDonneesInstrument(settings->TypeInstrument(instrument),
			&m_nIndexTypeInstrument[instrument],
			&m_nPortInstrument[instrument],
			&m_bInstrumentKeithleyVoie1[instrument],
			&m_bInstrumentKeithleyVoie2[instrument],
			&m_CBInstrumentKeithleyVoie2[instrument],
			&m_nIndexInstrumentKeithleyVoie2[instrument],
			&m_nInstrumentMensor[instrument],
			settings->COMInstrument(instrument) - 1,
			settings->FonctionInstrument(instrument));
	}

	// Get port valves
	m_nIndexPortVannes = settings->PortVannes - 1;

	// Port temperatures
	m_nIndexPortTemperatures = settings->PortTemperatures - 1;

	UpdateData(FALSE);

	// Show instruments
	for (size_t instrument = 0; instrument < NB_OF_INSTRUMENTS; instrument++)
	{
		ShowItem(m_nIndexTypeInstrument[instrument],
			m_bInstrumentKeithleyVoie2[instrument],
			&m_CBPortInstrument[instrument],
			&m_CheckInstrumentKeithleyVoie1[instrument],
			&m_CheckInstrumentKeithleyVoie2[instrument],
			&m_CBInstrumentKeithleyVoie2[instrument],
			&m_CBInstrumentMensor[instrument]);
	}
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


void ConnectionPort::EnregistrementParametresInstrument(int num_instr, 
														int m_nIndex, 
														int COMInstrument, 
														bool m_bInstrumentKeithleyVoie1, 
														bool m_bInstrumentKeithleyVoie2, 
														int m_nIndexInstrumentKeithleyVoie2, 
														int m_nInstrumentMensor,
														MachineSettings & newSettings)
{
	int fonction = FUNCTION_NONE;
	switch (m_nIndex)
	{

	case INDEX_AUCUN:
		newSettings.SetInstrument(num_instr, INSTRUMENT_NONE, -1, FUNCTION_NONE);
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
			// Probl�me
			fonction = FUNCTION_NONE;
		}
		newSettings.SetInstrument(num_instr, INSTRUMENT_KEITHLEY, COMInstrument, fonction);
		break;

	case INDEX_MENSOR:
		if (m_nInstrumentMensor == 0)
			fonction = INSTRUMENT_MENSOR_LP;
		else //(m_nInstrument1Mensor == 1)
			fonction = INSTRUMENT_MENSOR_HP;
		newSettings.SetInstrument(num_instr, INSTRUMENT_MENSOR, COMInstrument, fonction);
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
		for (size_t instrument = 0; instrument < NB_OF_INSTRUMENTS; instrument++)
		{
			VerifUnInstrument(instrument,
				m_nIndexTypeInstrument[instrument],
				m_bInstrumentKeithleyVoie1[instrument],
				m_bInstrumentKeithleyVoie2[instrument],
				m_nIndexInstrumentKeithleyVoie2[instrument],
				m_nInstrumentMensor[instrument],
				m_nPortInstrument[instrument] + 1);
		}

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
	// Aucun appareil branch�
	bool notConn = true;

	for (size_t i = 0; i < m_nIndexTypeInstrument.size(); i++)
	{
		if (m_nIndexTypeInstrument[i] != INDEX_AUCUN)
		{
			notConn = false;
		}
	}

	if (notConn)
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

void ConnectionPort::EnregistrementVerifications(MachineSettings& newSettings)
{
	if (CM_Calo.index == -1)
		newSettings.CaloToMeasure = false;
	else
		newSettings.CaloToMeasure = true;

	if (CM_BP.index == -1)
		newSettings.LowPressureToMeasure = false;
	else
		newSettings.LowPressureToMeasure = true;

	if (CM_HP.index == -1)
		newSettings.HighPressureToMeasure = false;
	else
		newSettings.HighPressureToMeasure = false;
}