// Dialogue_gaz.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "Dialogue_gaz.h"


// Boîte de dialogue CAjoutGaz

IMPLEMENT_DYNAMIC(CAjoutGaz, CPropertyPage)

CAjoutGaz::CAjoutGaz()
	: CPropertyPage(CAjoutGaz::IDD)
	, m_strNomAjoutGaz(_T(""))
	, m_strSymboleAjoutGaz(_T(""))
	, m_fMasseMoleculaireAjoutGaz(0)
	, m_fTemperatureCritiqueAjoutGaz(0)
	, m_fPressionCritiqueAjoutGaz(0)
	, m_fTemperatureEbullitionAjoutGaz(0)
	, m_fOmegaAjoutGaz(0)
	, m_strMessageAjoutGaz(_T(""))
{

}

CAjoutGaz::~CAjoutGaz()
{
}

void CAjoutGaz::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NOM_AJOUT_GAZ, m_EditNomAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_NOM_AJOUT_GAZ, m_strNomAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_SYMBOLE_AJOUT_GAZ, m_EditSymboleAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_SYMBOLE_AJOUT_GAZ, m_strSymboleAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_MASSE_MOLECULAIRE_AJOUT_GAZ, m_EditMasseMoleculaireAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_MASSE_MOLECULAIRE_AJOUT_GAZ, m_fMasseMoleculaireAjoutGaz);
	DDX_Control(pDX, IDC_SPIN_MASSE_MOLECULAIRE_AJOUT_GAZ, pSpinMasseMoleculaireAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_TEMPERATURE_CRITIQUE_AJOUT_GAZ, m_EditTemperatureCritiqueAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE_CRITIQUE_AJOUT_GAZ, m_fTemperatureCritiqueAjoutGaz);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_CRITIQUE_AJOUT_GAZ, pSpinTemperatureCritiqueAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_PRESSION_CRITIQUE_AJOUT_GAZ, m_EditPressionCritiqueAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_PRESSION_CRITIQUE_AJOUT_GAZ, m_fPressionCritiqueAjoutGaz);
	DDX_Control(pDX, IDC_SPIN_PRESSION_CRITIQUE_AJOUT_GAZ, pSpinPressionCritiqueAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_TEMPERATURE_EBULLITION_AJOUT_GAZ, m_EditTemperatureEbullitionAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE_EBULLITION_AJOUT_GAZ, m_fTemperatureEbullitionAjoutGaz);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_EBULLITION_AJOUT_GAZ, pSpinTemperatureEbullitionAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_OMEGA_AJOUT_GAZ, m_EditOmegaAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_OMEGA_AJOUT_GAZ, m_fOmegaAjoutGaz);
	DDX_Control(pDX, IDC_SPIN_OMEGA_AJOUT_GAZ, pSpinOmegaAjoutGaz);
	DDX_Control(pDX, IDC_EDIT_AJOUT_GAZ, m_EditMessageAjoutGaz);
	DDX_Text(pDX, IDC_EDIT_AJOUT_GAZ, m_strMessageAjoutGaz);
}


BEGIN_MESSAGE_MAP(CAjoutGaz, CPropertyPage)
	ON_BN_CLICKED(IDC_AJOUTER, &CAjoutGaz::OnBnClickedAjouter)
END_MESSAGE_MAP()


// Gestionnaires de messages de CAjoutGaz




