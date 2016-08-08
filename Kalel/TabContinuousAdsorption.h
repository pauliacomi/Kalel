#pragma once
#ifndef TABCONTINUOUSADSORPTION_H
#define TABCONTINUOUSADSORPTION_H


#include "ResourceDialogExperimentSettings.h"	// For resources
#include "SpinBtnCtrl.h"			// For the Spin Button class
#include "Classes_experiences.h"	// Definitions for all classes used in this file


class TabContinuousAdsorption : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(TabContinuousAdsorption)

public:
	TabContinuousAdsorption();
	virtual ~TabContinuousAdsorption();

	// Dialog Data
	enum { IDD = IDD_ADSORPTION_CONTINUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

	// Variables
protected:
	BOOL m_bAdsorptionContinue;

	int m_nTempsEtalonnageDebit;
	int m_nTempsEtalonnageVolumeInter;
	int m_nTempsEquilibreContinue;
	int m_nTempsFinalEquilibre;
	float m_fPressionFinaleAdsorptionContinue;

	CButton m_CheckAdsorptionContinue;

	CSpinBtnCtrl m_SpinTempsEtalonnageDebit;
	CSpinBtnCtrl m_SpinTempsEtalonnageVolumeInter;
	CSpinBtnCtrl m_SpinTempsEquilibreContinue;
	CSpinBtnCtrl m_SpinTempsFinalEquilibre;
	CSpinBtnCtrl m_SpinPressionFinaleAdsorptionContinue;

public:
	Donnees_Adsorption_Continue allSettings;

	// Functions
public:
	void Reinitialisation();

protected:
	void WriteData();
	void EnableAdsorptionContinue(void);

	// Message Handlers
protected:
	void OnBnClickedCheckAdsorptionContinue();
};


#endif // !TABCONTINUOUSADSORPTION_H