#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"


#include "Define.h"
#include "ResourceOngletsEtapes.h"


////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CPetitesDoses

class CPetitesDoses : public CPropertyPage
{
	DECLARE_DYNAMIC(CPetitesDoses)

public:
	CPetitesDoses();
	virtual ~CPetitesDoses();

// Données de boîte de dialogue
	enum { IDD = IDD_PETITES_DOSES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	DECLARE_MESSAGE_MAP()
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	CButton m_CheckPetitesDoses;
	BOOL m_bPetitesDoses;
	void Reinitialisation();
	afx_msg void OnBnClickedCheckPetitesDoses();
	virtual BOOL OnInitDialog();
	void EnablePetitesDoses(BOOL active);
	float m_fDeltaPressionPetitesDoses;
	CSpinBtnCtrl m_SpinDeltaPressionPetitesDoses;
	int m_nTempsVolumePetitesDoses;
	CSpinBtnCtrl m_SpinTempsVolumePetitesDoses;
	int m_nTempsAdsorptionPetitesDoses;
	CSpinBtnCtrl m_SpinTempsAdsorptionPetitesDoses;
	float m_fPressionFinalePetitesDoses;
	CSpinBtnCtrl m_SpinPressionFinalePetitesDoses;

	Donnees_Petites_Doses donnees_actuelles;

	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	void Enregistrement_donnees_actuelles();

	/*virtual*/ void GriserAEffectuer();
	/*virtual*/ void DegriserAEffectuer();

	void GriserCheck_PetitesDoses();
	void DegriserCheck_PetitesDoses();
	void ActionCheck_PetitesDoses();

	bool Check_PetitesDoses;
};
