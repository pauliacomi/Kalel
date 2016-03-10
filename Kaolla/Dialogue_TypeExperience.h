#pragma once

#include "Define.h"

// Boîte de dialogue CDialogue_TypeExperience

class CDialogue_TypeExperience : public CDialog
{
	DECLARE_DYNAMIC(CDialogue_TypeExperience)

public:
	CDialogue_TypeExperience(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDialogue_TypeExperience();

// Données de boîte de dialogue
	enum { IDD = IDD_TYPE_EXPERIENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_bExperienceAuto;
	int TypeExperience;
protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
};
