#ifndef CONNECTIONPORT_H
#define CONNECTIONPORT_H
#pragma once

#include "afxwin.h"
#include "ResourceConnectionPort.h"

#include "../Kalel Shared/Resources/DefineInstruments.h"

#include <vector>
#include <memory>

#define NUMBER_OF_READERS 6
#define NUMBER_OF_INSTRUMENTS 6

// Boîte de dialogue ConnectionPort

class MachineSettings;

class ConnectionPort : public CDialog
{
	DECLARE_DYNAMIC(ConnectionPort)

public:
	ConnectionPort(CWnd* pParent = NULL);   // constructeur standard
	virtual ~ConnectionPort();

// Données de boîte de dialogue
	enum { IDD = IDD_CONNECTION_PORT };

	// Pass the settings to display and be checked against changes
	void PassSettings(MachineSettings* machineSettings);

	// Returns whether the user modified any data
	bool Changed();

	// Returns the new machine settings
	std::shared_ptr<MachineSettings> GetSettings();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	DECLARE_MESSAGE_MAP()


	MachineSettings * settings;							// Local pointer to machine settings
	std::shared_ptr<MachineSettings> localSettings;		// Settings being built right here

	// --- Variables ---
	BOOL readers[NUMBER_OF_READERS];
	double sensitivities[NUMBER_OF_READERS];
	int channels[NUMBER_OF_READERS];
	int instrument_related[NUMBER_OF_READERS];

	int instruments[NUMBER_OF_INSTRUMENTS];
	int ports[NUMBER_OF_INSTRUMENTS];

	int valvecontroller;

	BOOL bPbm;
	BOOL bWarning;
	BOOL bPbmCalo;
	BOOL bPbmHP;
	BOOL bPbmBP;

	CString StrPbmCalo; 
	CString StrPbmBP; 
	CString StrPbmHP; 
	CString StrMessageErreur;
	CString StrMessageWarning;

	// --- Contrôles ---

	// Buttons that represent the checkboxes
	CButton m_CheckTemperature1;
	CButton m_CheckTemperature2;
	CButton m_CheckTemperature3;
	CButton m_CheckPressure1;
	CButton m_CheckPressure2;
	CButton m_CheckCalorimeter1;


	// Combo boxes for instrument channels
	CComboBox m_CBChannelInstrumentT1;
	CComboBox m_CBChannelInstrumentT2;
	CComboBox m_CBChannelInstrumentT3;
	CComboBox m_CBChannelInstrumentP1;
	CComboBox m_CBChannelInstrumentP2;
	CComboBox m_CBChannelInstrumentC1;

	// Related instrument
	CComboBox m_CBInstrumentT1;
	CComboBox m_CBInstrumentT2;
	CComboBox m_CBInstrumentT3;
	CComboBox m_CBInstrumentP1;
	CComboBox m_CBInstrumentP2;
	CComboBox m_CBInstrumentC1;

	/////////////////////////////////
	// Combo boxes for instrument types
	CComboBox m_CBTypeInstrument1;
	CComboBox m_CBTypeInstrument2;
	CComboBox m_CBTypeInstrument3;
	CComboBox m_CBTypeInstrument4;
	CComboBox m_CBTypeInstrument5;
	CComboBox m_CBTypeInstrument6;

	// Combo boxes for instrument ports
	CComboBox m_CBPortInstrumentT1;
	CComboBox m_CBPortInstrumentT2;
	CComboBox m_CBPortInstrumentT3;
	CComboBox m_CBPortInstrumentP1;
	CComboBox m_CBPortInstrumentP2;
	CComboBox m_CBPortInstrumentC1;


	CComboBox m_CBPortValves;


	// ---- Fonctions ---

	// Connection_port
protected:
	bool modified;
	void OnModified(UINT nID);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	void SaveModifications(MachineSettings & newSettings);

	void Verifications();
	void VerificationLectureMesures();
	bool AucunInstrumentBranche();
};

#endif