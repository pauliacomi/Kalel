#pragma once
#ifndef DIALOGUE_GAZ_H
#define DIALOGUE_GAZ_H

#include "afxwin.h"
#include "ParserXML.h"
#include "SpinBtnCtrl.h"
#include "ResourceDialogGas.h"



//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Add Gas Property Page------------------ 
				//------------------------------------------------------
				//
				//  For adding a new gas
				//
//////////////////////////////////////////////////////////////////////////////////////


class CAjoutGaz : public CPropertyPage
{
	DECLARE_DYNAMIC(CAjoutGaz)

public:
	CAjoutGaz();
	virtual ~CAjoutGaz();

// Données de boîte de dialogue
	enum { IDD = IDD_AJOUT_GAZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditNomAjoutGaz;
	CString m_strNomAjoutGaz;
	CEdit m_EditSymboleAjoutGaz;
	CString m_strSymboleAjoutGaz;
	CEdit m_EditMasseMoleculaireAjoutGaz;
	double m_fMasseMoleculaireAjoutGaz;
	CSpinBtnCtrl pSpinMasseMoleculaireAjoutGaz;
	CEdit m_EditTemperatureCritiqueAjoutGaz;
	double m_fTemperatureCritiqueAjoutGaz;
	CSpinBtnCtrl pSpinTemperatureCritiqueAjoutGaz;
	CEdit m_EditPressionCritiqueAjoutGaz;
	double m_fPressionCritiqueAjoutGaz;
	CSpinBtnCtrl pSpinPressionCritiqueAjoutGaz;
	CEdit m_EditTemperatureEbullitionAjoutGaz;
	double m_fTemperatureEbullitionAjoutGaz;
	CSpinBtnCtrl pSpinTemperatureEbullitionAjoutGaz;
	CEdit m_EditOmegaAjoutGaz;
	double m_fOmegaAjoutGaz;
	CSpinBtnCtrl pSpinOmegaAjoutGaz;
	CEdit m_EditMessageAjoutGaz;
	CString m_strMessageAjoutGaz;

	afx_msg void OnBnClickedAjouter();
	virtual BOOL OnInitDialog();
};






//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Modify Gas Property Page------------------ 
				//------------------------------------------------------
				//
				//  For modifying a gas
				//
//////////////////////////////////////////////////////////////////////////////////////

class CModifGaz : public CPropertyPage
{
	DECLARE_DYNAMIC(CModifGaz)

public:
	CModifGaz();
	virtual ~CModifGaz();

// Données de boîte de dialogue
	enum { IDD = IDD_MODIF_GAZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CComboBox m_CBModifGaz;
	int m_nIndexModifGaz;
	CEdit m_EditNomModifGaz;
	CString m_strNomModifGaz;
	CEdit m_EditSymboleModifGaz;
	CString m_strSymboleModifGaz;
	CEdit m_EditMasseMoleculaireModifGaz;
	double m_fMasseMoleculaireModifGaz;
	CSpinBtnCtrl pSpinMasseMoleculaireModifGaz;
	CEdit m_EditTemperatureCritiqueModifGaz;
	double m_fTemperatureCritiqueModifGaz;
	CSpinBtnCtrl pSpinTemperatureCritiqueModifGaz;
	CEdit m_EditPressionCritiqueModifGaz;
	double m_fPressionCritiqueModifGaz;
	CSpinBtnCtrl pSpinPressionCritiqueModifGaz;
	CEdit m_EditTemperatureEbullitionModifGaz;
	double m_fTemperatureEbullitionModifGaz;
	CSpinBtnCtrl pSpinTemperatureEbullitionModifGaz;
	CEdit m_EditOmegaModifGaz;
	double m_fOmegaModifGaz;
	CSpinBtnCtrl pSpinOmegaModifGaz;
	CEdit m_EditMessageModifGaz;
	CString m_strMessageModifGaz;

	vector<gaz> list_modif_gaz;

	afx_msg void OnCbnSelchangeComboModifGaz();
	virtual BOOL OnInitDialog();
	virtual BOOL OnSetActive();
	afx_msg void OnBnClickedModifier();
	void ReinitialisationComboBox();
};




//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Delete Gas Property Page------------------ 
				//------------------------------------------------------
				//
				//  For deleting a gas
				//
//////////////////////////////////////////////////////////////////////////////////////

class CSupprGaz : public CPropertyPage
{
	DECLARE_DYNAMIC(CSupprGaz)

public:
	CSupprGaz();
	virtual ~CSupprGaz();

// Données de boîte de dialogue
	enum { IDD = IDD_SUPPR_GAZ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_CBSupprGaz;
	int m_nIndexSupprGaz;
	CEdit m_EditSupprGaz;
	CString m_strMessageSupprGaz;

	CString m_strNomSuppGaz;
	CString m_strSymboleSuppGaz;
	float m_fMasseMoleculaireSuppGaz;
	float m_fTemperatureCritiqueSuppGaz;
	float m_fPressionCritiqueSuppGaz;
	float m_fTemperatureEbullitionSuppGaz;
	float m_fOmegaSuppGaz;


	CEdit m_EditNomSuppGaz;
	CEdit m_EditSymboleSuppGaz;
	CEdit m_EditMasseMoleculaireSuppGaz;
	CEdit m_EditTemperatureCritiqueSuppGaz;
	CEdit m_EditPressionCritiqueSuppGaz;
	CEdit m_EditTemperatureEbullitionSuppGaz;
	CEdit m_EditOmegaSuppGaz;

	vector<gaz> list_suppr_gaz;

	virtual BOOL OnSetActive();
	afx_msg void OnCbnSelchangeComboSupprGaz();
	afx_msg void OnBnClickedSupprimer();
	void ReinitialisationComboBox();

};







//////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------
//----------------- Gas Property Sheet------------------ 
//------------------------------------------------------
//
//  Main class, all property pages will be added here
//
//////////////////////////////////////////////////////////////////////////////////////


class CDialogueGaz : public CPropertySheet
{
	DECLARE_DYNAMIC(CDialogueGaz)

public:
	CAjoutGaz m_AjoutGaz;
	CModifGaz m_ModifGaz;
	CSupprGaz m_SupprGaz;

public:
	CDialogueGaz(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CDialogueGaz(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CDialogueGaz();

protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
};


#endif