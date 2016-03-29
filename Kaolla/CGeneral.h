#pragma once

#include "SpinBtnCtrl.h"
#include "afxwin.h"

#include "Classes_experiences.h"
#include "afxcmn.h"

#include "Dialogue_experimentateur.h"
#include "Dialogue_gaz.h"

#include "XSBrowseFolder.h"
#include "shlwapi.h" // pour PathIsDirectory

#include "Parametres.h"

#include "ResourceOnglets.h"


////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CGeneral

class CGeneral : public CPropertyPage
{
	DECLARE_DYNAMIC(CGeneral)

public:
	CGeneral();
	virtual ~CGeneral();

// Données de boîte de dialogue
	enum { IDD = IDD_GENERAL };

private:


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV
	virtual BOOL OnCommand(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNomFichier;
	CString m_strChemin;
	CString m_strNomEchantillon;
	float m_fMasseEchantillon;
	std::string Date_Experience;
	virtual BOOL OnInitDialog();
	CSpinBtnCtrl pSpinMasseEchantillon;

	int m_nTemperatureExperience;
	CSpinBtnCtrl pSpinTemperatureExperience;

	Donnees_General donnees_actuelles;

	

	vector<gaz> list_gaz;
	gaz gaz_exp;

	vector<experimentateur> list_experimentateur;
	experimentateur experimentateur_exp;

	CDialogueExperimentateur m_DialogueExperimentateur;
	CDialogueGaz m_DialogueGaz;

	CEdit m_EditCommentaires;
	CString m_StrCommentaires;

	CComboBox m_CBExperimentateur;
	int m_nIndexExperimentateur;
	CComboBox m_CBGaz;
	int m_nIndexGaz;

	afx_msg void OnCbnSelchangeComboExperimentateur();
	afx_msg void OnCbnSelchangeComboGaz();
	afx_msg void OnEnChangeNomEchantillon();
	void RajoutSymboleCombo(CString StrSymbole);
	virtual BOOL OnApply();

	CString StrCalo,StrSurnom,StrEchantillon,StrGaz,StrDate;

	void Enregistrement_donnees_actuelles();

	afx_msg void OnBnClickedButtonExperimentateur();

	afx_msg void OnBnClickedButtonGaz();
	void ActualiserDate(void);
	void Reinitialisation(void);
	virtual void OnCancel();
	virtual void OnOK();
	afx_msg void OnBnClickedParcourirDossier();
};
