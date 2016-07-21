#pragma once


// Boîte de dialogue CDonnees_Experience

class CDonnees_Experience : public CDialog
{
	DECLARE_DYNAMIC(CDonnees_Experience)

public:
	CDonnees_Experience(CWnd* pParent = NULL);   // constructeur standard
	virtual ~CDonnees_Experience();

// Données de boîte de dialogue
	enum { IDD = IDD_DONNEES_EXPERIENCE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // Prise en charge de DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	CString m_StrDonneesExperience;
	virtual BOOL OnInitDialog();
	void SetStrDonneesExperience(CString donnees);
};
