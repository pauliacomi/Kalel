#pragma once
#ifndef TABDOSES_H
#define TABDOSES_H


#include "ResourceDialogExperimentSettings.h"	// For resources
#include "SpinBtnCtrl.h"			// For the Spin Button class
#include "Classes_experiences.h"	// Definitions for all classes used in this file


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
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

// Variables
protected:
	CString m_caption;	// Title of the page

	HANDLE h_parent;	// Handle to propertysheet

	BOOL m_bDoses;
	bool checkDoses;
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

	
	// Functions
public:
	void Reinitialisation();
	void Rename(int number);
	void GreyOut(BOOL active);

protected:
	void WriteData();
	void ActionCheck_Doses();

// Message Handlers
protected:
	void DeletePage();
};

#endif // !TABDOSES_H