BOOL CAjoutGaz::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// On initialise juste des boutons spin

	//pSpinMasseMoleculaireAjoutGaz.SetRange(0,5);
	pSpinMasseMoleculaireAjoutGaz.SetPos(1);
	pSpinMasseMoleculaireAjoutGaz.SetInc(-0.0001);
	pSpinMasseMoleculaireAjoutGaz.SetFormat("%1.4f");
	pSpinMasseMoleculaireAjoutGaz.UpdateBuddy();

	//pSpinTemperatureCritiqueAjoutGaz.SetRange(0,5);
	pSpinTemperatureCritiqueAjoutGaz.SetPos(30);
	pSpinTemperatureCritiqueAjoutGaz.SetInc(-1);
	pSpinTemperatureCritiqueAjoutGaz.SetFormat("%1.f");
	pSpinTemperatureCritiqueAjoutGaz.UpdateBuddy();

	//pSpinPressionCritiqueAjoutGaz.SetRange(0,5);
	pSpinPressionCritiqueAjoutGaz.SetPos(1);
	pSpinPressionCritiqueAjoutGaz.SetInc(-0.0001);
	pSpinPressionCritiqueAjoutGaz.SetFormat("%1.4f");
	pSpinPressionCritiqueAjoutGaz.UpdateBuddy();

	//pSpinTemperatureEbullitionAjoutGaz.SetRange(0,5);
	pSpinTemperatureEbullitionAjoutGaz.SetPos(30);
	pSpinTemperatureEbullitionAjoutGaz.SetInc(-1);
	pSpinTemperatureEbullitionAjoutGaz.SetFormat("%1.f");
	pSpinTemperatureEbullitionAjoutGaz.UpdateBuddy();

	//pSpinOmegaAjoutGaz.SetRange(0,5);
	pSpinOmegaAjoutGaz.SetPos(1);
	pSpinOmegaAjoutGaz.SetInc(-0.0001);
	pSpinOmegaAjoutGaz.SetFormat("%1.4f");
	pSpinOmegaAjoutGaz.UpdateBuddy();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}



void CAjoutGaz::OnBnClickedAjouter()
{
	// Mise à jour des variables attachées aux contrôles
	UpdateData(TRUE);

	// On vérifie si les chmaps ne sont pas vides
	if(m_strNomAjoutGaz == "" || m_strSymboleAjoutGaz=="")
	{
		m_strMessageAjoutGaz = "Tous les champs n'ont pas été remplis.";
	}

	else
	{	// On vérifie que le gaz ne soit pas déjà répertorié
		if(DoublonNomGaz(m_strNomAjoutGaz.GetBuffer()) ||
		   DoublonSymboleGaz(m_strSymboleAjoutGaz.GetBuffer()))
		{
			m_strMessageAjoutGaz = "";
			if (DoublonNomGaz(m_strNomAjoutGaz.GetBuffer()))
				m_strMessageAjoutGaz += "Nom déja utilisé\r\n";
			if (DoublonSymboleGaz(m_strSymboleAjoutGaz.GetBuffer()))
				m_strMessageAjoutGaz += "Symbole déja utilisé\r\n";
		}
		// Si toutes les conditions sont bonnes, on peut rajouter le gaz dans un fichier XML
		else
		{
			Rajout_Gaz(m_strNomAjoutGaz.GetBuffer(),m_strSymboleAjoutGaz.GetBuffer(),
					   m_fMasseMoleculaireAjoutGaz,m_fTemperatureCritiqueAjoutGaz, 
					   m_fPressionCritiqueAjoutGaz, m_fTemperatureEbullitionAjoutGaz);
			m_strMessageAjoutGaz = "Le gaz " + m_strNomAjoutGaz + " (" +
								   m_strSymboleAjoutGaz + ") a été rajouté à la liste.";
			m_strNomAjoutGaz = "";
			m_strSymboleAjoutGaz = "";
		}
	}
	// Pour afficher les messages, on met à jour les contrôles
	UpdateData(FALSE);

}









//////////////////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CModifGaz

IMPLEMENT_DYNAMIC(CModifGaz, CPropertyPage)

CModifGaz::CModifGaz()
	: CPropertyPage(CModifGaz::IDD)
	, m_nIndexModifGaz(0)
	, m_strNomModifGaz(_T(""))
	, m_strSymboleModifGaz(_T(""))
	, m_fMasseMoleculaireModifGaz(0)
	, m_fTemperatureCritiqueModifGaz(0)
	, m_fPressionCritiqueModifGaz(0)
	, m_fTemperatureEbullitionModifGaz(0)
	, m_fOmegaModifGaz(0)
	, m_strMessageModifGaz(_T(""))
{

}

CModifGaz::~CModifGaz()
{
}

