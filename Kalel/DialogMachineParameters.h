#ifndef DIALOGMACHINEPARAMETERS_H
#define DIALOGMACHINEPARAMETERS_H
#pragma once

#include "afxwin.h"
#include "ResourceParametresAppareil.h"

#include "SpinBtnCtrl.h"

class MachineSettings;

// Dialogbox ApparatusParameters

class ApparatusParameters : public CDialog
{
	DECLARE_DYNAMIC(ApparatusParameters)

public:
	ApparatusParameters(CWnd* pParent = NULL);   // standard constructor
	virtual ~ApparatusParameters();

// Données de boîte de dialogue
	enum { IDD = IDD_PARAMETRES_APPAREIL };
	void PassSettings(const MachineSettings& machineSettings);

protected:
	MachineSettings * settings;
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()

public:
	CString Fichier_parametres;

	double m_fSensibiliteCalo;
	CSpinBtnCtrl m_SpinSensibiliteCalo;

	float m_fSensibiliteCapteurBassePression;
	CSpinBtnCtrl m_SpinSensibiliteCapteurBassePression;

	float m_fSensibiliteCapteurHautePression;
	CSpinBtnCtrl m_SpinSensibiliteCapteurHautePression;

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

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CString m_StrNomCalo;
	CString m_StrEnteteFichier;
	BOOL m_bSecurite;
	BOOL m_bTuyere;
};

#endif