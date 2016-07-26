#pragma once

#include "afxwin.h"
#include "ResourceTypeExperience.h"

// Bo�te de dialogue Dialog_TypeExperiment

class Dialog_TypeExperiment : public CDialog
{
	DECLARE_DYNAMIC(Dialog_TypeExperiment)

public:
	Dialog_TypeExperiment(CWnd* pParent = NULL);   // constructeur standard
	virtual ~Dialog_TypeExperiment();

// Donn�es de bo�te de dialogue
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
