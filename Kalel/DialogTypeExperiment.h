#pragma once
#include "afxwin.h"
#include "ResourceTypeExperience.h"

class MachineSettings;

// Boîte de dialogue DialogTypeExperiment

class DialogTypeExperiment : public CDialog
{
	DECLARE_DYNAMIC(DialogTypeExperiment)

public:
	DialogTypeExperiment(CWnd* pParent = NULL);   // constructeur standard
	virtual ~DialogTypeExperiment();

// Données de boîte de dialogue
	enum { IDD = IDD_TYPE_EXPERIENCE };
	void PassSettings(const MachineSettings& machineSettings);

protected:
	MachineSettings * settings;
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
