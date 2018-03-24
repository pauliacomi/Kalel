// Connection_port.cpp : fichier d'implémentation
//
#include "stdafx.h"
#include "DialogConnectionPort.h"

#include "../Kalel Shared/Com Classes/MachineSettings.h"		// Accessing the settings

#include "../Kalel Shared/Resources/StringTable.h"				// Error message strings
#include "../Kalel Shared/Resources/DefineStages.h"				// Experiment types

// Boîte de dialogue ConnectionPort

IMPLEMENT_DYNAMIC(ConnectionPort, CDialog)

ConnectionPort::ConnectionPort(CWnd* pParent /*=NULL*/)
	: CDialog(ConnectionPort::IDD, pParent)
{
}

void ConnectionPort::PassSettings(const MachineSettings &machineSettings)
{
	localSettings = std::make_unique<MachineSettings>(machineSettings);

	// instruments
	for (auto i : machineSettings.instruments)
	{
		i_type[i.first - 1] = static_cast<int>(i.second.type);
		i_ports[i.first - 1] = static_cast<int>(i.second.port);
	}

	// Readers
	for (auto i: machineSettings.readers)
	{
		int num;

		switch (i.first)
		{
		case TEMPERATURE_CALO: num = 0; break;
		case TEMPERATURE_CAGE: num = 1; break;
		case TEMPERATURE_ROOM: num = 2; break;
		case PRESSURE_HP: num = 3; break;
		case PRESSURE_LP: num = 4; break;
		case CALO: num = 5; break;
		default: break;
		}

		r_present					[num] = TRUE;
		r_instrument				[num] = i.second.instrument;
		r_instrument_channel		[num] = i.second.channel;
		r_sensitivity				[num] = i.second.sensitivity;
		r_safe_min					[num] = i.second.safe_min;
		r_safe_max					[num] = i.second.safe_max;
	}

	// Controllers
	for (auto i : machineSettings.controllers)
	{
		int num = 0;

		switch (i.first)
		{
		case ID_VALVE_1: num = 0; break;
		case ID_VALVE_2: num = 1; break;
		case ID_VALVE_3: num = 2; break;
		case ID_VALVE_4: num = 3; break;
		case ID_VALVE_5: num = 4; break;
		case ID_VALVE_6: num = 5; break;
		case ID_VALVE_7: num = 6; break;
		case ID_VALVE_8: num = 7; break;
		case ID_EV_1: num = 8; break;
		case ID_EV_2: num = 9; break;
		case ID_PUMP: num = 10; break;
		default: break;
		}

		c_present				[num] = TRUE;
		c_instrument			[num] = i.second.instrument;
		c_instrument_channel	[num] = i.second.channel;
		c_instrument_subchannel	[num] = i.second.subchannel;
	}
}

ConnectionPort::~ConnectionPort()
{
}

bool ConnectionPort::Changed()
{
	return modified;
}

MachineSettings * ConnectionPort::GetSettings()
{
	return localSettings.release();
}

