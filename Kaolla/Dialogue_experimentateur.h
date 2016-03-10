#pragma once
#include "afxwin.h"


#include "ParserXML.h"

////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CAjoutExperimentateur

class CAjoutExperimentateur : public CPropertyPage
{
	DECLARE_DYNAMIC(CAjoutExperimentateur)

public:
	CAjoutExperimentateur();
	virtual ~CAjoutExperimentateur();

// Données de boîte de dialogue
	enum { IDD = IDD_AJOUT_EXPERIMENTATEUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditNomAjoutExperimentateur;
	CString m_strNomAjoutExperimentateur;
	CEdit m_EditSurnomAjoutExperimentateur;
	CString m_strSurnomAjoutExperimentateur;
	CEdit m_EditAjoutExperimentateur;
	CString m_strAjoutExperimentateur;

	afx_msg void OnBnClickedAjouter();
};







//////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CModifExperimentateur

class CModifExperimentateur : public CPropertyPage
{
	DECLARE_DYNAMIC(CModifExperimentateur)

public:
	CModifExperimentateur();
	virtual ~CModifExperimentateur();

// Données de boîte de dialogue
	enum { IDD = IDD_MODIF_EXPERIMENTATEUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBModifExperimentateur;
	int m_nIndexModifExperimentateur;
	CEdit m_EditNomModifExperimentateur;
	CString m_strNomModifExperimentateur;
	CEdit m_EditSurnomModifExperimentateur;
	CString m_strSurnomModifExperimentateur;
	CEdit m_EditModifExperimentateur;
	CString m_strModifExperimentateur;

	vector<experimentateur> list_modif_experimentateur;

	afx_msg void OnBnClickedModifier();
	afx_msg void OnCbnSelchangeComboModifExperimentateur();
	virtual BOOL OnSetActive();
	void ReinitialisationComboBox();
};








////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CSupprExperimentateur

class CSupprExperimentateur : public CPropertyPage
{
	DECLARE_DYNAMIC(CSupprExperimentateur)

public:
	CSupprExperimentateur();
	virtual ~CSupprExperimentateur();

// Données de boîte de dialogue
	enum { IDD = IDD_SUPPR_EXPERIMENTATEUR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBSupprExperimentateur;
	int m_IndexSupprExperimentateur;
	CEdit m_EditSupprExperimentateur;
	CString m_MessageSupprExperimentateur;

	vector<experimentateur> list_suppr_experimentateur;

	afx_msg void OnCbnSelchangeComboSupprExperimentateur();
	afx_msg void OnBnClickedSupprimer();
	virtual BOOL OnSetActive();
	void ReinitialisationComboBox();
};






/////////////////////////////////////////////////////////////////////////////////////
//
// CDialogueExperimentateur

class CDialogueExperimentateur : public CPropertySheet
{
	DECLARE_DYNAMIC(CDialogueExperimentateur)

public:
	CAjoutExperimentateur m_AjoutExperimentateur;
	CModifExperimentateur m_ModifExperimentateur;
	CSupprExperimentateur m_SupprExperimentateur;

public:
	CDialogueExperimentateur(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDialogueExperimentateur(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDialogueExperimentateur();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};



