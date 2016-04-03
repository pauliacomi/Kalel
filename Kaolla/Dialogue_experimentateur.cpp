// Dialogue_experimentateur.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "Dialogue_experimentateur.h"


//////////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CAjoutExperimentateur

IMPLEMENT_DYNAMIC(CAjoutExperimentateur, CPropertyPage)

CAjoutExperimentateur::CAjoutExperimentateur()
	: CPropertyPage(CAjoutExperimentateur::IDD)
	, m_strNomAjoutExperimentateur(_T(""))
	, m_strSurnomAjoutExperimentateur(_T(""))
	, m_strAjoutExperimentateur(_T(""))
{

}

CAjoutExperimentateur::~CAjoutExperimentateur()
{
}

void CAjoutExperimentateur::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_NOM_AJOUT_EXPERIMENTATEUR, m_EditNomAjoutExperimentateur);
	DDX_Text(pDX, IDC_NOM_AJOUT_EXPERIMENTATEUR, m_strNomAjoutExperimentateur);
	DDX_Control(pDX, IDC_SURNOM_AJOUT_EXPERIMENTATEUR, m_EditSurnomAjoutExperimentateur);
	DDX_Text(pDX, IDC_SURNOM_AJOUT_EXPERIMENTATEUR, m_strSurnomAjoutExperimentateur);
	DDX_Control(pDX, IDC_EDIT_AJOUT_EXPERIMENTATEUR, m_EditAjoutExperimentateur);
	DDX_Text(pDX, IDC_EDIT_AJOUT_EXPERIMENTATEUR, m_strAjoutExperimentateur);
}


BEGIN_MESSAGE_MAP(CAjoutExperimentateur, CPropertyPage)
	ON_BN_CLICKED(IDC_AJOUTER, &CAjoutExperimentateur::OnBnClickedAjouter)
END_MESSAGE_MAP()


// Gestionnaires de messages de CAjoutExperimentateur

void CAjoutExperimentateur::OnBnClickedAjouter()
{
	// Mise à jour des variables attachées aux contrôles
	UpdateData(TRUE);

	// On vérifie si les chmaps ne sont pas vides
	if (m_strNomAjoutExperimentateur == "" || m_strSurnomAjoutExperimentateur == "")
	{
		m_strAjoutExperimentateur = "Tous les champs n'ont pas été remplis !";
	}
	else
	{	// On vérifie que l'expérimentateur n'existe pas et qu'il n'a pas un surnom déjà utilisé
		if(DoublonNomExperimentateur(m_strNomAjoutExperimentateur.GetBuffer()) ||
		   DoublonSurnomExperimentateur(m_strSurnomAjoutExperimentateur.GetBuffer()))
		{
			m_strAjoutExperimentateur = "";
			if (DoublonNomExperimentateur(m_strNomAjoutExperimentateur.GetBuffer()))
				m_strAjoutExperimentateur += "Nom déja utilisé\r\n";
			if (DoublonSurnomExperimentateur(m_strSurnomAjoutExperimentateur.GetBuffer()))
				m_strAjoutExperimentateur += "Surnom déja utilisé\r\n";
		}
		// Si toutes les conditions sont bonnes, on peut rajouter l'experimentateur dans un fichier XML
		else
		{
			Rajout_Experimentateur(m_strNomAjoutExperimentateur.GetBuffer(),
								   m_strSurnomAjoutExperimentateur.GetBuffer());
			m_strAjoutExperimentateur = "l'expérimentateur " + m_strNomAjoutExperimentateur + " (" +
										m_strSurnomAjoutExperimentateur + ") a été rajouté à la liste.";
			m_strNomAjoutExperimentateur = "";
			m_strSurnomAjoutExperimentateur = "";
		}
	}
	
	// Pour afficher les messages, on met à jour les contrôles
	UpdateData(FALSE);
	OnOK();
}









/////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CModifExperimentateur

IMPLEMENT_DYNAMIC(CModifExperimentateur, CPropertyPage)

CModifExperimentateur::CModifExperimentateur()
	: CPropertyPage(CModifExperimentateur::IDD)
	, m_nIndexModifExperimentateur(0)
	, m_strNomModifExperimentateur(_T(""))
	, m_strSurnomModifExperimentateur(_T(""))
	, m_strModifExperimentateur(_T(""))
{

}

CModifExperimentateur::~CModifExperimentateur()
{
}

