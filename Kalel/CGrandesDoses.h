#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"


#include "Define.h"

////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CGrandesDoses

class CGrandesDoses : public CPropertyPage
{
	DECLARE_DYNAMIC(CGrandesDoses)

public:
	CGrandesDoses();
	virtual ~CGrandesDoses();

// Données de boîte de dialogue
	enum { IDD = IDD_GRANDES_DOSES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public :
	CButton m_CheckGrandesDoses;
	BOOL m_bGrandesDoses;

	void Reinitialisation();
	afx_msg void OnBnClickedCheckGrandesDoses();
	virtual BOOL OnInitDialog();
	void EnableGrandesDoses(BOOL active);
	float m_fDeltaPressionGrandesDoses;
	CSpinBtnCtrl m_SpinDeltaPressionGrandesDoses;
	int m_nTempsVolumeGrandesDoses;
	CSpinBtnCtrl m_SpinTempsVolumeGrandesDoses;
	int m_nTempsAdsorptionGrandesDoses;
	CSpinBtnCtrl m_SpinTempsAdsorptionGrandesDoses;
	float m_fPressionFinaleGrandesDoses;
	CSpinBtnCtrl m_SpinPressionFinaleGrandesDoses;

	Donnees_Grandes_Doses donnees_actuelles;

	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	void Enregistrement_donnees_actuelles();

	void GriserAEffectuer();
	void DegriserAEffectuer();

	void GriserCheck_GrandesDoses();
	void DegriserCheck_GrandesDoses();
	void ActionCheck_GrandesDoses();

	bool Check_GrandesDoses;
};