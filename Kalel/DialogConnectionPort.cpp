// Connection_port.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "DialogConnectionPort.h"

#include "Com Classes/MachineSettings.h"		// Accessing the settings

#include "Resources/StringTable.h"				// Error message strings
#include "Resources/DefineStages.h"				// Experiment types

// Boîte de dialogue ConnectionPort

IMPLEMENT_DYNAMIC(ConnectionPort, CDialog)

ConnectionPort::ConnectionPort(CWnd* pParent /*=NULL*/)
	: CDialog(ConnectionPort::IDD, pParent)
	
	, settings{ nullptr }
	, m_nIndexPortVannes{ 0 }
	, m_nIndexPortTemperatures{ 0 }
	, modified{ false }
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
	}

	m_CBTypeInstrument.push_back(&m_CBTypeInstrument1);
	m_CBTypeInstrument.push_back(&m_CBTypeInstrument2);
	m_CBTypeInstrument.push_back(&m_CBTypeInstrument3);
								 
	m_CBPortInstrument.push_back(&m_CBPortInstrument1);
	m_CBPortInstrument.push_back(&m_CBPortInstrument2);
	m_CBPortInstrument.push_back(&m_CBPortInstrument3);

	m_CBInstrumentKeithleyVoie2.push_back(&m_CBInstrument1KeithleyVoie2);
	m_CBInstrumentKeithleyVoie2.push_back(&m_CBInstrument2KeithleyVoie2);
	m_CBInstrumentKeithleyVoie2.push_back(&m_CBInstrument3KeithleyVoie2);

	m_CBInstrumentMensor.push_back(&m_CBInstrumentMensor1);
	m_CBInstrumentMensor.push_back(&m_CBInstrumentMensor2);
	m_CBInstrumentMensor.push_back(&m_CBInstrumentMensor3);

	m_CheckInstrumentKeithleyVoie1.push_back(&m_CheckInstrument1KeithleyVoie1);
	m_CheckInstrumentKeithleyVoie1.push_back(&m_CheckInstrument2KeithleyVoie1);
	m_CheckInstrumentKeithleyVoie1.push_back(&m_CheckInstrument3KeithleyVoie1);

	m_CheckInstrumentKeithleyVoie2.push_back(&m_CheckInstrument1KeithleyVoie2);
	m_CheckInstrumentKeithleyVoie2.push_back(&m_CheckInstrument2KeithleyVoie2);
	m_CheckInstrumentKeithleyVoie2.push_back(&m_CheckInstrument3KeithleyVoie2);
}

void ConnectionPort::PassSettings(MachineSettings * machineSettings)
{
	settings = machineSettings;
}

ConnectionPort::~ConnectionPort()
{
}

bool ConnectionPort::Changed()
{
	return modified;
}

std::shared_ptr<MachineSettings> ConnectionPort::GetSettings()
{
	return localSettings;
}

void ConnectionPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT1, m_CBTypeInstrument1);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT2, m_CBTypeInstrument2);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT3, m_CBTypeInstrument3);
	
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT1, m_nIndexTypeInstrument[0]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT2, m_nIndexTypeInstrument[1]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT3, m_nIndexTypeInstrument[2]);
	
	DDX_Control(pDX, IDC_PORT_INSTRUMENT1, m_CBPortInstrument1);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT2, m_CBPortInstrument2);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT3, m_CBPortInstrument3);
	
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT1, m_nPortInstrument[0]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT2, m_nPortInstrument[1]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT3, m_nPortInstrument[2]);
	
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, m_CheckInstrument1KeithleyVoie1);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE1, m_CheckInstrument2KeithleyVoie1);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE1, m_CheckInstrument3KeithleyVoie1);
	
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, m_CheckInstrument1KeithleyVoie2);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, m_CheckInstrument2KeithleyVoie2);
	DDX_Control(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, m_CheckInstrument3KeithleyVoie2);
	
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, m_bInstrumentKeithleyVoie1[0]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE1, m_bInstrumentKeithleyVoie1[1]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE1, m_bInstrumentKeithleyVoie1[2]);
	
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2, m_bInstrumentKeithleyVoie2[0]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT2_KEITHLEY_VOIE2, m_bInstrumentKeithleyVoie2[1]);
	DDX_Check(pDX, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, m_bInstrumentKeithleyVoie2[2]);
	
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, m_CBInstrument1KeithleyVoie2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT2_KEITHLEY_VOIE2, m_CBInstrument2KeithleyVoie2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT3_KEITHLEY_VOIE2, m_CBInstrument3KeithleyVoie2);
	
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, m_nIndexInstrumentKeithleyVoie2[0]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT2_KEITHLEY_VOIE2, m_nIndexInstrumentKeithleyVoie2[1]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT3_KEITHLEY_VOIE2, m_nIndexInstrumentKeithleyVoie2[2]);

	DDX_Control(pDX, IDC_COMBO_INSTRUMENT1_MENSOR, m_CBInstrumentMensor1);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT2_MENSOR, m_CBInstrumentMensor2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT3_MENSOR, m_CBInstrumentMensor3);

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
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO_INSTRUMENT1_KEITHLEY_VOIE2, IDC_COMBO_INSTRUMENT3_MENSOR, &ConnectionPort::OnModified)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE1, IDC_CHECK_INSTRUMENT3_KEITHLEY_VOIE2, &ConnectionPort::OnModified)
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
		if (modified)
		{
			localSettings = std::make_shared<MachineSettings>(*settings);
			EnregistrementConnection_port(*localSettings);
			EnregistrementVerifications(*localSettings);
		}
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
		if (modified)
		{
			localSettings = std::make_shared<MachineSettings>(*settings);
			EnregistrementConnection_port(*localSettings);
			EnregistrementVerifications(*localSettings);
		}
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
	modified = true;

	UpdateData(TRUE);
	
	// Show instrument 1
	ShowItem(m_nIndexTypeInstrument[0],
		m_bInstrumentKeithleyVoie2[0],
		m_CBPortInstrument[0],
		m_CheckInstrumentKeithleyVoie1[0],
		m_CheckInstrumentKeithleyVoie2[0],
		m_CBInstrumentKeithleyVoie2[0],
		m_CBInstrumentMensor[0]);
	modified = true;
	UpdateData(FALSE);
}

