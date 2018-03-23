#ifndef DIALOGMACHINEPARAMETERS_H
#define DIALOGMACHINEPARAMETERS_H
#pragma once

#include "afxwin.h"
#include "ResourceParametresAppareil.h"

#include "SpinBtnCtrl.h"

#include <memory>

class MachineSettings;

// Dialogbox ApparatusParameters

class ApparatusParameters : public CDialog
{
	DECLARE_DYNAMIC(ApparatusParameters)

public:
	ApparatusParameters(CWnd* pParent = NULL);   // standard constructor
	virtual ~ApparatusParameters();

// Données de boîte de dialogue
	enum { IDD = IDD_PARAMETRES_APPAREIL };

	// Pass the settings to display and be checked against changes
	void PassSettings(const MachineSettings &machineSettings);

	// Returns whether the user modified any data
	bool Changed();

	// Returns the new machine settings
	MachineSettings * GetSettings();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	DECLARE_MESSAGE_MAP()

	bool modified = false;
	std::unique_ptr<MachineSettings> localSettings;

	BOOL b_safety;
	BOOL b_nozzle;

	CString m_StrNomCalo{""};
	CString m_StrEnteteFichier{""};

	CSpinBtnCtrl m_SpinVacuumPump;
	CSpinBtnCtrl m_SpinVacuumLimit;

	CSpinBtnCtrl m_SpinVolumeRef;
	CSpinBtnCtrl m_SpinVolumeP6;

	CSpinBtnCtrl m_SpinInjection[3];

	CSpinBtnCtrl m_SpinTime[9];

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	void OnModified(UINT nID);

public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};

#endif