// ExperimentPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "Kaolla.h"
#include "ExperimentPropertySheet.h"


#include "MessageBoxTexts.h"				// For the required strings
#include "Define_Experiment_Type.h"			// For the types of experiments used
#include "Define_Stages.h"					// For the definitions of the steps used


// ExperimentPropertySheet

IMPLEMENT_DYNAMIC(ExperimentPropertySheet, CMFCPropertySheet)

ExperimentPropertySheet::ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

ExperimentPropertySheet::ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// Link the pointers
	p_general = &m_general;
	p_divers = &m_divers;
	p_smalldoses = &m_smalldoses;
	p_mediumdoses = &m_mediumdoses;
	p_bigdoses = &m_bigdoses;

	/*
	p_desorption = &m_desorption;
	p_adsorptioncontinue = &m_adsorptioncontinue;*/

	for (int i = 0; i<nb_tabs; i++)
		availableTabs[i] = FALSE;

	AddTab(p_general, tab_general);

	///
	AddAllTabs();
	///
	
	experimentType = EXPERIMENT_TYPE_MANUAL;

	// Choose the view of the property page
	SetLook(CMFCPropertySheet::PropSheetLook_List, 150);

	// Set the name of the title
	CString title;
	title.Format(TITLE_MANUALPARAM);
	SetTitle(title);
}

ExperimentPropertySheet::~ExperimentPropertySheet()
{
}

void ExperimentPropertySheet::SetProprietiesManual(void)
{
	if (experimentType != EXPERIMENT_TYPE_MANUAL)
	{
		experimentType = EXPERIMENT_TYPE_MANUAL;

		// Set the name of the title
		CString title;
		title.Format(TITLE_MANUALPARAM);
		SetTitle(title);

		RemoveAllTabs();
		AddTab(p_general, tab_general);

		ReinitialisationManual();
	}

}

void ExperimentPropertySheet::SetProprietiesAuto(void)
{
	if (experimentType != EXPERIMENT_TYPE_AUTO)
	{
		// Set the name of the title
		CString title;
		title.Format(TITLE_AUTOPARAM);
		SetTitle(title);

		experimentType = EXPERIMENT_TYPE_AUTO;

		RemoveAllTabs();
		AddAllTabs();

		ReinitialisationAuto();
	}
}


void ExperimentPropertySheet::SetProprietiesModif(int etape_en_cours)
{
	experimentType = EXPERIMENT_TYPE_MODIFY;

	// Set the name of the title
	CString title;
	title.Format(TITLE_MODIFPARAM);
	SetTitle(title);

	RemoveAllTabs();

	/*switch (etape_en_cours)
	{
	case STAGE_TEMP:
	case STAGE_EQ_LINE:
	case STAGE_CONT_ADSORPTON:
		RajouterPetitesDoses();
		RajouterMoyennesDoses();
		RajouterGrandesDoses();
		RajouterDesorption();
		break;
	case STAGE_SMALL_DOSES:
		RajouterPetitesDoses();
		RajouterMoyennesDoses();
		RajouterGrandesDoses();
		RajouterDesorption();
		m_smalldoses.GriserCheck_PetitesDoses();
		break;
	case STAGE_MEDIUM_DOSES:
		RajouterMoyennesDoses();
		RajouterGrandesDoses();
		RajouterDesorption();
		m_mediumdoses.GriserCheck_MoyennesDoses();
		break;
	case STAGE_BIG_DOSES:
		RajouterGrandesDoses();
		RajouterDesorption();
		m_grandesdoses.GriserCheck_GrandesDoses();
		break;
	case STAGE_DESORPTION:
		RajouterDesorption();
		m_desorption.GriserCheck_Desorption();
		break;
	default:
		break;
	}*/
}

void ExperimentPropertySheet::ReinitialisationAuto()
{
	m_general.Reinitialisation();
	m_divers.Reinitialisation();

	/*m_petitesdoses.Reinitialisation();
	m_moyennesdoses.Reinitialisation();
	m_grandesdoses.Reinitialisation();
	m_desorption.Reinitialisation();
	m_adsorptioncontinue.Reinitialisation()*/;
}

void ExperimentPropertySheet::ReinitialisationManual()
{
	m_general.Reinitialisation();
}


BEGIN_MESSAGE_MAP(ExperimentPropertySheet, CMFCPropertySheet)
END_MESSAGE_MAP()


void ExperimentPropertySheet::AddAllTabs() {
	AddTab(p_general, tab_general);
	AddTab(p_divers, tab_divers);
	AddTab(p_smalldoses, tab_adsorption_continue);
	AddTab(p_mediumdoses, tab_petites_doses);
	AddTab(p_bigdoses, tab_moyennes_doses);

	/*AddTab(p_desorption, tab_grandes_doses);
	AddTab(p_adsorptioncontinue, tab_desorption);*/
}

void ExperimentPropertySheet::RemoveAllTabs()
{
}

void ExperimentPropertySheet::AddTab(CPropertyPage * tab, int checkTab) {
	if (availableTabs[checkTab] == FALSE)
	{
		AddPage(tab);
		availableTabs[checkTab] = TRUE;
	}
}

void ExperimentPropertySheet::RemoveTab(CPropertyPage * tab, int checkTab) {
	if (availableTabs[checkTab] == TRUE)
	{
		RemovePage(tab);
		availableTabs[checkTab] = FALSE;
	}
}


// ExperimentPropertySheet message handlers