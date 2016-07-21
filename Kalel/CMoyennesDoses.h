#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"


#include "Define.h"

////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CMoyennesDoses

class CMoyennesDoses : public CPropertyPage
{
	DECLARE_DYNAMIC(CMoyennesDoses)

public:
	CMoyennesDoses();
	virtual ~CMoyennesDoses();

// Données de boîte de dialogue
	enum { IDD = IDD_MOYENNES_DOSES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public :
	CButton m_CheckMoyennesDoses;
	BOOL m_bMoyennesDoses;

	void Reinitialisation();
	afx_msg void OnBnClickedCheckMoyennesDoses();
	virtual BOOL OnInitDialog();
	void EnableMoyennesDoses(BOOL active);
	float m_fDeltaPressionMoyennesDoses;
	CSpinBtnCtrl m_SpinDeltaPressionMoyennesDoses;
	int m_nTempsVolumeMoyennesDoses;
	CSpinBtnCtrl m_SpinTempsVolumeMoyennesDoses;
	int m_nTempsAdsorptionMoyennesDoses;
	CSpinBtnCtrl m_SpinTempsAdsorptionMoyennesDoses;
	float m_fPressionFinaleMoyennesDoses;
	CSpinBtnCtrl m_SpinPressionFinaleMoyennesDoses;

	Donnees_Moyennes_Doses donnees_actuelles;

	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	void Enregistrement_donnees_actuelles();

	void GriserAEffectuer();
	void DegriserAEffectuer();

	void GriserCheck_MoyennesDoses();
	void DegriserCheck_MoyennesDoses();
	void ActionCheck_MoyennesDoses();

	bool Check_MoyennesDoses;
};