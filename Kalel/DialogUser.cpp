// Dialogue_experimentateur.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kalel.h"
#include "DialogUser.h"




//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//-------------- User Property Sheet------------------ 
				//------------------------------------------------------
				//
				//  Where all the tabs will go
				//
//////////////////////////////////////////////////////////////////////////////////////

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

	// Set the name of the title
	CString title;
	title.Format(TITLE_DIALOG_USER);
	SetTitle(title);
}

CDialogueExperimentateur::~CDialogueExperimentateur()
{
}

BOOL CDialogueExperimentateur::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	// We will need a pointer to each button
	CButton *btnOK, *btnCancel, *btnApply;
	// We will need the location and dimensions of Apply
	CRect RectApply;

	// Get handles to the OK, Cancel and Apply buttons
	btnOK = reinterpret_cast<CButton *>(GetDlgItem(IDOK));
	btnCancel = reinterpret_cast<CButton *>(GetDlgItem(IDCANCEL));
	btnApply = reinterpret_cast<CButton *>(GetDlgItem(ID_APPLY_NOW));

	// Get the location and the dimensions of the Apply button
	btnApply->GetWindowRect(&RectApply);

	// Dismiss the Apply and the Cancel buttons
	btnApply->DestroyWindow();
	btnCancel->DestroyWindow();

	// Convert the location and dimensions to screen coordinates
	ScreenToClient(&RectApply);

	// Put the OK button where the Apply button was
	btnOK->SetWindowPos(NULL, RectApply.left, RectApply.top, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_SHOWWINDOW);

	return bResult;
}

BEGIN_MESSAGE_MAP(CDialogueExperimentateur, CPropertySheet)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDialogueExperimentateur










//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Add User Property Page------------------ 
				//------------------------------------------------------
				//
				//  For adding a user to the configuration file
				//
//////////////////////////////////////////////////////////////////////////////////////


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
	ON_BN_CLICKED(IDC_ADD_USER, &CAjoutExperimentateur::OnBnClickedAjouter)
END_MESSAGE_MAP()


// Gestionnaires de messages de CAjoutExperimentateur

void CAjoutExperimentateur::OnBnClickedAjouter()
{
	// Mise à jour des variables attachées aux contrôles
	UpdateData(TRUE);

	CString message;

	// On vérifie si les chmaps ne sont pas vides
	if (m_strNomAjoutExperimentateur == "" || m_strSurnomAjoutExperimentateur == "")
	{
		message.Format(ERROR_FIELDS_NOT_FILLED);
		m_strAjoutExperimentateur = message + "\r\n";
	}

	else
	{	// On vérifie que l'expérimentateur n'existe pas et qu'il n'a pas un surnom déjà utilisé
		if( DoublonSurnomExperimentateur(m_strSurnomAjoutExperimentateur.GetBuffer()))
		{
			message.Format(ERROR_NAME_USED);
			m_strAjoutExperimentateur = message + "\r\n";
		}
		// Si toutes les conditions sont bonnes, on peut rajouter l'experimentateur dans un fichier XML
		else
		{
			Rajout_Experimentateur(m_strNomAjoutExperimentateur.GetBuffer(),
								   m_strSurnomAjoutExperimentateur.GetBuffer());

			message.Format(TEXT_USER_ADDED, m_strNomAjoutExperimentateur, m_strSurnomAjoutExperimentateur);
			m_strAjoutExperimentateur = message + "\r\n";

			m_strNomAjoutExperimentateur = "";
			m_strSurnomAjoutExperimentateur = "";
		}
	}
	
	// Pour afficher les messages, on met à jour les contrôles
	UpdateData(FALSE);
	OnOK();
}










//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Modify User Property Page------------------ 
				//------------------------------------------------------
				//
				//  For modifying a user in the configuration file
				//
//////////////////////////////////////////////////////////////////////////////////////

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


// OnSetActive : fonction utilisée à chaque fois qu'on active la page
BOOL CModifExperimentateur::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}


BEGIN_MESSAGE_MAP(CModifExperimentateur, CPropertyPage)
	ON_BN_CLICKED(IDC_MODIFY_USER, &CModifExperimentateur::OnBnClickedModifier)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIF_EXPERIMENTATEUR, &CModifExperimentateur::OnCbnSelchangeComboModifExperimentateur)
