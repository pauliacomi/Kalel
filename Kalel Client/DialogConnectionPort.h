#ifndef CONNECTIONPORT_H
#define CONNECTIONPORT_H
#pragma once

#include "afxwin.h"
#include "ResourceConnectionPort.h"

#include "../Kalel Shared/Resources/DefineInstruments.h"

#include <vector>
#include <memory>

#define NB_INSTRUMENTS 6
#define NB_READERS 6
#define NB_CONTROLLERS 11

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
	void PassSettings(const MachineSettings &machineSettings);

	// Returns whether the user modified any data
	bool Changed();

	// Returns the new machine settings
	MachineSettings * GetSettings();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	DECLARE_MESSAGE_MAP()

	std::unique_ptr<MachineSettings> localSettings;		// Settings being built right here

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

	// --- Variables ---
	// These are here because windows does not let anything else but 
	// int/BOOL etc to be bound to a control

	///////////////////////////////////
	// Instrument
	int i_type[NB_INSTRUMENTS];
	int i_ports[NB_INSTRUMENTS];

	///////////////////////////////////
	// Reader
	BOOL r_present[NB_READERS];
	int r_instrument[NB_READERS];
	int r_instrument_channel[NB_READERS];
	double r_sensitivity[NB_READERS];
	double r_safe_min[NB_READERS];
	double r_safe_max[NB_READERS];

	///////////////////////////////////
	// Controller
	BOOL c_present[NB_CONTROLLERS];
	int c_instrument[NB_CONTROLLERS];
	int c_instrument_channel[NB_CONTROLLERS];
	int c_instrument_subchannel[NB_CONTROLLERS];

	// ---- Fonctions ---

	// Connection_port
protected:
	bool modified = false;
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