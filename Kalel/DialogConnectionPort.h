#pragma once
#include "afxwin.h"
#include "ResourceConnectionPort.h"

#include "ConnectionMesure.h"
#include "VerifInstrument.h"
#include "DefineInstruments.h"

// Boîte de dialogue ConnectionPort

class ConnectionPort : public CDialog
{
	DECLARE_DYNAMIC(ConnectionPort)

public:
	ConnectionPort(CWnd* pParent = NULL);   // constructeur standard
	virtual ~ConnectionPort();

// Données de boîte de dialogue
	enum { IDD = IDD_CONNECTION_PORT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	// --- Variables ---

	int m_nIndexTypeInstrument1;
	int m_nIndexTypeInstrument2;
	int m_nIndexTypeInstrument3;

	int m_nPortInstrument1;
	int m_nPortInstrument2;
	int m_nPortInstrument3;

	BOOL m_bInstrument1KeithleyVoie1;			// Used to be BOOL
	BOOL m_bInstrument1KeithleyVoie2;
	BOOL m_bInstrument2KeithleyVoie1;
	BOOL m_bInstrument2KeithleyVoie2;
	BOOL m_bInstrument3KeithleyVoie1;
	BOOL m_bInstrument3KeithleyVoie2;

	int m_nIndexInstrument1KeithleyVoie2;
	int m_nIndexInstrument2KeithleyVoie2;
	int m_nIndexInstrument3KeithleyVoie2;

	int m_nInstrument1Mensor;
	int m_nInstrument2Mensor;
	int m_nInstrument3Mensor;

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


	// --- Contrôles ---

	CComboBox m_CBTypeInstrument1;
	CComboBox m_CBTypeInstrument2;
	CComboBox m_CBTypeInstrument3;

	CComboBox m_CBPortInstrument1;
	CComboBox m_CBPortInstrument2;
	CComboBox m_CBPortInstrument3;

	CButton m_CheckInstrument1KeithleyVoie1;
	CButton m_CheckInstrument1KeithleyVoie2;
	CButton m_CheckInstrument2KeithleyVoie1;
	CButton m_CheckInstrument2KeithleyVoie2;
	CButton m_CheckInstrument3KeithleyVoie1;
	CButton m_CheckInstrument3KeithleyVoie2;

	CComboBox m_CBInstrument1KeithleyVoie2;
	CComboBox m_CBInstrument2KeithleyVoie2;
	CComboBox m_CBInstrument3KeithleyVoie2;

	CComboBox m_CBInstrument1Mensor;
	CComboBox m_CBInstrument2Mensor;
	CComboBox m_CBInstrument3Mensor;

	CComboBox m_CBPortVannes;
	CComboBox m_CBPortTemperatures;


	// ---- Fonctions ---

	// Connection_port

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboTypeInstrument1();
	afx_msg void OnCbnSelchangeComboTypeInstrument2();
	afx_msg void OnCbnSelchangeComboTypeInstrument3();
	afx_msg void OnBnClickedCheckInstrument1KeithleyVoie2();
	afx_msg void OnBnClickedCheckInstrument2KeithleyVoie2();
	afx_msg void OnBnClickedCheckInstrument3KeithleyVoie2();
	void EnregistrementConnection_port();
	void InitConnection_port();


	// Connection_port_fct

	void InitDonneesInstrument(int TypeInstr,int* m_nIndex,int* m_nPortInstrument,BOOL* m_bInstrumentKeithleyVoie1,BOOL* m_bInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentKeithleyVoie2,int* m_nIndexInstrumentKeithleyVoie2,int* m_nInstrumentMensor,int GetPort,int GetFonction);
	void EnregistrementParametresInstrument(int num_instr,int m_nIndex,int COMInstrument,bool m_bInstrumentKeithleyVoie1, bool m_bInstrumentKeithleyVoie2,int m_nIndexInstrumentKeithleyVoie2,int m_nInstrumentMensor);
	void ShowItem(int m_nIndex,bool m_bInstrumentKeithleyVoie2,CComboBox* m_CBPortInstrument,CButton* m_CheckInstrumentKeithleyVoie1,CButton* m_CheckInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentKeithleyVoie2,CComboBox* m_CBInstrumentMensor);


	// Connection_port_Verifications

	void Verifications();

	void VerificationPortUSB();
	void VerificationPortCOM();
	void VerificationLectureMesures();
	bool AucunInstrumentBranche();

	void VerifUnInstrument(int num_instr,int m_nIndex,BOOL m_bKeithleyVoie1,BOOL m_bKeithleyVoie2,int m_nInstrumentKeithleyVoie2,int m_nMensor,int PortInstr);
	void VerifMesure(ConnectionMesure* CM,int num_instr,int voie_mesure,CString* StrPbm,BOOL* bPbmMesure);
	void EnregistrementVerifications();

};
