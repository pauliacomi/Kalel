// Dialogue_cellule.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kalel.h"
#include "DialogCell.h"




//////////////////////////////////////////////////////////////////////////////////////
			//------------------------------------------------------
			//----------------- Cell Property Sheet------------------ 
			//------------------------------------------------------
			//
			//  Main class, all property pages will be added here
			//
//////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CDialogueCellule, CPropertySheet)

CDialogueCellule::CDialogueCellule(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CDialogueCellule::CDialogueCellule(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_AjoutCellule);
	AddPage(&m_ModifCellule);
	AddPage(&m_SupprCellule);

	CString title;
	title.Format(TITLE_DIALOG_CELL);
	SetTitle(title);
}

CDialogueCellule::~CDialogueCellule()
{
}

BOOL CDialogueCellule::OnInitDialog()
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

BEGIN_MESSAGE_MAP(CDialogueCellule, CPropertySheet)
END_MESSAGE_MAP()


// Gestionnaires de messages de CDialogueCellule










//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Add Cell Property Page------------------ 
				//------------------------------------------------------
				//
				//  For adding a new cell
				//
//////////////////////////////////////////////////////////////////////////////////////



IMPLEMENT_DYNAMIC(CAjoutCellule, CPropertyPage)

CAjoutCellule::CAjoutCellule()
	: CPropertyPage(CAjoutCellule::IDD)
	, m_StrAjoutNumeroCellule(_T(""))
	, m_fAjoutVolumeCellule(0)
	, m_fAjoutVolumeCalo(0)
	, m_strMessageAjoutCellule(_T(""))
{

}

CAjoutCellule::~CAjoutCellule()
{
}

void CAjoutCellule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_AJOUT_NUMERO_CELLULE, m_StrAjoutNumeroCellule);
	DDX_Text(pDX, IDC_EDIT_AJOUT_VOLUME_CELLULE, m_fAjoutVolumeCellule);
	DDX_Control(pDX, IDC_SPIN_AJOUT_VOLUME_CELLULE, m_SpinAjoutVolumeCellule);
	DDX_Text(pDX, IDC_EDIT_AJOUT_VOLUME_CALO, m_fAjoutVolumeCalo);
	DDX_Control(pDX, IDC_SPIN_AJOUT_VOLUME_CALO, m_SpinAjoutVolumeCalo);
	DDX_Control(pDX, IDC_EDIT_MESSAGE_AJOUT_CELLULE, m_EditMessageAjoutCellule);
	DDX_Text(pDX, IDC_EDIT_MESSAGE_AJOUT_CELLULE, m_strMessageAjoutCellule);
}


BEGIN_MESSAGE_MAP(CAjoutCellule, CPropertyPage)
	ON_BN_CLICKED(IDC_ADD_CELL, &CAjoutCellule::OnBnClickedAjouter)
END_MESSAGE_MAP()


// Gestionnaires de messages de CAjoutCellule


BOOL CAjoutCellule::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// On initialise juste des boutons spin

	m_SpinAjoutVolumeCellule.SetRange(0,10000);
	m_SpinAjoutVolumeCellule.SetPos(0.000);
	m_SpinAjoutVolumeCellule.SetInc(-0.001);
	m_SpinAjoutVolumeCellule.SetFormat("%1.3f");
	m_SpinAjoutVolumeCellule.UpdateBuddy();

	m_SpinAjoutVolumeCalo.SetRange(0,10000);
	m_SpinAjoutVolumeCalo.SetPos(0.000);
	m_SpinAjoutVolumeCalo.SetInc(-0.001);
	m_SpinAjoutVolumeCalo.SetFormat("%1.3f");
	m_SpinAjoutVolumeCalo.UpdateBuddy();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}