void CModifGaz::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODIF_GAZ, m_CBModifGaz);
	DDX_CBIndex(pDX, IDC_COMBO_MODIF_GAZ, m_nIndexModifGaz);
	DDX_Control(pDX, IDC_EDIT_NOM_MODIF_GAZ, m_EditNomModifGaz);
	DDX_Text(pDX, IDC_EDIT_NOM_MODIF_GAZ, m_strNomModifGaz);
	DDX_Control(pDX, IDC_EDIT_SYMBOLE_MODIF_GAZ, m_EditSymboleModifGaz);
	DDX_Text(pDX, IDC_EDIT_SYMBOLE_MODIF_GAZ, m_strSymboleModifGaz);
	DDX_Control(pDX, IDC_EDIT_MASSE_MOLECULAIRE_MODIF_GAZ, m_EditMasseMoleculaireModifGaz);
	DDX_Text(pDX, IDC_EDIT_MASSE_MOLECULAIRE_MODIF_GAZ, m_fMasseMoleculaireModifGaz);
	DDX_Control(pDX, IDC_SPIN_MASSE_MOLECULAIRE_MODIF_GAZ, pSpinMasseMoleculaireModifGaz);
	DDX_Control(pDX, IDC_EDIT_TEMPERATURE_CRITIQUE_MODIF_GAZ, m_EditTemperatureCritiqueModifGaz);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE_CRITIQUE_MODIF_GAZ, m_fTemperatureCritiqueModifGaz);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_CRITIQUE_MODIF_GAZ, pSpinTemperatureCritiqueModifGaz);
	DDX_Control(pDX, IDC_EDIT_PRESSION_CRITIQUE_MODIF_GAZ, m_EditPressionCritiqueModifGaz);
	DDX_Text(pDX, IDC_EDIT_PRESSION_CRITIQUE_MODIF_GAZ, m_fPressionCritiqueModifGaz);
	DDX_Control(pDX, IDC_SPIN_PRESSION_CRITIQUE_MODIF_GAZ, pSpinPressionCritiqueModifGaz);
	DDX_Control(pDX, IDC_EDIT_TEMPERATURE_EBULLITION_MODIF_GAZ, m_EditTemperatureEbullitionModifGaz);
	DDX_Text(pDX, IDC_EDIT_TEMPERATURE_EBULLITION_MODIF_GAZ, m_fTemperatureEbullitionModifGaz);
	DDX_Control(pDX, IDC_SPIN_TEMPERATURE_EBULLITION_MODIF_GAZ, pSpinTemperatureEbullitionModifGaz);
	DDX_Control(pDX, IDC_EDIT_OMEGA_MODIF_GAZ, m_EditOmegaModifGaz);
	DDX_Text(pDX, IDC_EDIT_OMEGA_MODIF_GAZ, m_fOmegaModifGaz);
	DDX_Control(pDX, IDC_SPIN_OMEGA_MODIF_GAZ, pSpinOmegaModifGaz);
	DDX_Control(pDX, IDC_EDIT_MODIF_GAZ, m_EditMessageModifGaz);
	DDX_Text(pDX, IDC_EDIT_MODIF_GAZ, m_strMessageModifGaz);

}


BEGIN_MESSAGE_MAP(CModifGaz, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIF_GAZ, &CModifGaz::OnCbnSelchangeComboModifGaz)
	ON_BN_CLICKED(IDC_MODIFIER, &CModifGaz::OnBnClickedModifier)
END_MESSAGE_MAP()


// Gestionnaires de messages de CModifGaz



BOOL CModifGaz::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// On initialise seulement les boutons spin

	//pSpinMasseMoleculaireModifGaz.SetRange(0,5);
	pSpinMasseMoleculaireModifGaz.SetPos(1);
	pSpinMasseMoleculaireModifGaz.SetInc(-0.0001);
	pSpinMasseMoleculaireModifGaz.SetFormat("%1.4f");
	pSpinMasseMoleculaireModifGaz.UpdateBuddy();

	//pSpinTemperatureCritiqueModifGaz.SetRange(0,5);
	pSpinTemperatureCritiqueModifGaz.SetPos(30);
	pSpinTemperatureCritiqueModifGaz.SetInc(-1);
	pSpinTemperatureCritiqueModifGaz.SetFormat("%1.f");
	pSpinTemperatureCritiqueModifGaz.UpdateBuddy();

	//pSpinPressionCritiqueModifGaz.SetRange(0,5);
	pSpinPressionCritiqueModifGaz.SetPos(1);
	pSpinPressionCritiqueModifGaz.SetInc(-0.0001);
	pSpinPressionCritiqueModifGaz.SetFormat("%1.4f");
	pSpinPressionCritiqueModifGaz.UpdateBuddy();

	//pSpinTemperatureEbullitionModifGaz.SetRange(0,5);
	pSpinTemperatureEbullitionModifGaz.SetPos(30);
	pSpinTemperatureEbullitionModifGaz.SetInc(-1);
	pSpinTemperatureEbullitionModifGaz.SetFormat("%1.f");
	pSpinTemperatureEbullitionModifGaz.UpdateBuddy();

	//pSpinOmegaModifGaz.SetRange(0,5);
	pSpinOmegaModifGaz.SetPos(1);
	pSpinOmegaModifGaz.SetInc(-0.0001);
	pSpinOmegaModifGaz.SetFormat("%1.4f");
	pSpinOmegaModifGaz.UpdateBuddy();



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


