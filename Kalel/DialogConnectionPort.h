#ifndef CONNECTIONPORT_H
#define CONNECTIONPORT_H
#pragma once

#include "afxwin.h"
#include "ResourceConnectionPort.h"

#include "ConnectionMesure.h"
#include "VerifInstrument.h"
#include "Resources/DefineInstruments.h"

#include <vector>
#include <memory>

// Bo�te de dialogue ConnectionPort

class MachineSettings;

class ConnectionPort : public CDialog
{
	DECLARE_DYNAMIC(ConnectionPort)

public:
	ConnectionPort(CWnd* pParent = NULL);   // constructeur standard
	virtual ~ConnectionPort();

// Donn�es de bo�te de dialogue
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

	MachineSettings * settings;
	std::shared_ptr<MachineSettings> localSettings;

	// --- Variables ---
	std::vector<int> m_nIndexTypeInstrument;
	std::vector<int> m_nPortInstrument;
	std::vector<int> m_nIndexInstrumentKeithleyVoie2;
	std::vector<int> m_nInstrumentMensor;
	std::vector<BOOL> m_bInstrumentKeithleyVoie1;
	std::vector<BOOL> m_bInstrumentKeithleyVoie2;

	int m_nIndexPortVannes;
	int m_nIndexPortTemperatures;

	VerifInstrument verifInstrument[NB_OF_INSTRUMENTS];
	ConnectionMesure CM_Calo,CM_HP,CM_BP;

	int index_verifInstrument;

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

	// --- Contr�les ---
	std::vector<CComboBox*> m_CBTypeInstrument;
	std::vector<CComboBox*> m_CBPortInstrument;
	std::vector<CButton*> m_CheckInstrumentKeithleyVoie1;
	std::vector<CButton*> m_CheckInstrumentKeithleyVoie2;
	std::vector<CComboBox*> m_CBInstrumentKeithleyVoie2;
	std::vector<CComboBox*> m_CBInstrumentMensor;

	CComboBox m_CBTypeInstrument1;
	CComboBox m_CBTypeInstrument2;
	CComboBox m_CBTypeInstrument3;

	CComboBox m_CBPortInstrument1;
	CComboBox m_CBPortInstrument2;
	CComboBox m_CBPortInstrument3;

	CButton m_CheckInstrument1KeithleyVoie1;
	CButton m_CheckInstrument2KeithleyVoie1;
	CButton m_CheckInstrument3KeithleyVoie1;

	CButton m_CheckInstrument1KeithleyVoie2;
	CButton m_CheckInstrument2KeithleyVoie2;
	CButton m_CheckInstrument3KeithleyVoie2;

	CComboBox m_CBInstrument1KeithleyVoie2;
	CComboBox m_CBInstrument2KeithleyVoie2;
	CComboBox m_CBInstrument3KeithleyVoie2;

	CComboBox m_CBInstrumentMensor1;
	CComboBox m_CBInstrumentMensor2;
	CComboBox m_CBInstrumentMensor3;


	CComboBox m_CBPortVannes;
	CComboBox m_CBPortTemperatures;


	// ---- Fonctions ---

	// Connection_port
protected:
	bool modified;
	void OnModified(UINT nID);

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboTypeInstrument1();
	afx_msg void OnCbnSelchangeComboTypeInstrument2();
	afx_msg void OnCbnSelchangeComboTypeInstrument3();
	void EnregistrementConnection_port(MachineSettings & newSettings);
	void InitConnection_port();


	void InitDonneesInstrument(int TypeInstr,int* m_nIndex,int* m_nPortInstrument,BOOL* m_bInstrumentKeithleyVoie1,BOOL* m_bInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentKeithleyVoie2,int* m_nIndexInstrumentKeithleyVoie2,int* m_nInstrumentMensor,int GetPort,int GetFonction);
	void EnregistrementParametresInstrument(int num_instr,int m_nIndex,int COMInstrument,bool m_bInstrumentKeithleyVoie1, bool m_bInstrumentKeithleyVoie2,int m_nIndexInstrumentKeithleyVoie2,int m_nInstrumentMensor, MachineSettings & newSettings);
	void ShowItem(int m_nIndex,bool m_bInstrumentKeithleyVoie2,CComboBox* m_CBPortInstrument,CButton* m_CheckInstrumentKeithleyVoie1,CButton* m_CheckInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentMensor);

	void Verifications();

	void VerificationPortUSB();
	void VerificationPortCOM();
	void VerificationLectureMesures();
	bool AucunInstrumentBranche();

	void VerifUnInstrument(int num_instr,int m_nIndex,BOOL m_bKeithleyVoie1,BOOL m_bKeithleyVoie2,int m_nInstrumentKeithleyVoie2,int m_nMensor,int PortInstr);
	void VerifMesure(ConnectionMesure* CM,int num_instr,int voie_mesure,CString* StrPbm,BOOL* bPbmMesure);
	void EnregistrementVerifications(MachineSettings& newSettings);

};

#endif