#pragma once
#ifndef TABDIVERS_H
#define TABDIVERS_H

#include "ResourceDialogExperimentSettings.h"		// For resources
#include "SpinBtnCtrl.h"			// For the Spin Button class
#include "Classes_experiences.h"	// Definitions for all classes used in this file
#include <vector>					// Using the vector class

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
	virtual BOOL OnCommand(WPARAM wParam,LPARAM lParam);
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
	vector<cellule> cellList;
	cellule cellExp;

public:
	// All the settings are stored here
	Donnees_Divers allSettings;

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