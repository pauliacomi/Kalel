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
	
	, settings{ nullptr }
	, modified{ false }
{
}

void ConnectionPort::PassSettings(MachineSettings * machineSettings)
{
	settings = machineSettings;

	// Instruments
	for (auto i : settings->instruments)
	{
		instruments[i.first - 1] = i.second.type;
		ports[i.first - 1] = i.second.port;
	}

	// Readers
	for (auto i:  settings->readers)
	{
		switch (i.second.type)
		{
		case READER_TEMPERATURE:
			switch (i.second.identifier)
			{
			case TEMPERATURE_CALO:
				readers				[0] = TRUE;
				sensitivities		[0] = i.second.sensitivity;
				channels			[0] = i.second.channel;
				instrument_related	[0] = i.second.instrument;
				break;
			case TEMPERATURE_CAGE:
				readers				[1] = TRUE;
				sensitivities		[1] = i.second.sensitivity;
				channels			[1] = i.second.channel;
				instrument_related  [1] = i.second.instrument;
				break;
			case TEMPERATURE_ROOM:
				readers				[2] = TRUE;
				sensitivities		[2] = i.second.sensitivity;
				channels			[2] = i.second.channel;
				instrument_related  [2] = i.second.instrument;
				break;
			default:
				break;
			}
			break;
		case READER_PRESSURE:
			switch (i.second.identifier)
			{
			case PRESSURE_HP:
				readers				[3] = TRUE;
				sensitivities		[3] = i.second.sensitivity;
				channels			[3] = i.second.channel;
				instrument_related  [3] = i.second.instrument;
				break;
			case PRESSURE_LP:
				readers				[4] = TRUE;
				sensitivities		[4] = i.second.sensitivity;
				channels			[4] = i.second.channel;
				instrument_related  [4] = i.second.instrument;
				break;
			default:
				break;
			}
			break;
		case READER_CALO:
			switch (i.second.identifier)
			{
			case CALO:
				readers				[5] = TRUE;
				sensitivities		[5] = i.second.sensitivity;
				channels			[5] = i.second.channel;
				instrument_related  [5] = i.second.instrument;
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}

	// Controllers
	for (auto i : settings->controllers)
	{
		switch (i.second.type)
		{
		case CONTROLLER_VALVES:
			valvecontroller = i.second.instrument;
		default:
			break;
		}
	}	
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

	// Checkboxes
	DDX_Control(pDX, IDC_CHECK_T1, m_CheckTemperature1);
	DDX_Control(pDX, IDC_CHECK_T2, m_CheckTemperature2);
	DDX_Control(pDX, IDC_CHECK_T3, m_CheckTemperature3);
	DDX_Control(pDX, IDC_CHECK_P1, m_CheckPressure1);
	DDX_Control(pDX, IDC_CHECK_P2, m_CheckPressure2);
	DDX_Control(pDX, IDC_CHECK_C1, m_CheckCalorimeter1);

	DDX_Check(pDX, IDC_CHECK_T1, readers[0]);
	DDX_Check(pDX, IDC_CHECK_T2, readers[1]);
	DDX_Check(pDX, IDC_CHECK_T3, readers[2]);
	DDX_Check(pDX, IDC_CHECK_P1, readers[3]);
	DDX_Check(pDX, IDC_CHECK_P2, readers[4]);
	DDX_Check(pDX, IDC_CHECK_C1, readers[5]);

	// Sensitivity

	DDX_Text(pDX, IDC_EDIT_T1, sensitivities[0]);
	DDX_Text(pDX, IDC_EDIT_T2, sensitivities[1]);
	DDX_Text(pDX, IDC_EDIT_T3, sensitivities[2]);
	DDX_Text(pDX, IDC_EDIT_P1, sensitivities[3]);
	DDX_Text(pDX, IDC_EDIT_P2, sensitivities[4]);
	DDX_Text(pDX, IDC_EDIT_C1, sensitivities[5]);

	// Channel
	DDX_Control(pDX, IDC_COMBO_CHANNEL_T1, m_CBPortInstrumentT1);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_T2, m_CBPortInstrumentT2);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_T3, m_CBPortInstrumentT3);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_P1, m_CBPortInstrumentP1);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_P2, m_CBPortInstrumentP2);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_C1, m_CBPortInstrumentC1);

	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T1, channels[0]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T2, channels[1]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T3, channels[2]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_P1, channels[3]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_P2, channels[4]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_C1, channels[5]);

	// Channel
	DDX_Control(pDX, IDC_COMBO_CHANNEL_T1, m_CBPortInstrumentT1);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_T2, m_CBPortInstrumentT2);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_T3, m_CBPortInstrumentT3);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_P1, m_CBPortInstrumentP1);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_P2, m_CBPortInstrumentP2);
	DDX_Control(pDX, IDC_COMBO_CHANNEL_C1, m_CBPortInstrumentC1);

	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T1, channels[0]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T2, channels[1]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_T3, channels[2]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_P1, channels[3]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_P2, channels[4]);
	DDX_CBIndex(pDX, IDC_COMBO_CHANNEL_C1, channels[5]);

	// Instrument number
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_T1, m_CBInstrumentT1);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_T2, m_CBInstrumentT2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_T3, m_CBInstrumentT3);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_P1, m_CBInstrumentP1);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_P2, m_CBInstrumentP2);
	DDX_Control(pDX, IDC_COMBO_INSTRUMENT_C1, m_CBInstrumentC1);
							  
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T1, instrument_related[0]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T2, instrument_related[1]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_T3, instrument_related[2]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_P1, instrument_related[3]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_P2, instrument_related[4]);
	DDX_CBIndex(pDX, IDC_COMBO_INSTRUMENT_C1, instrument_related[5]);

	//******************************
	// Instrument name
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT_1, m_CBTypeInstrument1);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT_2, m_CBTypeInstrument2);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT_3, m_CBTypeInstrument3);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT_4, m_CBTypeInstrument4);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT_5, m_CBTypeInstrument5);
	DDX_Control(pDX, IDC_COMBO_TYPE_INSTRUMENT_6, m_CBTypeInstrument6);

	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_1, instruments[0]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_2, instruments[1]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_3, instruments[2]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_4, instruments[3]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_5, instruments[4]);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE_INSTRUMENT_6, instruments[5]);

	// Port
	DDX_Control(pDX, IDC_PORT_INSTRUMENT_1, m_CBPortInstrumentT1);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT_2, m_CBPortInstrumentT2);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT_3, m_CBPortInstrumentT3);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT_4, m_CBPortInstrumentP1);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT_5, m_CBPortInstrumentP2);
	DDX_Control(pDX, IDC_PORT_INSTRUMENT_6, m_CBPortInstrumentC1);
	
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_1, ports[0]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_2, ports[1]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_3, ports[2]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_4, ports[3]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_5, ports[4]);
	DDX_CBIndex(pDX, IDC_PORT_INSTRUMENT_6, ports[5]);
	
	//***************************************************
	// Controllers - currently just falves
	DDX_Control(pDX, IDC_PORT_VANNES, m_CBPortValves);
	DDX_CBIndex(pDX, IDC_PORT_VANNES, valvecontroller);
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
	ON_CONTROL_RANGE(CBN_SELCHANGE, IDC_PORT_VANNES, IDC_COMBO_TYPE_INSTRUMENT_6, &ConnectionPort::OnModified)
	// Edit boxes
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT_T1, IDC_EDIT_C1, &ConnectionPort::OnModified)
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
			localSettings = std::make_shared<MachineSettings>(*settings);
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
			localSettings = std::make_shared<MachineSettings>(*settings);
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
	// Save instruments
	for (size_t key = 0; key < sizeof(instruments)/sizeof(*instruments); key++)
	{
		Instrument i;
		i.type = instruments[key];
		i.port = ports[key];
		newSettings.AddInstrument(i, key + 1);
	}

	// Save readers
	for (size_t key = 0; key < sizeof(readers) / sizeof(*readers); key++)
	{
		Reader r;

		switch (key)
		{
		case 0:
			r.type = READER_TEMPERATURE;
			r.identifier = TEMPERATURE_CALO;
			break;
		case 1:
			r.type = READER_TEMPERATURE;
			r.identifier = TEMPERATURE_CAGE;
			break;
		case 2:
			r.type = READER_TEMPERATURE;
			r.identifier = TEMPERATURE_ROOM;
			break;
		case 3:
			r.type = READER_PRESSURE;
			r.identifier = PRESSURE_HP;
			break;
		case 4:
			r.type = READER_PRESSURE;
			r.identifier = PRESSURE_LP;
			break;
		case 5:
			r.type = READER_CALO;
			r.identifier = CALO;
			break;
		default:
			break;
		}
		r.channel = channels[key];
		r.sensitivity = sensitivities[key];
		r.instrument = instrument_related[key];
		
		newSettings.AddReader(r, key + 1);
	}

	// Save controllers
	Controller c;
	c.type = CONTROLLER_VALVES;
	c.identifier = ID_CONTROLLER_VALVES;
	c.channel = 1;
	c.instrument = valvecontroller;

	newSettings.AddController(c, 1);
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
	if (readers[5] == FALSE)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_CALORIMETRY);
		StrMessageWarning += message;
	}

	if (readers[4] == FALSE)
	{
		bWarning = TRUE;
		CString message;
		message.Format(ERROR_NO_INSTRUMENT_PRESSURE_LOW);
		StrMessageWarning += message;
	}

	if (readers[3] == FALSE)
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

	for (size_t i = 0; i < sizeof(instruments)/sizeof(*readers); i++)
	{
		if (instruments[i] != 0)
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