END_MESSAGE_MAP()


// Gestionnaires de messages de CModifExperimentateur


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
		if (m_strNomModifExperimentateur == "" || m_strSurnomModifExperimentateur == "")
		{
			m_strModifExperimentateur.Format(ERROR_FIELDS_NOT_FILLED);
		}
		else
		{
			// On vérifie que l'expérimentateur n'existe pas et qu'il n'a pas un surnom déjà utilisé
			if (DoublonSurnomExperimentateur(m_strSurnomModifExperimentateur.GetBuffer()) &&
				DoublonNomExperimentateur(m_strNomModifExperimentateur.GetBuffer())
				)
			{
				m_strModifExperimentateur.Format(ERROR_NAME_USED);
			}
			else {
				if (Modif_Experimentateur(nom_modif, surnom_modif, index) == true)
				{
					// Si tout se passe bien, on le signale dans la boite de dialogue
					m_strModifExperimentateur.Format(TEXT_USER_MODIFIED,
						list_modif_experimentateur[index].nom.c_str(), list_modif_experimentateur[index].surnom.c_str(),
						nom_modif.c_str(), surnom_modif.c_str());

					ReinitialisationComboBox();
				}
				else {	// Si la modification ne se fait pas... On le signale dans la boite de dialogue
					m_strModifExperimentateur.Format(ERROR_MODIFICATION);
				}
			}
		}
	}
	else {
		m_strModifExperimentateur.Format(ERROR_SELECTED);
	}

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









//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Delete User Property Page------------------ 
				//------------------------------------------------------
				//
				//  For deleting a user in the configuration file
				//
//////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSupprExperimentateur, CPropertyPage)

CSupprExperimentateur::CSupprExperimentateur()
	: CPropertyPage(CSupprExperimentateur::IDD)
	, m_MessageSupprExperimentateur(_T(""))
	, m_IndexSupprExperimentateur(0)
	, m_strNomSupprExperimentateur(_T(""))
	, m_strSurnomSupprExperimentateur(_T(""))
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

	DDX_Control(pDX, IDC_NOM_SUPPR_EXPERIMENTATEUR, m_EditNomSupprExperimentateur);
	DDX_Text(pDX, IDC_NOM_SUPPR_EXPERIMENTATEUR, m_strNomSupprExperimentateur);
	DDX_Control(pDX, IDC_SURNOM_SUPPR_EXPERIMENTATEUR, m_EditSurnomSupprExperimentateur);
	DDX_Text(pDX, IDC_SURNOM_SUPPR_EXPERIMENTATEUR, m_strSurnomSupprExperimentateur);
}

// Comme dans 'EXPERIMENT_TYPE_MODIFY', on réinitialise le ComboBox
BOOL CSupprExperimentateur::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}

BEGIN_MESSAGE_MAP(CSupprExperimentateur, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPR_EXPERIMENTATEUR, &CSupprExperimentateur::OnCbnSelchangeComboSupprExperimentateur)
	ON_BN_CLICKED(IDC_DELETE_USER, &CSupprExperimentateur::OnBnClickedSupprimer)
END_MESSAGE_MAP()


// Gestionnaires de messages de CSupprExperimentateur

void CSupprExperimentateur::OnCbnSelchangeComboSupprExperimentateur()
{
	// On met juste à jour la variable m_IndexSupprExperimentateur
	// Si on clique sur 'supprimer', on effacera l'expérimentateur 
	// de l'index m_IndexSupprExperimentateur mise à jour.
	UpdateData(TRUE);

	// On affiche les données de la cellule qu'on veux effacer
	m_strNomSupprExperimentateur = list_suppr_experimentateur[m_IndexSupprExperimentateur].nom.c_str();
	m_strSurnomSupprExperimentateur = list_suppr_experimentateur[m_IndexSupprExperimentateur].surnom.c_str();

	UpdateData(FALSE);
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
			m_MessageSupprExperimentateur.Format(TEXT_USER_DELETED,	nom_suppr.c_str(),surnom_suppr.c_str());

			ReinitialisationComboBox();
		}
		else
		{	// Si ça ne marche pas, on le signale dans la boite de dialogue
			m_MessageSupprExperimentateur.Format(ERROR_DELETION);
		}
	}
	else
		m_MessageSupprExperimentateur.Format(ERROR_SELECTED);

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
