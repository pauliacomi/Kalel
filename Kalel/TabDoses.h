#pragma once
#ifndef TABDOSES_H
#define TABDOSES_H

#include "ResourceDialogExperimentSettings.h"								// For resources
#include "SpinBtnCtrl.h"													// For the Spin Button class
#include "../Kalel Shared/Com Classes/Classes_experiences.h"				// Definitions for all classes used in this file



class TabDoses : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(TabDoses)

public:
	TabDoses(int i);
	virtual ~TabDoses();

	// Dialog Data
	enum { IDD = IDD_DOSES };

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
	CString m_caption;	// Title of the page

	HANDLE h_parent;	// Handle to propertysheet

	int position;

	int m_nTimeAdsorptionDoses;
	int m_nTimeVolumeDoses;
	float m_fDeltaPressureDoses;
	float m_fFinalPressureDoses;

	CButton m_CheckDoses;

	CSpinBtnCtrl m_SpinDeltaPressureDoses;
	CSpinBtnCtrl m_SpinTimeVolumeDoses;
	CSpinBtnCtrl m_SpinTimeAdsorptionDoses;
	CSpinBtnCtrl m_SpinFinalPressureDoses;

public:
	Donnees_Doses allSettings;
	bool checkDoses;				// for greying out
	
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

#endif // !TABDOSES_H