void ConnectionPort::OnCbnSelchangeComboTypeInstrument2()
{
	modified = true;

	UpdateData(TRUE);

	// Show instrument 2
	ShowItem(m_nIndexTypeInstrument[1],
		m_bInstrumentKeithleyVoie2[1],
		m_CBPortInstrument[1],
		m_CheckInstrumentKeithleyVoie1[1],
		m_CheckInstrumentKeithleyVoie2[1],
		m_CBInstrumentKeithleyVoie2[1],
		m_CBInstrumentMensor[1]);

	UpdateData(FALSE);
}

void ConnectionPort::OnCbnSelchangeComboTypeInstrument3()
{
	modified = true;

	UpdateData(TRUE);

	// Show instrument 3
	ShowItem(m_nIndexTypeInstrument[2],
		m_bInstrumentKeithleyVoie2[2],
		m_CBPortInstrument[2],
		m_CheckInstrumentKeithleyVoie1[2],
		m_CheckInstrumentKeithleyVoie2[2],
		m_CBInstrumentKeithleyVoie2[2],
		m_CBInstrumentMensor[2]);

	UpdateData(FALSE);
}

void ConnectionPort::OnModified(UINT nID)
{
	for (UINT i = 0; i < m_CBInstrumentKeithleyVoie2.size(); i++)
	{
		if (nID == IDC_CHECK_INSTRUMENT1_KEITHLEY_VOIE2 + i)
		{
			UpdateData(TRUE);
			m_CBInstrumentKeithleyVoie2[i]->EnableWindow(m_bInstrumentKeithleyVoie2[i]);
			UpdateData(FALSE);
		}
	}
	
	modified = true;
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
		EnregistrementParametresInstrument(instrument,
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
			m_CBInstrumentKeithleyVoie2[instrument],
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
			m_CBPortInstrument[instrument],
			m_CheckInstrumentKeithleyVoie1[instrument],
			m_CheckInstrumentKeithleyVoie2[instrument],
			m_CBInstrumentKeithleyVoie2[instrument],
			m_CBInstrumentMensor[instrument]);
	}
}



void ConnectionPort::InitDonneesInstrument(int TypeInstr, int* m_nIndex, int* portInstrument, BOOL* instrumentKeithleyVoie1, BOOL* instrumentKeithleyVoie2, CComboBox* CBInstrumentKeithleyVoie2, int* indexInstrumentKeithleyVoie2, int* instrumentMensor, int GetPort, int GetFonction)
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
		*portInstrument = GetPort;
		switch (GetFonction)
		{
		case CALO_V1_BP_V2_KEITHLEY:
			*instrumentKeithleyVoie1 = TRUE;
			*instrumentKeithleyVoie2 = TRUE;
			CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*indexInstrumentKeithleyVoie2 = READ_LP;
			break;
		case CALO_V1_HP_V2_KEITHLEY:
			*instrumentKeithleyVoie1 = TRUE;
			*instrumentKeithleyVoie2 = TRUE;
			CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*indexInstrumentKeithleyVoie2 = READ_HP;
			break;
		case CALO_V1_KEITHLEY:
			*instrumentKeithleyVoie1 = TRUE;
			*instrumentKeithleyVoie2 = FALSE;
			CBInstrumentKeithleyVoie2->EnableWindow(FALSE);
			break;
		case INSTRUMENT_KEYTHLEY_LP_V2:
			*instrumentKeithleyVoie1 = FALSE;
			*instrumentKeithleyVoie2 = TRUE;
			CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*indexInstrumentKeithleyVoie2 = READ_LP;
			break;
		case INSTRUMENT_KEYTHLEY_HP_V2:
			*instrumentKeithleyVoie1 = FALSE;
			*instrumentKeithleyVoie2 = TRUE;
			CBInstrumentKeithleyVoie2->EnableWindow(TRUE);
			*indexInstrumentKeithleyVoie2 = READ_HP;
			break;
		default:
			break;
		}
		break;

	case INSTRUMENT_MENSOR:
		*m_nIndex = INDEX_MENSOR;
		*portInstrument = GetPort;
		if (GetFonction == INSTRUMENT_MENSOR_LP)
			*instrumentMensor = READ_LP;
		else if (GetFonction == INSTRUMENT_MENSOR_HP)
			*instrumentMensor = READ_HP;
		break;
	default:
		break;
	}
}


