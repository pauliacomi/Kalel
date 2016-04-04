#pragma once
#ifndef TABDESORPTION_H
#define TABDESORPTION_H


#include "ResourceOngletsEtapes.h"	// For resources
#include "SpinBtnCtrl.h"			// For the Spin Button class
#include "Classes_experiences.h"	// Definitions for all classes used in this file


class TabDesorption : public CMFCPropertyPage
{
	DECLARE_DYNAMIC(TabDesorption)

public:
	TabDesorption();
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

	bool checkDesorption;

	BOOL m_bDesorption;
	BOOL m_bDerniereEtapeDesorption;

	int m_nTempsDesorption;
	int m_nTempsVolumeDesorption;
	float m_fDeltaPressionDesorption;
	float m_fPressionFinaleDesorption;

	CButton m_CheckDesorption;

	CSpinBtnCtrl m_SpinDeltaPressionDesorption;
	CSpinBtnCtrl m_SpinTempsDesorption;
	CSpinBtnCtrl m_SpinPressionFinaleDesorption;
	CSpinBtnCtrl m_SpinTempsVolumeDesorption;

	Donnees_Desorption allSettings;

// Functions
public:
	void Reinitialisation();

protected:
	void WriteData();
	void EnableDesorption(BOOL active);

	void GreyOut();
	void UnGreyOut();
	void CheckGreyOut();
	void CheckUnGreyOut();
	void ActionCheck_Desorption();

// Message Handlers
protected:
	void OnBnClickedCheckDesorption();
};

#endif // !TABDESORPTION_H




