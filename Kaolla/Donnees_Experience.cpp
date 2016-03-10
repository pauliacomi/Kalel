// Donnees_Experience.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "Donnees_Experience.h"


// Boîte de dialogue CDonnees_Experience

IMPLEMENT_DYNAMIC(CDonnees_Experience, CDialog)

CDonnees_Experience::CDonnees_Experience(CWnd* pParent /*=NULL*/)
	: CDialog(CDonnees_Experience::IDD, pParent)
	, m_StrDonneesExperience(_T(""))
{

}

CDonnees_Experience::~CDonnees_Experience()
{
}

void CDonnees_Experience::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_DONNEES_EXPERIENCE, m_StrDonneesExperience);
}


BEGIN_MESSAGE_MAP(CDonnees_Experience, CDialog)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDonnees_Experience

BOOL CDonnees_Experience::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_StrDonneesExperience = GetDonneesExperience();

	// UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}

void CDonnees_Experience::SetStrDonneesExperience(CString donnees)
{
	m_StrDonneesExperience = donnees;
	//UpdateData(FALSE);
}