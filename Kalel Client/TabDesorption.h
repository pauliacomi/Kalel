#ifndef TABDESORPTION_H
#define TABDESORPTION_H
#pragma once

#include "ResourceDialogExperimentSettings.h"								// For resources
#include "SpinBtnCtrl.h"													// For the Spin Button class
#include "../Kalel Shared/Com Classes/ExperimentHelperClasses.h"			// Definitions for all classes used in this file


class TabDesorption : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(TabDesorption)

public:
	TabDesorption(int number);
	virtual ~TabDesorption();

	// Dialog Data
	enum { IDD = IDD_DESORPTION };

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
	CString m_caption;	// Title of the page

	int position;

	int m_nTempsDesorption;
	int m_nTempsVolumeDesorption;
	float m_fDeltaPressionDesorption;
	float m_fPressionFinaleDesorption;

	CButton m_CheckDesorption;

	CSpinBtnCtrl m_SpinDeltaPressionDesorption;
	CSpinBtnCtrl m_SpinTempsDesorption;
	CSpinBtnCtrl m_SpinPressionFinaleDesorption;
	CSpinBtnCtrl m_SpinTempsVolumeDesorption;

public:
	data_desorption allSettings;
	bool checkDesorption;			//For greying out

// Functions
public:
	void Reinitialisation();
	void Rename(int number);
	void GreyOut(BOOL active);
	void ToggleGreyOut();

protected:
	void WriteData();

// Message Handlers
protected:
	void DeletePage();
};

#endif // !TABDESORPTION_H




