#pragma once
#include "SpinBtnCtrl.h"


// Boîte de dialogue CParametres_appareil

class CParametres_appareil : public CDialog
{
	DECLARE_DYNAMIC(CParametres_appareil)

public:
	CParametres_appareil(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CParametres_appareil();

// Données de boîte de dialogue
	enum { IDD = IDD_PARAMETRES_APPAREIL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString Fichier_parametres;
	//void ChargerFichierParametres(CString fichier_ini);
	//afx_msg void OnBnClickedOkAppareil();
	//float m_fSensibiliteCalo;
	double m_fSensibiliteCalo;
	//long double m_fSensibiliteCalo;
	CSpinBtnCtrl m_SpinSensibiliteCalo;
	float m_fSensibiliteCapteurBassePression;
	CSpinBtnCtrl m_SpinSensibiliteCapteurBassePression;
	float m_fSensibiliteCapteurHautePression;
	CSpinBtnCtrl m_SpinSensibiliteCapteurHautePression;
	virtual BOOL OnInitDialog();
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	float m_fPressionSecuriteBassePression;
	CSpinBtnCtrl m_SpinPressionSecuriteBassePression;
	float m_fPressionSecuriteHautePression;
	CSpinBtnCtrl m_SpinPressionSecuriteHautePression;
	float m_fPressionLimiteVide;
	CSpinBtnCtrl m_SpinPressionLimiteVide;
	float m_fVolumeRef;
	CSpinBtnCtrl m_SpinVolumeRef;
	float m_fVolumeP6;
	CSpinBtnCtrl m_SpinVolumeP6;
	CString m_StrNomCalo;
	CString m_StrEnteteFichier;
	BOOL m_bSecurite;
	BOOL m_bTuyere;
};