void ConnectionPort::EnregistrementParametresInstrument(int num_instr, 
														int m_nIndex, 
														int COMInstrument, 
														bool instrumentKeithleyVoie1, 
														bool instrumentKeithleyVoie2, 
														int indexInstrumentKeithleyVoie2, 
														int instrumentMensor,
														MachineSettings & newSettings)
{
	int fonction = FUNCTION_NONE;
	switch (m_nIndex)
	{

	case INDEX_AUCUN:
		newSettings.SetInstrument(num_instr, INSTRUMENT_NONE, -1, FUNCTION_NONE);
		break;

	case INDEX_KEITHLEY:
		if (instrumentKeithleyVoie1 && instrumentKeithleyVoie2)
		{
			if (indexInstrumentKeithleyVoie2 == READ_LP)
				fonction = CALO_V1_BP_V2_KEITHLEY;
			else if (indexInstrumentKeithleyVoie2 == READ_HP)
				fonction = CALO_V1_HP_V2_KEITHLEY;
		}
		else if (instrumentKeithleyVoie1)
			fonction = CALO_V1_KEITHLEY;
		else if (instrumentKeithleyVoie2)
		{
			if (indexInstrumentKeithleyVoie2 == READ_LP)
				fonction = INSTRUMENT_KEYTHLEY_LP_V2;
			else if (indexInstrumentKeithleyVoie2 == READ_HP)
				fonction = INSTRUMENT_KEYTHLEY_HP_V2;
		}
		else
		{
			// Problème
			fonction = FUNCTION_NONE;
		}
		newSettings.SetInstrument(num_instr, INSTRUMENT_KEITHLEY, COMInstrument, fonction);
		break;

	case INDEX_MENSOR:
		if (instrumentMensor == 0)
			fonction = INSTRUMENT_MENSOR_LP;
		else //(m_nInstrument1Mensor == 1)
			fonction = INSTRUMENT_MENSOR_HP;
		newSettings.SetInstrument(num_instr, INSTRUMENT_MENSOR, COMInstrument, fonction);
	default:
		break;
	}
}


void ConnectionPort::ShowItem(int m_nIndex, bool instrumentKeithleyVoie2, CComboBox* CBPortInstrument, CButton* checkInstrumentKeithleyVoie1, CButton* checkInstrumentKeithleyVoie2, CComboBox* CBInstrumentKeithleyVoie2, CComboBox* CBInstrumentMensor)
{
	switch (m_nIndex)
	{
	case INDEX_AUCUN:
	case INSTRUMENT_UNDEF:
	case INSTRUMENT_INEXIST:
		CBPortInstrument->ShowWindow(SW_HIDE);
		checkInstrumentKeithleyVoie1->ShowWindow(SW_HIDE);
		checkInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		CBInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		CBInstrumentKeithleyVoie2->EnableWindow(instrumentKeithleyVoie2);
		CBInstrumentMensor->ShowWindow(SW_HIDE);
		break;
	case INDEX_KEITHLEY:
		CBPortInstrument->ShowWindow(SW_SHOW);
		checkInstrumentKeithleyVoie1->ShowWindow(SW_SHOW);
		checkInstrumentKeithleyVoie2->ShowWindow(SW_SHOW);
		CBInstrumentKeithleyVoie2->ShowWindow(SW_SHOW);
		CBInstrumentKeithleyVoie2->EnableWindow(instrumentKeithleyVoie2);
		CBInstrumentMensor->ShowWindow(SW_HIDE);
		break;
	case INDEX_MENSOR:
		CBPortInstrument->ShowWindow(SW_SHOW);
		checkInstrumentKeithleyVoie1->ShowWindow(SW_HIDE);
		checkInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		CBInstrumentKeithleyVoie2->ShowWindow(SW_HIDE);
		CBInstrumentKeithleyVoie2->EnableWindow(instrumentKeithleyVoie2);
		CBInstrumentMensor->ShowWindow(SW_SHOW);
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
	// Aucun appareil branché
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
		newSettings.HighPressureToMeasure = true;
}