void CModifExperimentateur::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODIF_EXPERIMENTATEUR, m_CBModifExperimentateur);
	DDX_CBIndex(pDX, IDC_COMBO_MODIF_EXPERIMENTATEUR, m_nIndexModifExperimentateur);
	DDX_Control(pDX, IDC_NOM_MODIF_EXPERIMENTATEUR, m_EditNomModifExperimentateur);
	DDX_Text(pDX, IDC_NOM_MODIF_EXPERIMENTATEUR, m_strNomModifExperimentateur);
	DDX_Control(pDX, IDC_SURNOM_MODIF_EXPERIMENTATEUR, m_EditSurnomModifExperimentateur);
	DDX_Text(pDX, IDC_SURNOM_MODIF_EXPERIMENTATEUR, m_strSurnomModifExperimentateur);
	DDX_Control(pDX, IDC_EDIT_MODIF_EXPERIMENTATEUR, m_EditModifExperimentateur);
	DDX_Text(pDX, IDC_EDIT_MODIF_EXPERIMENTATEUR, m_strModifExperimentateur);
}


BEGIN_MESSAGE_MAP(CModifExperimentateur, CPropertyPage)
	ON_BN_CLICKED(IDC_MODIFIER, &CModifExperimentateur::OnBnClickedModifier)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIF_EXPERIMENTATEUR, &CModifExperimentateur::OnCbnSelchangeComboModifExperimentateur)
END_MESSAGE_MAP()


// Gestionnaires de messages de CModifExperimentateur


// OnSetActive : fonction utilisée à chaque fois qu'on active la page
BOOL CModifExperimentateur::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}


// Lorsqu'on selectionne un expérimentateur à modifier
void CModifExperimentateur::OnCbnSelchangeComboModifExperimentateur()
{
	// On récupère les données des contrôles pour mettre à jour les variables
	UpdateData(TRUE);

	int index = m_nIndexModifExperimentateur;
	string nom_modif = list_modif_experimentateur[index].nom;
	string surnom_modif = list_modif_experimentateur[index].surnom;
	// On affiche les nom et prenom de l'expérimentateur désigné
	m_strNomModifExperimentateur.Format(_T("%s"),nom_modif.c_str());
	m_strSurnomModifExperimentateur.Format(_T("%s"),surnom_modif.c_str());
	UpdateData(FALSE);

}


// Lorsqu'on clique sur le bouton 'modifier'
void CModifExperimentateur::OnBnClickedModifier()
{
	// On met à jour les variables
	UpdateData(TRUE);
	string nom_modif((LPCTSTR)m_strNomModifExperimentateur);
	string surnom_modif ((LPCTSTR)m_strSurnomModifExperimentateur);
	int index = m_nIndexModifExperimentateur;

	// On modifie l'expérimentateur n°'index'.
	if(index !=-1)
	{
		if(Modif_Experimentateur(nom_modif,surnom_modif,index))
		{
			// Si tout se passe bien, on le signale dans la boite de dialogue
			m_strModifExperimentateur.Format(_T("L'experimentateur %s(%s) a été modifié en :\r\n%s (%s)"), 
				list_modif_experimentateur[index].nom.c_str(),list_modif_experimentateur[index].surnom.c_str(),
				nom_modif.c_str(),surnom_modif.c_str());

			ReinitialisationComboBox();
		}
		else
			// Si la modification ne se fait pas... On le signale dans la boite de dialogue
			m_strModifExperimentateur = "La modification n'a pu être effectuée\r\n Veuillez recommencer.";
	}
	else
		m_strModifExperimentateur = "Expérimentateur a modifié non défini";

	UpdateData(FALSE);
}

// On réinitialise la liste des expérimentateurs en cas ou on l'on ait modifié
void CModifExperimentateur::ReinitialisationComboBox()
{
	// On efface la liste des experimentateurs et on la réinitialise en récupérant 
	// les données dans le fichier XML adéquat
	list_modif_experimentateur.clear();
	list_modif_experimentateur=GetExperimentateurs();
	// On efface la contenance de m_CBModifExperimentateur
	// Puis on le (ré)initialise
	m_CBModifExperimentateur.ResetContent();
	for(UINT i=0;i<list_modif_experimentateur.size();i++)
	{
		CString StrSurnom;
		StrSurnom.Format(_T("%s"),list_modif_experimentateur[i].surnom.c_str());
		m_CBModifExperimentateur.InsertString(-1,StrSurnom);
	}

	m_nIndexModifExperimentateur = -1;
	m_strNomModifExperimentateur = "";
	m_strSurnomModifExperimentateur = "";
	UpdateData(FALSE);
}









///////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CSupprExperimentateur

IMPLEMENT_DYNAMIC(CSupprExperimentateur, CPropertyPage)

CSupprExperimentateur::CSupprExperimentateur()
	: CPropertyPage(CSupprExperimentateur::IDD)
	, m_MessageSupprExperimentateur(_T(""))
	, m_IndexSupprExperimentateur(0)
{

}