void ConnectionPort::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//**********************************
	//
	//	Instruments
	//
	//**********************************

	// Instrument name
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_1, i_type[0]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_2, i_type[1]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_3, i_type[2]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_4, i_type[3]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_5, i_type[4]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_6, i_type[5]);

	// Port

	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_1, i_ports[0]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_2, i_ports[1]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_3, i_ports[2]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_4, i_ports[3]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_5, i_ports[4]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_6, i_ports[5]);

	//**********************************
	//
	//	Readers
	//
	//**********************************

	// Checkboxes
	DDX_Check(pDX, IDC_CHECK_T1, r_present[0]);
	DDX_Check(pDX, IDC_CHECK_T2, r_present[1]);
	DDX_Check(pDX, IDC_CHECK_T3, r_present[2]);
	DDX_Check(pDX, IDC_CHECK_P1, r_present[3]);
	DDX_Check(pDX, IDC_CHECK_P2, r_present[4]);
	DDX_Check(pDX, IDC_CHECK_C1, r_present[5]);

	// Instrument number
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T1, r_instrument[0]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T2, r_instrument[1]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T3, r_instrument[2]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_P1, r_instrument[3]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_P2, r_instrument[4]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_C1, r_instrument[5]);

	// Channel
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T1, r_instrument_channel[0]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T2, r_instrument_channel[1]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T3, r_instrument_channel[2]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_P1, r_instrument_channel[3]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_P2, r_instrument_channel[4]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_C1, r_instrument_channel[5]);

	DDX_Text(pDX, IDC_EDIT_T4, r_safe_min[0]);
	DDX_Text(pDX, IDC_EDIT_T5, r_safe_min[1]);
	DDX_Text(pDX, IDC_EDIT_T6, r_safe_min[2]);
	DDX_Text(pDX, IDC_EDIT_P3, r_safe_min[3]);
	DDX_Text(pDX, IDC_EDIT_P4, r_safe_min[4]);
	DDX_Text(pDX, IDC_EDIT_C2, r_safe_min[5]);

	DDX_Text(pDX, IDC_EDIT_T7, r_safe_max[0]);
	DDX_Text(pDX, IDC_EDIT_T8, r_safe_max[1]);
	DDX_Text(pDX, IDC_EDIT_T9, r_safe_max[2]);
	DDX_Text(pDX, IDC_EDIT_P5, r_safe_max[3]);
	DDX_Text(pDX, IDC_EDIT_P6, r_safe_max[4]);
	DDX_Text(pDX, IDC_EDIT_C3, r_safe_max[5]);

	// Sensitivity
	DDX_Text(pDX, IDC_EDIT_T1, r_sensitivity[0]);
	DDX_Text(pDX, IDC_EDIT_T2, r_sensitivity[1]);
	DDX_Text(pDX, IDC_EDIT_T3, r_sensitivity[2]);
	DDX_Text(pDX, IDC_EDIT_P1, r_sensitivity[3]);
	DDX_Text(pDX, IDC_EDIT_P2, r_sensitivity[4]);
	DDX_Text(pDX, IDC_EDIT_C1, r_sensitivity[5]);

	//**********************************
	//
	//	Controllers
	//
	//**********************************

	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T4, c_instrument[0]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T5, c_instrument[1]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T6, c_instrument[2]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T7, c_instrument[3]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T8, c_instrument[4]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T9, c_instrument[5]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T10, c_instrument[6]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T11, c_instrument[7]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T12, c_instrument[8]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T13, c_instrument[9]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T14, c_instrument[10]);

	// Channel
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T4, c_instrument_channel[0]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T6, c_instrument_channel[1]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T8, c_instrument_channel[2]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T10, c_instrument_channel[3]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T12, c_instrument_channel[4]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T14, c_instrument_channel[5]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T16, c_instrument_channel[6]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T18, c_instrument_channel[7]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T20, c_instrument_channel[8]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T22, c_instrument_channel[9]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T24, c_instrument_channel[10]);


	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T5, c_instrument_subchannel[0]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T7, c_instrument_subchannel[1]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T9, c_instrument_subchannel[2]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T11, c_instrument_subchannel[3]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T13, c_instrument_subchannel[4]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T15, c_instrument_subchannel[5]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T17, c_instrument_subchannel[6]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T19, c_instrument_subchannel[7]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T21, c_instrument_subchannel[8]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T23, c_instrument_subchannel[9]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T25, c_instrument_subchannel[10]);

}



////////////////////////////////////////////////////////////
//
// Message map + functions
//
////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(ConnectionPort, CDialog)
	ON_BN_CLICKED(IDOK, &ConnectionPort::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &ConnectionPort::OnBnClickedCancel)

	// Dropdown-down boxes
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_COMBO_INSTRUMENT_T1, IDC_COMBO_CHANNEL_T25, &ConnectionPort::OnModified)
	// Edit boxes
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_T1, IDC_EDIT_C3, &ConnectionPort::OnModified)
	// Check boxes
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHECK_T1, IDC_CHECK_C1, &ConnectionPort::OnModified)

END_MESSAGE_MAP()


// Gestionnaires de messages de ConnectionPort

