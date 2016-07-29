#pragma once

#include "afxwin.h"
#include "ResourceTypeExperience.h"

// Bo�te de dialogue DialogTypeExperiment

class DialogTypeExperiment : public CDialog
{
	DECLARE_DYNAMIC(DialogTypeExperiment)

public:
	DialogTypeExperiment(CWnd* pParent = NULL);   // constructeur standard
	virtual ~DialogTypeExperiment();

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
