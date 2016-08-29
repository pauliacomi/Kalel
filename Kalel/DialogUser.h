#pragma once
#ifndef DIALOGUE_EXPERIMENTATEUR_H
#define DIALOGUE_EXPERIMENTATEUR_H

#include "afxwin.h"
#include "ParserXML.h"
#include "ResourceDialogUser.h"
#include <vector>



//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Add User Property Page------------------ 
				//------------------------------------------------------
				//
				//  For adding a user to the configuration file
				//
//////////////////////////////////////////////////////////////////////////////////////

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
				//------------------------------------------------------
				//--------------Modify User Property Page------------------ 
				//------------------------------------------------------
				//
				//  For modifying a user in the configuration file
				//
//////////////////////////////////////////////////////////////////////////////////////

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

	std::vector<experimentateur> list_modif_experimentateur;

	afx_msg void OnBnClickedModifier();
	afx_msg void OnCbnSelchangeComboModifExperimentateur();
	virtual BOOL OnSetActive();
	void ReinitialisationComboBox();
};






//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Delete User Property Page------------------ 
				//------------------------------------------------------
				//
				//  For deleting a user in the configuration file
				//
//////////////////////////////////////////////////////////////////////////////////////

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

	CEdit m_EditNomSupprExperimentateur;
	CString m_strNomSupprExperimentateur;
	CEdit m_EditSurnomSupprExperimentateur;
	CString m_strSurnomSupprExperimentateur;

	std::vector<experimentateur> list_suppr_experimentateur;

	afx_msg void OnCbnSelchangeComboSupprExperimentateur();
	afx_msg void OnBnClickedSupprimer();
	virtual BOOL OnSetActive();
	void ReinitialisationComboBox();
};





//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//-------------- User Property Sheet------------------ 
				//------------------------------------------------------
				//
				//  Where all the tabs will go
				//
//////////////////////////////////////////////////////////////////////////////////////



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

	virtual BOOL OnInitDialog();
};

#endif