BOOL ConnectionPort::OnInitDialog()
{
	CDialog::OnInitDialog();


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
			SaveModifications(*localSettings);
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
			SaveModifications(*localSettings);
		}
		OnOK();
	}
}

void ConnectionPort::OnBnClickedCancel()
{
	OnCancel();
}


void ConnectionPort::OnModified(UINT nID)
{
	modified = true;
}

////////////////////////////////////////////////////////////
//
// Functions
//
////////////////////////////////////////////////////////////


void ConnectionPort::SaveModifications(MachineSettings& newSettings)
{
	// Save instrument
	for (size_t key = 0; key < NB_INSTRUMENTS; key++)
	{
		Instrument i;
		i.type = i_type[key];
		i.port = i_ports[key];
		newSettings.AddInstrument(i, key + 1);
	}

	// Save reader
	for (size_t key = 0; key < NB_READERS; key++)
	{
		Reader r;

		switch (key)
		{
		case 0:
			r.type = READER_TEMPERATURE;
			r.identifier = 1;
			break;
		case 1:
			r.type = READER_TEMPERATURE;
			r.identifier = 2;
			break;
		case 2:
			r.type = READER_TEMPERATURE;
			r.identifier = 3;
			break;
		case 3:
			r.type = READER_PRESSURE;
			r.identifier = 2;
			break;
		case 4:
			r.type = READER_PRESSURE;
			r.identifier = 1;
			break;
		case 5:
			r.type = READER_CALO;
			r.identifier = 1;
			break;
		default:
			break;
		}
		r.instrument = r_instrument[key];
		r.channel = r_instrument_channel[key];
		r.sensitivity = r_sensitivity[key];
		r.safe_min = r_safe_min[key];
		r.safe_max = r_safe_max[key];
		
		newSettings.AddReader(r, (r.type + r.identifier));
	}


	// Save controllers
	for (size_t key = 0; key < NB_CONTROLLERS; key++)
	{
		Controller r;

		switch (key)
		{
		case 0:
			r.type = CONTROLLER_VALVE;
			r.identifier = 1;
			break;
		case 1:
			r.type = CONTROLLER_VALVE;
			r.identifier = 2;
			break;
		case 2:
			r.type = CONTROLLER_VALVE;
			r.identifier = 3;
			break;
		case 3:
			r.type = CONTROLLER_VALVE;
			r.identifier = 4;
			break;
		case 4:
			r.type = CONTROLLER_VALVE;
			r.identifier = 5;
			break;
		case 5:
			r.type = CONTROLLER_VALVE;
			r.identifier = 6;
			break;
		case 6:
			r.type = CONTROLLER_VALVE;
			r.identifier = 7;
			break;
		case 7:
			r.type = CONTROLLER_VALVE;
			r.identifier = 8;
			break;
		case 8:
			r.type = CONTROLLER_EV;
			r.identifier = 1;
			break;
		case 9:
			r.type = CONTROLLER_EV;
			r.identifier = 2;
			break;
		case 10:
			r.type = CONTROLLER_PUMP;
			r.identifier = 1;
			break;
		default:
			break;
		}
		r.instrument = c_instrument[key];
		r.channel = c_instrument_channel[key];
		r.subchannel= c_instrument_subchannel[key];

		newSettings.AddController(r, (r.type + r.identifier));
	}
}



////////////////////////////////////////////////////////////
//
// Checks
//
////////////////////////////////////////////////////////////

void ConnectionPort::Verifications()
{
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

	if (!AucunInstrumentBranche())
	{
		VerificationLectureMesures();
	}
}


void ConnectionPort::VerificationLectureMesures()
{
	if (r_present[5] == FALSE)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_CALORIMETRY);
		StrMessageWarning += message;
	}

	if (r_present[4] == FALSE)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_PRESSURE_LOW);
		StrMessageWarning += message;
	}

	if (r_present[3] == FALSE)
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

	for (size_t i = 0; i < sizeof(i_type)/sizeof(*r_present); i++)
	{
		if (i_type[i] != 0)
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