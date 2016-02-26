#pragma once
#ifndef DIALOGUE_CELLULE_H
#define DIALOGUE_CELLULE_H

#include "afxwin.h"
#include "ParserXML.h"
#include "SpinBtnCtrl.h"


///////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CAjoutCellule

class CAjoutCellule : public CPropertyPage
{
	DECLARE_DYNAMIC(CAjoutCellule)

public:
	CAjoutCellule();
	virtual ~CAjoutCellule();

// Données de boîte de dialogue
	enum { IDD = IDD_AJOUT_CELLULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString m_StrAjoutNumeroCellule;
	//CSpinBtnCtrl m_SpinNumeroCellule;
	double m_fAjoutVolumeCellule;
	CSpinBtnCtrl m_SpinAjoutVolumeCellule;
	float m_fAjoutVolumeCalo;
	CSpinBtnCtrl m_SpinAjoutVolumeCalo;
	CEdit m_EditMessageAjoutCellule;
	CString m_strMessageAjoutCellule;

	afx_msg void OnBnClickedAjouter();
	virtual BOOL OnInitDialog();
};

#endif // !DIALOGUE_CELLULE_H






//////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CModifCellule

class CModifCellule : public CPropertyPage
{
	DECLARE_DYNAMIC(CModifCellule)

public:
	CModifCellule();
	virtual ~CModifCellule();

// Données de boîte de dialogue
	enum { IDD = IDD_MODIF_CELLULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBModifCellule;
	int m_nIndexModifCellule;
	CString m_StrModifNumeroCellule;
	//CSpinBtnCtrl m_SpinModifNumeroCellule;
	double m_fModifVolumeCellule;
	CSpinBtnCtrl m_SpinModifVolumeCellule;
	float m_fModifVolumeCalo;
	CSpinBtnCtrl m_SpinModifVolumeCalo;
	CEdit m_EditMessageModifCellule;
	CString m_strMessageModifCellule;

	vector<cellule> list_modif_cellule;

	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnCbnSelchangeComboModifCellule();
	afx_msg void OnBnClickedModifier();
	void ReinitialisationComboBox();
};








/////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CSupprCellule

class CSupprCellule : public CPropertyPage
{
	DECLARE_DYNAMIC(CSupprCellule)

public:
	CSupprCellule();
	virtual ~CSupprCellule();

// Données de boîte de dialogue
	enum { IDD = IDD_SUPPR_CELLULE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBSupprNumeroCellule;
	int m_nIndexSupprNumeroCellule;
	double m_fSupprVolumeCellule;
	CEdit m_EditSupprVolumeCellule;
	float m_fSupprVolumeCalo;
	CEdit m_EditSupprVolumeCalo;
	CEdit m_EditMessageSupprCellule;
	CString m_strMessageSupprCellule;

	vector<cellule> list_suppr_cellule;

	virtual BOOL OnSetActive();
	afx_msg void OnCbnSelchangeComboSupprNumeroCellule();
	afx_msg void OnBnClickedSupprimer();
	void ReinitialisationComboBox();
};








////////////////////////////////////////////////////////////////////////////////////////////
//
// CDialogueCellule

class CDialogueCellule : public CPropertySheet
{
	DECLARE_DYNAMIC(CDialogueCellule)

public:
	CAjoutCellule m_AjoutCellule;
	CModifCellule m_ModifCellule;
	CSupprCellule m_SupprCellule;

public:
	CDialogueCellule(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDialogueCellule(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDialogueCellule();

protected:
	DECLARE_MESSAGE_MAP()
};