void CAjoutCellule::OnBnClickedAjouter()
{
	// Mise à jour des variables attachées aux contrôles
	UpdateData(TRUE);

	CString message;

	// On vérifie si les champs ne sont pas vides
	if(m_StrAjoutNumeroCellule == _T("") || m_fAjoutVolumeCellule == 0 || m_fAjoutVolumeCalo == 0)
	{
		message.Format(ERROR_FIELDS_NOT_FILLED);
		m_strMessageAjoutCellule = message + "\r\n";
	}
	else
	{	// On vérifie que la cellule ne soit pas déjà initialisé
		if (DoublonNumeroCellule(m_StrAjoutNumeroCellule.GetBuffer())) {
			message.Format(ERROR_NAME_USED);
			m_strMessageAjoutCellule = message + "\r\n";
		}

		else
		{	// On vérifie que le volume total de la cellule soit plus important que 
			// le volume du calo dans la cellule
			if(m_fAjoutVolumeCellule < m_fAjoutVolumeCalo)
			{
				message.Format(ERROR_CELL_VCALO_CCELL);
				m_strMessageAjoutCellule = message + "\r\n";
			}
			// Si toutes les conditions sont bonnes, on peut rajouter l'experimentateur dans un fichier XML
			else
			{
				Rajout_Cellule(m_StrAjoutNumeroCellule.GetBuffer(),m_fAjoutVolumeCellule,m_fAjoutVolumeCalo);

				m_strMessageAjoutCellule.Format(TEXT_CELL_ADDED, m_StrAjoutNumeroCellule);
				m_StrAjoutNumeroCellule=_T("");
				m_fAjoutVolumeCellule=0;
				m_fAjoutVolumeCalo=0;
			}
		}
	}
	// Pour afficher les messages, on met à jour les contrôles
	UpdateData(FALSE);

}









//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Modify Cell Property Page------------------ 
				//------------------------------------------------------
				//
				//  For modifying existing cells
				//
//////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CModifCellule, CPropertyPage)

CModifCellule::CModifCellule()
	: CPropertyPage(CModifCellule::IDD)
	, m_nIndexModifCellule(0)
	, m_StrModifNumeroCellule(_T(""))
	, m_fModifVolumeCellule(0)
	, m_fModifVolumeCalo(0)
	, m_strMessageModifCellule(_T(""))
{

}

CModifCellule::~CModifCellule()
{
}

void CModifCellule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_MODIF_CELLULE, m_CBModifCellule);
	DDX_CBIndex(pDX, IDC_COMBO_MODIF_CELLULE, m_nIndexModifCellule);
	DDX_Text(pDX, IDC_EDIT_MODIF_NUMERO_CELLULE, m_StrModifNumeroCellule);
	DDX_Text(pDX, IDC_EDIT_MODIF_VOLUME_CELLULE, m_fModifVolumeCellule);
	DDX_Control(pDX, IDC_SPIN_MODIF_VOLUME_CELLULE, m_SpinModifVolumeCellule);
	DDX_Text(pDX, IDC_EDIT_MODIF_VOLUME_CALO, m_fModifVolumeCalo);
	DDX_Control(pDX, IDC_SPIN_MODIF_VOLUME_CALO, m_SpinModifVolumeCalo);
	DDX_Control(pDX, IDC_EDIT_MESSAGE_MODIF_CELLULE, m_EditMessageModifCellule);
	DDX_Text(pDX, IDC_EDIT_MESSAGE_MODIF_CELLULE, m_strMessageModifCellule);

}


BEGIN_MESSAGE_MAP(CModifCellule, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_MODIF_CELLULE, &CModifCellule::OnCbnSelchangeComboModifCellule)
	ON_BN_CLICKED(IDC_MODIFY_CELL, &CModifCellule::OnBnClickedModifier)
END_MESSAGE_MAP()


// Gestionnaires de messages de CModifCellule



BOOL CModifCellule::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// On initialise seulement les boutons spin

	m_SpinModifVolumeCellule.SetRange(0,10000);
	m_SpinModifVolumeCellule.SetPos(0.000);
	m_SpinModifVolumeCellule.SetInc(-0.001);
	m_SpinModifVolumeCellule.SetFormat("%1.3f");
	m_SpinModifVolumeCellule.UpdateBuddy();

	m_SpinModifVolumeCalo.SetRange(0,10000);
	m_SpinModifVolumeCalo.SetPos(0.000);
	m_SpinModifVolumeCalo.SetInc(-0.001);
	m_SpinModifVolumeCalo.SetFormat("%1.3f");
	m_SpinModifVolumeCalo.UpdateBuddy();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION : les pages de propriétés OCX devraient retourner FALSE
}