CSupprExperimentateur::~CSupprExperimentateur()
{
}

void CSupprExperimentateur::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SUPPR_EXPERIMENTATEUR, m_CBSupprExperimentateur);
	DDX_CBIndex(pDX, IDC_COMBO_SUPPR_EXPERIMENTATEUR, m_IndexSupprExperimentateur);
	DDX_Control(pDX, IDC_EDIT_SUPPR_EXPERIMENTATEUR, m_EditSupprExperimentateur);
	DDX_Text(pDX, IDC_EDIT_SUPPR_EXPERIMENTATEUR, m_MessageSupprExperimentateur);
}


BEGIN_MESSAGE_MAP(CSupprExperimentateur, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPR_EXPERIMENTATEUR, &CSupprExperimentateur::OnCbnSelchangeComboSupprExperimentateur)
	ON_BN_CLICKED(IDC_SUPPRIMER, &CSupprExperimentateur::OnBnClickedSupprimer)
END_MESSAGE_MAP()


// Gestionnaires de messages de CSupprExperimentateur

// Comme dans 'EXPERIMENT_TYPE_MODIFY', on réinitialise le ComboBox
BOOL CSupprExperimentateur::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}


void CSupprExperimentateur::OnCbnSelchangeComboSupprExperimentateur()
{
	// On met juste à jour la variable m_IndexSupprExperimentateur
	// Si on clique sur 'supprimer', on effacera l'expérimentateur 
	// de l'index m_IndexSupprExperimentateur mise à jour.
	UpdateData(TRUE);
}


void CSupprExperimentateur::OnBnClickedSupprimer()
{
	if(m_IndexSupprExperimentateur != -1)
	{
		// On supprime l'expérimentateur de l'index : m_IndexSupprExperimentateur
		if(Suppression_Experimentateur(m_IndexSupprExperimentateur))
		{
			// Si tout se passe bien, on le signale dans la boite de dialogue
			// Et on réinitialise le ComboBox
			int index = m_IndexSupprExperimentateur;
			string nom_suppr = list_suppr_experimentateur[index].nom;
			string surnom_suppr = list_suppr_experimentateur[index].surnom;
			m_MessageSupprExperimentateur.Format(_T("L'expérimentateur %s(%s) a été supprimé."),
				nom_suppr.c_str(),surnom_suppr.c_str());

			ReinitialisationComboBox();
		}
		else
		{
			// Si ça ne marche pas, on le signale dans la boite de dialogue
			m_MessageSupprExperimentateur = "Erreur dans la suppression";
		}
	}
	else
		m_MessageSupprExperimentateur = "Expérimentateur a supprimé non défini";

	UpdateData(FALSE);
}


void CSupprExperimentateur::ReinitialisationComboBox()
{
	// On (ré)initialise m_CBSupprExperimentateur
	list_suppr_experimentateur.clear();
	list_suppr_experimentateur=GetExperimentateurs();

	m_CBSupprExperimentateur.ResetContent();
	for(UINT i=0;i<list_suppr_experimentateur.size();i++)
	{
		CString StrSurnom;
		StrSurnom.Format(_T("%s"),list_suppr_experimentateur[i].surnom.c_str());
		m_CBSupprExperimentateur.InsertString(-1,StrSurnom);
	}

	m_IndexSupprExperimentateur = -1;
	UpdateData(FALSE);
}






/////////////////////////////////////////////////////////////////////////////////////////////
//
// CDialogueExperimentateur

IMPLEMENT_DYNAMIC(CDialogueExperimentateur, CPropertySheet)

CDialogueExperimentateur::CDialogueExperimentateur(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDialogueExperimentateur::CDialogueExperimentateur(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_AjoutExperimentateur);
	AddPage(&m_ModifExperimentateur);
	AddPage(&m_SupprExperimentateur);

	SetTitle("Ajouter/Modifier un expérimentateur");
}

CDialogueExperimentateur::~CDialogueExperimentateur()
{
}


BEGIN_MESSAGE_MAP(CDialogueExperimentateur, CPropertySheet)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDialogueExperimentateur

BOOL CDialogueExperimentateur::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// Pour enlever 'annuler' et 'appliquer'
	//int ids [] = {IDOK, IDCANCEL, ID_APPLY_NOW, IDHELP };
	int ids [] = {IDCANCEL, ID_APPLY_NOW };

	CWnd *pWnd;
	for(int i=0;i<sizeof(ids)/sizeof(int);i++)
	{
		pWnd = GetDlgItem( ids[i] );
		if(pWnd)
		{
			pWnd->EnableWindow(FALSE);
			pWnd->ShowWindow( SW_HIDE );
		}
	}

	return bResult;
}