// OnSetActive : fonction utilisée à chaque fois qu'on active la page
BOOL CModifGaz::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}

// Lorsqu'on selectionne un gaz à modifier
void CModifGaz::OnCbnSelchangeComboModifGaz()
{
	// On récupère les données des contrôles pour mettre à jour les variables
	UpdateData(TRUE);
	int index = m_nIndexModifGaz;
	string nom_modif = list_modif_gaz[index].nom;
	string symbole_modif = list_modif_gaz[index].symbole;

	// On affiche les données du gaz désigné
	m_strNomModifGaz.Format(_T("%s"),nom_modif.c_str());
	m_strSymboleModifGaz.Format(_T("%s"),symbole_modif.c_str());
	m_fMasseMoleculaireModifGaz = list_modif_gaz[index].masse_moleculaire;
	m_fTemperatureCritiqueModifGaz = list_modif_gaz[index].temperature_critique;
	m_fPressionCritiqueModifGaz = list_modif_gaz[index].pression_critique;
	m_fTemperatureEbullitionModifGaz = list_modif_gaz[index].temperature_ebullition;
	m_fOmegaModifGaz = list_modif_gaz[index].omega;

	UpdateData(FALSE);

}

// Lorsqu'on clique sur le bouton 'modifier'
void CModifGaz::OnBnClickedModifier()
{
	// On met à jour les variables
	UpdateData(TRUE);
	string nom_modif((LPCTSTR)m_strNomModifGaz);
	string symbole_modif ((LPCTSTR)m_strSymboleModifGaz);
	int index = m_nIndexModifGaz;

	// On modifie l'expérimentateur n°'index'.
	if(index != -1)
	{
		if(Modif_Gaz(nom_modif,symbole_modif,m_fMasseMoleculaireModifGaz,m_fTemperatureCritiqueModifGaz,
					 m_fPressionCritiqueModifGaz,m_fTemperatureEbullitionModifGaz,index))
		{	// Si tout se passe bien, on le signale dans la boite de dialogue
			m_strMessageModifGaz.Format(_T("Le gaz %s(%s) a été modifié."), 
				nom_modif.c_str(),symbole_modif.c_str());
			
			ReinitialisationComboBox();

		}
		else
			// Si la modification ne se fait pas... On le signale dans la boite de dialogue
			m_strMessageModifGaz = "La modification n'a pu être effectuée\r\n Veuillez recommencer.";
	}
	else
		m_strMessageModifGaz = "Gaz à modifier non sélectionné.";

	UpdateData(FALSE);
}

// On réinitialise la liste des gaz en cas ou on l'on ait modifié
void CModifGaz::ReinitialisationComboBox()
{
	// On efface la liste des gaz et on la réinitialise en récupérant 
	// les données dans le fichier XML adéquat
	list_modif_gaz.clear();
	list_modif_gaz=GetGazs();
	// On efface la contenance de m_CBModifGaz
	// Puis on le (ré)initialise
	m_CBModifGaz.ResetContent();
	for(UINT i=0;i<list_modif_gaz.size();i++)
	{
		CString StrSymbole;
		StrSymbole.Format(_T("%s"),list_modif_gaz[i].symbole.c_str());
		m_CBModifGaz.InsertString(-1,StrSymbole);
	}

	m_nIndexModifGaz = -1;
	m_strNomModifGaz = "";
	m_strSymboleModifGaz = "";

	pSpinMasseMoleculaireModifGaz.SetPos(1);
	pSpinTemperatureCritiqueModifGaz.SetPos(30);
	pSpinPressionCritiqueModifGaz.SetPos(1);
	pSpinTemperatureEbullitionModifGaz.SetPos(30);
	pSpinOmegaModifGaz.SetPos(1);

	UpdateData(FALSE);
}