// OnSetActive : fonction utilisée à chaque fois qu'on active la page
BOOL CModifCellule::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}


// Lorsqu'on selectionne un expérimentateur à modifier
void CModifCellule::OnCbnSelchangeComboModifCellule()
{
	// On récupère les données des contrôles pour mettre à jour les variables
	UpdateData(TRUE);

	int index = m_nIndexModifCellule;
	// On affiche les nom et prenom de l'expérimentateur désigné
	string numero_modif = list_modif_cellule[index].numero;
	m_StrModifNumeroCellule.Format(_T("%s"),numero_modif.c_str());
	m_fModifVolumeCellule = list_modif_cellule[index].volume_total;
	m_fModifVolumeCalo = list_modif_cellule[index].volume_calo;

	UpdateData(FALSE);
}


// Lorsqu'on clique sur le bouton 'modifier'
void CModifCellule::OnBnClickedModifier()
{
	// On met à jour les variables
	UpdateData(TRUE);

	int index = m_nIndexModifCellule;

	if(index != -1)
	{
		// On modifie la cellule n°'index+1'
		if(m_StrModifNumeroCellule == _T("") || m_fModifVolumeCellule == 0 || m_fModifVolumeCalo == 0)
		{
			m_strMessageModifCellule.Format(ERROR_FIELDS_NOT_FILLED);
		}
		else
		{
			// On verifie que si on donne un nouveau numéro qui n'est pas déjà pris à la cellule
			if(
				DoublonNumeroCellule(m_StrModifNumeroCellule.GetBuffer()))
				m_strMessageModifCellule.Format(ERROR_NAME_USED);
			else
			{
				// On vérifie que le volume total de la cellule soit plus important que 
				// le volume du calo dans la cellule
				if (m_fModifVolumeCellule < m_fModifVolumeCalo) {
					m_strMessageModifCellule.Format(ERROR_CELL_VCALO_CCELL);
				}
				else
				{
					// Si tout se passe bien, on le signale dans la boite de dialogue
					if(Modif_Cellule(m_StrModifNumeroCellule.GetBuffer(), m_fModifVolumeCellule, 
									 m_fModifVolumeCalo, m_nIndexModifCellule))
					{
						m_strMessageModifCellule.Format(TEXT_CELL_MODIFIED, list_modif_cellule[index].numero.c_str(), m_StrModifNumeroCellule);
						ReinitialisationComboBox();
					}
					else
					{	// Si la modification ne se fait pas... On le signale dans la boite de dialogue
						m_strMessageModifCellule.Format(ERROR_MODIFICATION);
					}
				}
			}
		}
	}
	else {
		m_strMessageModifCellule.Format(ERROR_SELECTED);
	}

	UpdateData(FALSE);
}

// On réinitialise la liste des cellules en cas ou on l'on ait modifié
void CModifCellule::ReinitialisationComboBox()
{
	// On efface la liste des cellules et on la réinitialise en récupérant 
	// les données dans le fichier XML adéquat
	list_modif_cellule.clear();
	list_modif_cellule=GetCellules();

	// On réinitialise le ComboBox
	m_CBModifCellule.ResetContent();  // On efface la contenance de m_CBModifCellule
	for(UINT i=0;i<list_modif_cellule.size();i++)
	{
		CString StrNumero;
		StrNumero.Format(_T("%s"),list_modif_cellule[i].numero.c_str());
		m_CBModifCellule.InsertString(-1,StrNumero);
	}

	m_nIndexModifCellule = -1;
	m_StrModifNumeroCellule=_T("");
	m_fModifVolumeCellule=0;
	m_fModifVolumeCalo=0;

	UpdateData(FALSE);
}









//////////////////////////////////////////////////////////////////////////////////////
				//------------------------------------------------------
				//--------------Delete Cell Property Page------------------ 
				//------------------------------------------------------
				//
				//  For deleting a cell
				//
//////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CSupprCellule, CPropertyPage)

CSupprCellule::CSupprCellule()
	: CPropertyPage(CSupprCellule::IDD)
	, m_nIndexSupprNumeroCellule(0)
	, m_fSupprVolumeCellule(0)
	, m_fSupprVolumeCalo(0)
	, m_strMessageSupprCellule(_T(""))
{

}

