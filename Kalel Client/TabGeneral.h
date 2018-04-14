#ifndef TABGENERAL_H
#define TABGENERAL_H
#pragma once

#include "ResourceDialogExperimentSettings.h"								// For resources
#include "SpinBtnCtrl.h"													// For the Spin Button class
#include "../Kalel Shared/Com Classes/ExperimentHelperClasses.h"			// Definitions for all classes used in this file
#include <vector>															// Using the vector class

class MachineSettings;

class TabGeneral : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(TabGeneral)

public:
	TabGeneral();
	virtual ~TabGeneral();

// Dialog Data
	enum { IDD = IDD_GENERAL };
	void PassSettings(const MachineSettings* machineSettings);

protected:
	const MachineSettings * settings;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

// Variables
protected:

	CString m_FileName;
	CString m_Path;
	CString m_SampleName;
	CString m_Comments;
	float m_SampleMass;
	float m_ExperimentTemperature;
	float m_ExperimentTemperatureMargin;
	CEdit m_EditComments;
	CSpinBtnCtrl m_SpinSampleMass;
	CSpinBtnCtrl m_SpinTemperatureExperiment;
	CSpinBtnCtrl m_SpinTemperatureMarginExperiment;
	CComboBox m_CBGas;
	int m_GasIndex;
	CComboBox m_CBUser;
	int m_UserIndex;

	// Strings for building the file name
	CString StrCalo;
	CString StrSurnom;
	CString StrEchantillon;
	CString StrGaz;
	CString StrDate;
	CString experimentDate;

	// Definitions for gasses 
	std::vector<gas> gasArray;
	gas gasExp;

	// Definitions for users 
	std::vector<user> userArray;
	user userExp;

	// All the settings are stored here
public:
	data_general allSettings;
	bool checkGeneral;			// for greying out

// Functions
public:
	void Reinitialisation(void);
	void GreyOut(BOOL active);
	void ToggleGreyOut();

protected:
	void UpdateDate(void);
	void WriteData();

// Message handlers
protected:
	void OnCbnSelchangeComboExperimentateur();
	void OnCbnSelchangeComboGaz();
	void OnEnChangeNomEchantillon();
	void OnBnClickedUserMenu();
	void OnBnClickedGasMenu();
	void OnBnClickedChangeFolder();
};


#endif // !TABGENERAL_H