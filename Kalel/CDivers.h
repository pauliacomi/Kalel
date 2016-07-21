#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"

#include "Dialogue_cellule.h"

#include "ResourceOnglets.h"

/////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CDivers

class CDivers : public CPropertyPage
{
	DECLARE_DYNAMIC(CDivers)

public:
	CDivers();
	virtual ~CDivers();

// Données de boîte de dialogue
	enum { IDD = IDD_DIVERS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnCommand(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBCellule;
	int m_IndexCellule;
	CButton m_ButtonCellule;
	CEdit m_EditVolumeTotal;
	double m_fVolumeTotal;
	CEdit m_EditVolumeCalo;
	double m_fVolumeCalo;
	//CEdit m_EditTempsLigneBase;
	int m_nTempsLigneBase;
	CButton m_CheckMiseSousVide;
	/*CSpinButtonCtrl*/ CSpinBtnCtrl m_SpinTempsLigneBase;
	CEdit m_EditTempsVide;
	int m_nTempsVide;
	CSpinBtnCtrl m_SpinTempsVide;
	virtual BOOL OnInitDialog();

	Donnees_Divers donnees_actuelles;

	vector<cellule> list_cellule;
	cellule cellule_exp;

	CDialogueCellule m_DialogueCellule;

	void Reinitialisation();
	afx_msg void OnCbnSelchangeComboCellule();
	afx_msg void OnBnClickedCheckMiseSousVide();
	BOOL m_bMiseSousVide;
	void EnableMiseSousVide(BOOL active);
	afx_msg void OnBnClickedButtonCellule();
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	void Enregistrement_donnees_actuelles();
};

