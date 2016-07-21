#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"


#include "Define.h"

/////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CDesorption


class CDesorption : public CPropertyPage
{
	DECLARE_DYNAMIC(CDesorption)

public:
	CDesorption();
	virtual ~CDesorption();

// Données de boîte de dialogue
	enum { IDD = IDD_DESORPTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public :
	CButton m_CheckDesorption;
	BOOL m_bDesorption;

	void Reinitialisation();
	afx_msg void OnBnClickedCheckDesorption();
	virtual BOOL OnInitDialog();
	void EnableDesorption(BOOL active);
	float m_fDeltaPressionDesorption;
	CSpinBtnCtrl m_SpinDeltaPressionDesorption;
	int m_nTempsVolumeDesorption;
	CSpinBtnCtrl m_SpinTempsVolumeDesorption;
	int m_nTempsDesorption;
	CSpinBtnCtrl m_SpinTempsDesorption;
	float m_fPressionFinaleDesorption;
	CSpinBtnCtrl m_SpinPressionFinaleDesorption;

	Donnees_Desorption donnees_actuelles;

	BOOL m_bDerniereEtapeDesorption;
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();

	void Enregistrement_donnees_actuelles();

	void GriserAEffectuer();
	void DegriserAEffectuer();

	void GriserCheck_Desorption();
	void DegriserCheck_Desorption();
	void ActionCheck_Desorption();

	bool Check_Desorption;
};