CSupprCellule::~CSupprCellule()
{
}

void CSupprCellule::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_SUPPR_NUMERO_CELLULE, m_CBSupprNumeroCellule);
	DDX_CBIndex(pDX, IDC_COMBO_SUPPR_NUMERO_CELLULE, m_nIndexSupprNumeroCellule);
	DDX_Text(pDX, IDC_EDIT_SUPPR_VOLUME_CELLULE, m_fSupprVolumeCellule);
	DDX_Control(pDX, IDC_EDIT_SUPPR_VOLUME_CELLULE, m_EditSupprVolumeCellule);
	DDX_Text(pDX, IDC_EDIT_SUPPR_VOLUME_CALO, m_fSupprVolumeCalo);
	DDX_Control(pDX, IDC_EDIT_SUPPR_VOLUME_CALO, m_EditSupprVolumeCalo);
	DDX_Control(pDX, IDC_EDIT_MESSAGE_SUPPR_CELLULE, m_EditMessageSupprCellule);
	DDX_Text(pDX, IDC_EDIT_MESSAGE_SUPPR_CELLULE, m_strMessageSupprCellule);
}


BEGIN_MESSAGE_MAP(CSupprCellule, CPropertyPage)
	ON_CBN_SELCHANGE(IDC_COMBO_SUPPR_NUMERO_CELLULE, &CSupprCellule::OnCbnSelchangeComboSupprNumeroCellule)
	ON_BN_CLICKED(IDC_DELETE_CELL, &CSupprCellule::OnBnClickedSupprimer)
END_MESSAGE_MAP()


// Gestionnaires de messages de CSupprCellule

// Comme dans 'EXPERIMENT_TYPE_MODIFY', on réinitialise le ComboBox
BOOL CSupprCellule::OnSetActive()
{
	ReinitialisationComboBox();

	return CPropertyPage::OnSetActive();
}

void CSupprCellule::OnCbnSelchangeComboSupprNumeroCellule()
{
	// On met à jour la variable m_IndexSupprGaz
	// Si on clique sur 'supprimer', on effacera le gaz 
	// de l'index m_IndexSupprGaz mise à jour.
	UpdateData(TRUE);
	// On affiche les données de la cellule qu'on veux effacer
	m_fSupprVolumeCellule = list_suppr_cellule[m_nIndexSupprNumeroCellule].volume_total;
	m_fSupprVolumeCalo = list_suppr_cellule[m_nIndexSupprNumeroCellule].volume_calo;
	UpdateData(FALSE);
}

void CSupprCellule::OnBnClickedSupprimer()
{
	int index = m_nIndexSupprNumeroCellule;
	int Cellule_A_Supprime = index + 1;

	if (index != -1)
	{
		// On supprime l'expérimentateur de l'index : m_IndexSupprExperimentateur
		if(Suppression_Cellule(index))
		{
			// Si tout se passe bien, on le signale dans la boite de dialogue
			// Et on réinitialise le ComboBox
			m_strMessageSupprCellule.Format(TEXT_CELL_DELETED,	list_suppr_cellule[index].numero.c_str());
			ReinitialisationComboBox();
		}
		else
		{	// Si ça ne marche pas, on le signale dans la boite de dialogue
			m_strMessageSupprCellule.Format(ERROR_DELETION);
		}
	}
	else
		m_strMessageSupprCellule.Format(ERROR_SELECTED);
	
	UpdateData(FALSE);
}

void CSupprCellule::ReinitialisationComboBox()
{
	list_suppr_cellule.clear();
	list_suppr_cellule=GetCellules();

	m_CBSupprNumeroCellule.ResetContent();
	for(UINT i=0;i<list_suppr_cellule.size();i++)
	{
		CString StrNumero;
		StrNumero.Format(_T("%s"),list_suppr_cellule[i].numero.c_str());
		m_CBSupprNumeroCellule.InsertString(-1,StrNumero);
	}

	m_nIndexSupprNumeroCellule=-1;
	m_fSupprVolumeCellule=0;
	m_fSupprVolumeCalo=0;
	UpdateData(FALSE);
}
