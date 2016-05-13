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
	: CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
	, numberOfAdsorptions(2)
	, numberOfDesorptions(2)
{
	// Choose the view of the property page
	SetLook(CMFCPropertySheet::PropSheetLook_List, 150);

	// Link the pointers
	p_generalPP = &m_general;
	p_diversPP = &m_divers;
	p_adsorptioncontinuePP = &m_continuousAdsorption;

	// Dynamically generate the tabs
	for (int i = 0; i < numberOfAdsorptions; i++)
	{
		AddAdsorption(i);
	}
	for (int i = 0; i < numberOfDesorptions; i++)
	{
		AddDesorption(i);
	}
	
	availableTabs.resize(nb_permanent_tabs + numberOfAdsorptions + numberOfDesorptions);
	for (size_t i = 0; i < availableTabs.size(); i++)
	{
		availableTabs[i] = false;
	}

	// Instantiate the property sheet as a manual experiment no matter what the user chooses afterwards
	SetProprietiesManual();
}

ExperimentPropertySheet::~ExperimentPropertySheet()
{
	// Must delete all created tabs
	for (int i = 0; i < adsorptionTabs.size(); i++)
	{
		delete adsorptionTabs[i];
	}
	for (int i = 0; i < desorptionTabs.size(); i++)
	{
		delete desorptionTabs[i];
	}
}


// Sets the experiment type as manual
// It allows only the general tab to be viewed
void ExperimentPropertySheet::SetProprietiesManual(void)
{
	if (experimentType != EXPERIMENT_TYPE_MANUAL)						// cancel might destroy everything
	{
		experimentType = EXPERIMENT_TYPE_MANUAL;

		// Set the name of the title
		CString title;
		title.Format(TITLE_MANUALPARAM);
		SetTitle(title);

		// Remove any existing tabs and add the general tab
		RemoveAllTabs();
		AddTab(p_generalPP, tab_general);

		// Make sure the tab data is blank
		ReinitialisationManual();
	}

}

// Sets the experiment type as automatic
// It allows all the tabs to be viewed
void ExperimentPropertySheet::SetProprietiesAuto(void)
{
	if (experimentType != EXPERIMENT_TYPE_AUTO)
	{
		experimentType = EXPERIMENT_TYPE_AUTO;

		// Set the name of the title
		CString title;
		title.Format(TITLE_AUTOPARAM);
		SetTitle(title);

		// Remove any existing tabs and re-add all the tabs
		RemoveAllTabs();
		AddAllTabs();

		// Make sure the tab data is blanks
		ReinitialisationAuto();
	}
}

// Sets the experiment type as modified
// It allows only the tabs which have parameters that can be mofified to be showed
void ExperimentPropertySheet::SetProprietiesModif(int etape_en_cours)
{
	// Set the name of the title
	CString title;
	title.Format(TITLE_MODIFPARAM);
	SetTitle(title);

	// Remove any existing tabs
	RemoveAllTabs();
	
	if (experimentType == EXPERIMENT_TYPE_MANUAL)
	{
		AddTab(p_generalPP, tab_general);
	}
	if (experimentType == EXPERIMENT_TYPE_MANUAL)
	{

	}
	
	// Add the tabs needed
	AddTab(p_generalPP, tab_general);


	// Add only allowed tabs
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

// Reinitialise the data in all the tabs
void ExperimentPropertySheet::ReinitialisationAuto()
{
	m_general.Reinitialisation();
	m_divers.Reinitialisation();

	for (int i = 0; i < adsorptionTabs.size(); i++)
	{
		adsorptionTabs[i]->Reinitialisation();
	}
	for (int i = 0; i < desorptionTabs.size(); i++)
	{
		desorptionTabs[i]->Reinitialisation();
	}

	m_continuousAdsorption.Reinitialisation();
}

// Reinitialise the data only in the general tab
void ExperimentPropertySheet::ReinitialisationManual()
{
	m_general.Reinitialisation();
}


// Asks all the tabs to be added
void ExperimentPropertySheet::AddAllTabs() {
	AddTab(p_generalPP, tab_general);
	AddTab(p_diversPP, tab_divers);
	for (int i = 0; i < adsorptionTabs.size(); i++)
	{
		AddTab(adsorptionTabs[i], nb_permanent_tabs + i);
	}
	for (int i = 0; i < desorptionTabs.size(); i++)
	{
		AddTab(desorptionTabs[i], nb_permanent_tabs + numberOfAdsorptions + i);
	}
	//AddTab(p_adsorptioncontinuePP);
}

// Asks all the tabs to be removed
void ExperimentPropertySheet::RemoveAllTabs()
{
	RemoveTab(p_generalPP, tab_general);
	RemoveTab(p_diversPP, tab_divers);
	for (int i = 0; i < adsorptionTabs.size(); i++)
	{
		RemoveTab(adsorptionTabs[i], nb_permanent_tabs + i);
	}
	for (int i = 0; i < desorptionTabs.size(); i++)
	{
		RemoveTab(desorptionTabs[i], nb_permanent_tabs + numberOfAdsorptions + i);
	}
	//AddTab(p_adsorptioncontinuePP);
}

void ExperimentPropertySheet::AddAdsorption(int i)
{
	CString tabtitle;
	tabtitle.Format(_T("Adsorption %d"), i + 1);

	m_dose = new TabDoses(tabtitle);
	adsorptionTabs.push_back(m_dose);
	adsorptionTabPointers.push_back(adsorptionTabs[i]);
}

void ExperimentPropertySheet::AddDesorption(int i)
{
	CString tabtitle;
	tabtitle.Format(_T("Desorption %d"), i + 1);

	m_desorption = new TabDesorption(tabtitle);
	desorptionTabs.push_back(m_desorption);
	desorptionTabPointers.push_back(desorptionTabs[i]);
}



// Adds a tab, checking if it is available first
void ExperimentPropertySheet::AddTab(CPropertyPage * tab, int checkTab) {
	if (availableTabs[checkTab] == false)
	{
		AddPage(tab);
		availableTabs[checkTab] = true;
	}
}

// Removes a tab, checking if it is available first
void ExperimentPropertySheet::RemoveTab(CPropertyPage * tab, int checkTab) {
	if (availableTabs[checkTab] == true)
	{
		RemovePage(tab);
		availableTabs[checkTab] = false;
	}
}


BEGIN_MESSAGE_MAP(ExperimentPropertySheet, CMFCPropertySheet)
END_MESSAGE_MAP()

// ExperimentPropertySheet message handlers