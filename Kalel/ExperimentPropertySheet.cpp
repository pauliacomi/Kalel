// ExperimentPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "Kalel.h"
#include "ExperimentPropertySheet.h"


#include "DefineStages.h"			// For the types of experiments used
#include "DefinePostMessages.h"		// For custom message definitions



// ExperimentPropertySheet

IMPLEMENT_DYNAMIC(ExperimentPropertySheet, CMFCPropertySheet)

ExperimentPropertySheet::ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

ExperimentPropertySheet::ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
	, numberOfAdsorptions(3)
	, numberOfDesorptions(1)
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
		m_adsorption = new TabDoses(i+1);

		// Insert page
		std::vector<TabDoses*>::iterator pos = adsorptionTabs.begin() + i;
		adsorptionTabs.insert(pos, m_adsorption);
	}
	for (int i = 0; i < numberOfDesorptions; i++)
	{
		m_desorption = new TabDesorption(i+1);

		// Insert page
		std::vector<TabDesorption*>::iterator pos = desorptionTabs.begin() + i;
		desorptionTabs.insert(pos, m_desorption);
	}
	
	// Resize the bool storage for tab availability
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
	for (size_t i = 0; i < adsorptionTabs.size(); i++)
	{
		delete adsorptionTabs[i];
	}
	for (size_t i = 0; i < desorptionTabs.size(); i++)
	{
		delete desorptionTabs[i];
	}
}

BOOL ExperimentPropertySheet::OnInitDialog()
{
	BOOL bResult = CMFCPropertySheet::OnInitDialog();

	//
	// Adding new buttons

	// Find a bottom-left point for the edit control in the client area
	CRect rect;
	GetClientRect(&rect);

	int nOffset = 6;
	int nWidth = 70;
	int nHeight = 22;

	int nLeft = rect.left + nOffset;
	int nTop = rect.top + (rect.Height() - nHeight) - nOffset;
	int nRight = nLeft + nWidth;
	int nBottom = nTop + nHeight;

	// finally create the new buttons
	m_addAdsorption.Create(_T("+ADS"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nLeft, nTop, nRight, nBottom) , this, IDC_PLUSADS);
	m_addAdsorption.SetFont(GetFont());
	m_addDesorption.Create(_T("+DES"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nLeft + nWidth + nOffset, nTop, nRight + nWidth, nBottom), this, IDC_PLUSDES);
	m_addDesorption.SetFont(GetFont());

	// Done adding buttons
	//

	return bResult;
}


// Sets the experiment type as manual
// It allows only the general tab to be viewed
void ExperimentPropertySheet::SetProprietiesManual(void)
{
	if (experimentType != EXPERIMENT_TYPE_MANUAL)
	{
		experimentType = EXPERIMENT_TYPE_MANUAL;

		// Set the name of the title
		CString title;
		title.Format(TITLE_MANUALPARAM);
		SetTitle(title);

		// Add the general tab
		AddTab(p_generalPP, tab_general);

		// Make sure the tab data is blank
		Reinitialisation(false);
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

		// Add all the tabs
		AddTab(p_generalPP, tab_general);
		AddTab(p_diversPP, tab_divers);
		AddStepTabs();

		// Make sure the tab data is blank
		Reinitialisation(true);
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
	
	// Add all the tabs 
	AddTab(p_generalPP, tab_general);
	AddTab(p_diversPP, tab_divers);
	AddStepTabs();

	// Grey out the ones that should not be modified
	m_general.GreyOut(FALSE);

	// Add only allowed tabs
	switch (etape_en_cours)
	{
	case STAGE_TEMP:
	case STAGE_EQUILIBRATION:
	case STAGE_CONTINUOUS_ADSORPTION:
		break;
	case STAGE_ADSORPTION:

		break;
	case STAGE_DESORPTION:
		break;
	default:
		break;
	}


}

// Reinitialise the data in all the tabs
void ExperimentPropertySheet::Reinitialisation(bool automatic)
{
	m_general.Reinitialisation();

	if (automatic)
	{
		m_divers.Reinitialisation();

		for (size_t i = 0; i < adsorptionTabs.size(); i++)
		{
			adsorptionTabs[i]->Reinitialisation();
		}

		for (size_t i = 0; i < desorptionTabs.size(); i++)
		{
			desorptionTabs[i]->Reinitialisation();
		}

		m_continuousAdsorption.Reinitialisation();
	}
}

// Asks all the tabs to be added
void ExperimentPropertySheet::AddStepTabs() {
	for (size_t i = 0; i < adsorptionTabs.size(); i++)
	{
		AddTab(adsorptionTabs[i], nb_permanent_tabs + i);
	}
	for (size_t i = 0; i < desorptionTabs.size(); i++)
	{
		AddTab(desorptionTabs[i], nb_permanent_tabs + numberOfAdsorptions + i);
	}
	//AddTab(p_adsorptioncontinuePP);
}

// Asks all the tabs to be removed
void ExperimentPropertySheet::RemoveStepTabs()
{
	for (size_t i = 0; i < adsorptionTabs.size(); i++)
	{
		RemoveTab(adsorptionTabs[i], nb_permanent_tabs + i);
	}
	for (size_t i = 0; i < desorptionTabs.size(); i++)
	{
		RemoveTab(desorptionTabs[i], nb_permanent_tabs + numberOfAdsorptions + i);
	}
	//AddTab(p_adsorptioncontinuePP);
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
	//{{AFX_MSG_MAP(CMyPropertySheet)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_PLUSADS, OnButtonAddAdsorption)					// Clicking the add adsorption button
	ON_BN_CLICKED(IDC_PLUSDES, OnButtonAddDesorption)					// Clicking the add desorption button
	ON_MESSAGE(WM_PP_ADSORPTION_DELETE, OnButtonRemoveAdsorption)		// Clicking the delete button, inside one of the property sheets
	ON_MESSAGE(WM_PP_DESORPTION_DELETE, OnButtonRemoveDesorption)		// Clicking the delete button, inside one of the property sheets
END_MESSAGE_MAP()


// ExperimentPropertySheet message handlers

void ExperimentPropertySheet::OnButtonAddAdsorption()
{
	if (experimentType == EXPERIMENT_TYPE_MANUAL) {			// Check if the experiment type allows the addition of tabs
		AfxMessageBox(ERROR_NOT_APPLICABLE);
	}
	else {
		if (numberOfAdsorptions > nb_max_tabs) {			// Check if the maximum allowed doses have been reached, yes i know it's bad to hardcode it
			AfxMessageBox(ERROR_TOO_MANY_TABS);
		}
		else {

			// Set the index for where to insert the tab. If no adsorption tab is selected then insert at the end
			int index = GetActiveIndex();
			if (index < nb_permanent_tabs || index >= nb_permanent_tabs + numberOfAdsorptions) {
				index = nb_permanent_tabs + numberOfAdsorptions;
			}
			else
			{
				index++;
			}

			// increment the number of adsorption tabs
			numberOfAdsorptions++;

			// Set the position of an iterator where we want to insert the new element
			std::vector<bool>::iterator pos = availableTabs.begin() + index;
			availableTabs.insert(pos, false);

			// Create the page
			m_adsorption = new TabDoses(index - nb_permanent_tabs + 1);

			// Insert page
			std::vector<TabDoses*>::iterator pos2 = adsorptionTabs.begin() + (index - nb_permanent_tabs);
			adsorptionTabs.insert(pos2, m_adsorption);


			// Rename all remaining pages
			for (size_t i = 0; i < numberOfAdsorptions; i++)
			{
				adsorptionTabs[i]->Rename(i + 1);
			}

			// Reset the tabs
			RemoveStepTabs();
			AddStepTabs();

			// Show new page
			SetActivePage(index);
		}
	}
}

void ExperimentPropertySheet::OnButtonAddDesorption(void)
{
	if (experimentType == EXPERIMENT_TYPE_MANUAL) {
		AfxMessageBox(ERROR_NOT_APPLICABLE);
	}
	else {
		if (numberOfDesorptions > nb_max_tabs) {
			AfxMessageBox(ERROR_TOO_MANY_TABS);
		}
		else {

			// Set the index for where to insert the tab. If no adsorption tab is selected then insert at the end
			int index = GetActiveIndex();
			if (index < nb_permanent_tabs + numberOfAdsorptions || index >= nb_permanent_tabs + numberOfAdsorptions + numberOfDesorptions) {
				index = nb_permanent_tabs + numberOfAdsorptions + numberOfDesorptions;
			}
			else
			{
				index++;
			}

			// increment the number of adsorption tabs
			numberOfDesorptions++;

			// Set the position of an iterator where we want to insert the new element
			std::vector<bool>::iterator pos = availableTabs.begin() + index;
			availableTabs.insert(pos, false);

			// Create the page
			m_desorption = new TabDesorption(index - numberOfAdsorptions - nb_permanent_tabs + 1);

			// Insert page
			std::vector<TabDesorption*>::iterator pos2 = desorptionTabs.begin() + (index - numberOfAdsorptions - nb_permanent_tabs);
			desorptionTabs.insert(pos2, m_desorption);


			// Rename all remaining pages
			for (size_t i = 0; i < numberOfDesorptions; i++)
			{
				desorptionTabs[i]->Rename(i + 1);
			}

			// Reset the tabs
			RemoveStepTabs();
			AddStepTabs();

			// Show new page
			SetActivePage(index);
		}
	}
}

LRESULT ExperimentPropertySheet::OnButtonRemoveAdsorption(WPARAM, LPARAM lParam)
{
	int pageToRemove = static_cast<int>(lParam);

	SetActivePage(0);															// First go to page number 1
	RemovePage(nb_permanent_tabs + pageToRemove - 1);							// Then remove from propertysheet list

	// Erase bool from available tabs
	std::vector<bool>::iterator pos = availableTabs.begin() + (nb_permanent_tabs + pageToRemove - 1);
	availableTabs.erase(pos);
	
	// Erase the page itself from the list of pages
	std::vector<TabDoses*>::iterator pos2 = adsorptionTabs.begin() + pageToRemove - 1;
	adsorptionTabs.erase(pos2);

	numberOfAdsorptions--;														// Decrement the number of adsorption 

	// Rename all remaining pages
	for (size_t i = 0; i < numberOfAdsorptions; i++)
	{
		adsorptionTabs[i]->Rename(i+1);
	}

	// Reset the tabs
	RemoveStepTabs();
	AddStepTabs();

	return 0;
}

LRESULT ExperimentPropertySheet::OnButtonRemoveDesorption(WPARAM, LPARAM lParam)
{
	int pageToRemove = static_cast<int>(lParam);

	SetActivePage(0);																	// First go to page number 1
	RemovePage(nb_permanent_tabs + numberOfAdsorptions + pageToRemove - 1);				// Then remove from propertysheet list

																						// Erase bool from available tabs
	std::vector<bool>::iterator pos = availableTabs.begin() + (nb_permanent_tabs + numberOfAdsorptions + pageToRemove - 1);
	availableTabs.erase(pos);

	// Erase the page itself from the list of pages
	std::vector<TabDesorption *>::iterator pos2 = desorptionTabs.begin() + pageToRemove - 1;
	desorptionTabs.erase(pos2);

	numberOfDesorptions--;														// Decrement the number of adsorption 

	// Rename all remaining pages
	for (size_t i = 0; i < numberOfDesorptions; i++)
	{
		desorptionTabs[i]->Rename(i + 1);
	}

	// Reset the tabs
	RemoveStepTabs();
	AddStepTabs();

	return 0;
}
