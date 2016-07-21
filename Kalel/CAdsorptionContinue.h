#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"


#include "Define.h"

////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CAdsorptionContinue

class CAdsorptionContinue : public CPropertyPage
{
	DECLARE_DYNAMIC(CAdsorptionContinue)

public:
	CAdsorptionContinue();
	virtual ~CAdsorptionContinue();

// Données de boîte de dialogue
	enum { IDD = IDD_ADSORPTION_CONTINUE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CButton m_CheckAdsorptionContinue;
	BOOL m_bAdsorptionContinue;
	int m_nTempsEtalonnageDebit;
	CSpinBtnCtrl m_SpinTempsEtalonnageDebit;
	int m_nTempsEtalonnageVolumeInter;
	CSpinBtnCtrl m_SpinTempsEtalonnageVolumeInter;
	int m_nTempsEquilibreContinue;
	CSpinBtnCtrl m_SpinTempsEquilibreContinue;
	int m_nTempsFinalEquilibre;
	CSpinBtnCtrl m_SpinTempsFinalEquilibre;
	float m_fPressionFinaleAdsorptionContinue;
	CSpinBtnCtrl m_SpinPressionFinaleAdsorptionContinue;

	Donnees_Adsorption_Continue donnees_actuelles;

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	void Reinitialisation();
	afx_msg void OnBnClickedCheckAdsorptionContinue();
	void EnableAdsorptionContinue(void);
	virtual BOOL OnInitDialog();

	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();

	void Enregistrement_donnees_actuelles();
};