/////////////////////////////////////////////////////////////////////////////////
//
// Boîte de dialogue CSupprGaz

IMPLEMENT_DYNAMIC(CSupprGaz, CPropertyPage)

CSupprGaz::CSupprGaz()
	: CPropertyPage(CSupprGaz::IDD)
	, m_nIndexSupprGaz(0)
	, m_strMessageSupprGaz(_T(""))
{

}

CSupprGaz::~CSupprGaz()
{
}

void CSupprGaz::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SUPPR_GAZ, m_CBSupprGaz);
	DDX_CBIndex(pDX, IDC_COMBO_SUPPR_GAZ, m_nIndexSupprGaz);
	DDX_Control(pDX, IDC_EDIT_SUPPR_GAZ, m_EditSupprGaz);
	DDX_Text(pDX, IDC_EDIT_SUPPR_GAZ, m_strMessageSupprGaz);
}


BEGIN_MESSAGE_MAP(CSupprGaz, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPR_GAZ, &CSupprGaz::OnCbnSelchangeComboSupprGaz)
	ON_BN_CLICKED(IDC_SUPPRIMER, &CSupprGaz::OnBnClickedSupprimer)
END_MESSAGE_MAP()


// Gestionnaires de messages de CSupprGaz

// Comme dans 'MODIFY_WINDOW', on réinitialise le ComboBox
BOOL CSupprGaz::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}


void CSupprGaz::OnCbnSelchangeComboSupprGaz()
{
	// On met juste à jour la variable m_IndexSupprGaz
	// Si on clique sur 'supprimer', on effacera le gaz
	// de l'index m_IndexSupprGaz mise à jour.
	UpdateData(TRUE);
}


void CSupprGaz::OnBnClickedSupprimer()
{
	// On supprime le gaz de l'index : m_IndexSupprGaz
	int index = m_nIndexSupprGaz;
	if(index != -1)
	{
		if(Suppression_Gaz(m_nIndexSupprGaz))
		{	// Si tout se passe bien, on le signale dans la boite de dialogue
			// Et on réinitialise le ComboBox
			string nom_suppr = list_suppr_gaz[index].nom;
			string symbole_suppr = list_suppr_gaz[index].symbole;
			m_strMessageSupprGaz.Format(_T("Le gaz %s(%s) a été supprimé."),
				nom_suppr.c_str(),symbole_suppr.c_str());

			ReinitialisationComboBox();

		}
		else
		{	// Si ça ne marche pas, on le signale dans la boite de dialogue
			m_strMessageSupprGaz = "Erreur dans la suppression";
		}
	}
	else
		m_strMessageSupprGaz = "Gaz à supprimer non sélectionné";
	
	UpdateData(FALSE);
}

void CSupprGaz::ReinitialisationComboBox()
{
	// On (ré)initialise m_CBSupprGaz
	list_suppr_gaz.clear();
	list_suppr_gaz=GetGazs();

	m_CBSupprGaz.ResetContent();
	for(UINT i=0;i<list_suppr_gaz.size();i++)
	{
		CString StrSymbole;
		StrSymbole.Format(_T("%s"),list_suppr_gaz[i].symbole.c_str());
		m_CBSupprGaz.InsertString(-1,StrSymbole);
	}

	m_nIndexSupprGaz = -1;
	UpdateData(FALSE);
}








////////////////////////////////////////////////////////////////////////////////////
//
// CDialogueGaz

IMPLEMENT_DYNAMIC(CDialogueGaz, CPropertySheet)

CDialogueGaz::CDialogueGaz(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDialogueGaz::CDialogueGaz(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_AjoutGaz);
	AddPage(&m_ModifGaz);
	AddPage(&m_SupprGaz);

	SetTitle("Ajouter/Modifier un gaz");
}

CDialogueGaz::~CDialogueGaz()
{
}


BEGIN_MESSAGE_MAP(CDialogueGaz, CPropertySheet)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDialogueGaz









