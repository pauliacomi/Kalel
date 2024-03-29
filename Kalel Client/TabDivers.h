#ifndef TABDIVERS_H
#define TABDIVERS_H
#pragma once

#include "ResourceDialogExperimentSettings.h"								// For resources
#include "SpinBtnCtrl.h"													// For the Spin Button class
#include "../Kalel Shared/Com Classes/ExperimentHelperClasses.h"							// Definitions for all classes used in this file
#include <vector>															// Using the vector class


class TabDivers : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(TabDivers)

public:
	TabDivers();
	virtual ~TabDivers();

// Dialog Data
	enum { IDD = IDD_DIVERS};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

// Variables
protected:
	BOOL m_bMiseSousVide;

	int m_IndexCellule;
	int m_nTempsLigneBase;
	int m_nTempsVide;

	double m_fVolumeTotal;
	double m_fVolumeCalo;

	CComboBox m_CBCellule;
	
	CButton m_CheckMiseSousVide;
	CButton m_ButtonCellule;

	CEdit m_EditVolumeTotal;
	CEdit m_EditVolumeCalo;
	CEdit m_EditTempsVide;

	CSpinBtnCtrl m_SpinTempsLigneBase;
	CSpinBtnCtrl m_SpinTempsVide;

	// Definintions for cells
	std::vector<cell> cellList;
	cell cellExp;

public:
	// All the settings are stored here
	data_other allSettings;

// Functions
public:
	void Reinitialisation();

protected:
	void WriteData();
	void EnableMiseSousVide(BOOL active);

// Message Handlers
protected:
	void OnCbnSelchangeComboCellule();
	void OnBnClickedCheckMiseSousVide();
	void OnBnClickedButtonCellule();
};

#endif