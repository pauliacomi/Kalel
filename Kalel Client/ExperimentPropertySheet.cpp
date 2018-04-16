// ExperimentPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "ExperimentPropertySheet.h"

#include "DefineDialogMessages.h"							// For custom message definitions
#include "../Kalel Shared/Resources/DefineStages.h"			// For the types of experiments used
#include "StringTable.h"									// Error message strings

// Experiment settings class
#include "../Kalel Shared/Com Classes/ExperimentSettings.h"
#include "../Kalel Shared/Com Classes/Serialization.h"
#include "../Kalel Shared/Netcode/json.hpp"

#include <fstream>

using json = nlohmann::json;

IMPLEMENT_DYNAMIC(ExperimentPropertySheet, CMFCPropertySheet)

ExperimentPropertySheet::ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CMFCPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	// Choose the view of the property page
	SetLook(CMFCPropertySheet::PropSheetLook_List, 150);
}

ExperimentPropertySheet::ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	: CMFCPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	// Choose the view of the property page
	SetLook(CMFCPropertySheet::PropSheetLook_List, 150);
}

ExperimentPropertySheet::ExperimentPropertySheet(LPCTSTR pszCaption, MachineSettings * machineSettings, CWnd* pParentWnd, UINT iSelectPage)
	:ExperimentPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	m_general.PassSettings(machineSettings);
	// Choose the view of the property page
	SetLook(CMFCPropertySheet::PropSheetLook_List, 150);
}

ExperimentPropertySheet::~ExperimentPropertySheet()
{
}

void ExperimentPropertySheet::Initiate(const ExperimentSettings & experimentSettings)
{
	// initiate variables
	modified = false;

	// Save number of adsorption / desorptions
	numberOfAdsorptions = experimentSettings.dataAdsorption.size();
	numberOfDesorptions = experimentSettings.dataDesorption.size();

	// Populate tabs from the experiment settings
	// General tabs
	m_general.allSettings = experimentSettings.dataGeneral;
	m_divers.allSettings = experimentSettings.dataDivers;
	m_continuousAdsorption.allSettings = experimentSettings.dataContinuous;

	// Adsorption
	for (int i = 0; i < numberOfAdsorptions; i++)
	{
		auto tempTab = std::make_unique<TabDoses>(i + 1);
		tempTab->allSettings = experimentSettings.dataAdsorption[i];

		// Insert page
		adsorptionTabs.push_back(std::move(tempTab));
	}
	// Desorption
	for (int i = 0; i < numberOfDesorptions; i++)
	{
		auto tempTab = std::make_unique<TabDesorption>(i + 1);
		tempTab->allSettings = experimentSettings.dataDesorption[i];

		// Insert page
		desorptionTabs.push_back(std::move(tempTab));
	}

	// Resize the bool storage for tab availability
	availableTabs.resize(nb_permanent_tabs + numberOfAdsorptions + numberOfDesorptions);
	for (size_t i = 0; i < availableTabs.size(); i++)
	{
		availableTabs[i] = false;
	}
	
	// Instantiate the correct type of dialog
	switch (experimentSettings.experimentType)
	{
	case EXPERIMENT_TYPE_MANUAL:
		SetProprietiesManual();
		break;
	case EXPERIMENT_TYPE_AUTO:
		SetProprietiesAuto();
		break;
	case EXPERIMENT_TYPE_CONTINUOUS:
		SetProprietiesAutoCont();
		break;
	default:
		ASSERT(0);  // Should never be reached
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
	m_saveSettings.Create(_T("Save"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nLeft + 2 * nWidth + nOffset, nTop, nRight + 2 * nWidth, nBottom), this, IDC_SAVE_SETT);
	m_saveSettings.SetFont(GetFont());
	m_loadSettings.Create(_T("Load"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nLeft + 3 * nWidth + nOffset, nTop, nRight + 3 * nWidth, nBottom), this, IDC_LOAD_SETT);
	m_loadSettings.SetFont(GetFont());

	// Done adding buttons
	//
	
	return bResult;
}


// Sets the experiment type as manual
// It allows only the general tab to be viewed
void ExperimentPropertySheet::SetProprietiesManual(void)
{
	experimentType = EXPERIMENT_TYPE_MANUAL;

	// Set the name of the title
	CString title;
	title.Format(TITLE_MANUALPARAM);
	SetTitle(title);

	// Add the general tab
	AddTab(&m_general, tab_general);
}

// Sets the experiment type as automatic
// It allows all the tabs to be viewed
void ExperimentPropertySheet::SetProprietiesAuto(void)
{
	experimentType = EXPERIMENT_TYPE_AUTO;

	// Set the name of the title
	CString title;
	title.Format(TITLE_AUTOPARAM);
	SetTitle(title);

	// Add all the tabs
	AddTab(&m_general, tab_general);
	AddTab(&m_divers, tab_divers);
	AddStepTabs();
}

// Sets the experiment type as automatic
// It allows all the tabs to be viewed
void ExperimentPropertySheet::SetProprietiesAutoCont(void)
{
	experimentType = EXPERIMENT_TYPE_CONTINUOUS;

	// Set the name of the title
	CString title;
	title.Format(TITLE_AUTOPARAM);
	SetTitle(title);

	// Add all the tabs
	AddTab(&m_general, tab_general);
	AddTab(&m_divers, tab_divers);
	availableTabs.resize(nb_permanent_tabs + 1);
	AddTab(&m_continuousAdsorption, tab_adsorption_continue);
}


// Sets the experiment type as modified
// It allows only the tabs which have parameters that can be mofified to be showed
void ExperimentPropertySheet::SetProprietiesModif(int stage, int substage)
{
	modified = true;
	experimentStage = stage;
	experimentSubStage = substage;

	// Set the name of the title
	CString title;
	title.Format(TITLE_MODIFPARAM);
	SetTitle(title);

	// Grey out the tabs that should not be modified
	m_general.checkGeneral = true;

	if (experimentType == EXPERIMENT_TYPE_AUTO)
	{
		switch (experimentStage)
		{
		case STAGE_AUTO_ADSORPTION:
			for (int i = 0; i < experimentSubStage; i++)
			{
				adsorptionTabs[i]->checkDoses = true;
			}
			break;
		case STAGE_AUTO_DESORPTION:
			for (int i = 0; i < numberOfAdsorptions; i++)
			{
				adsorptionTabs[i]->checkDoses = true;
			}
			for (int i = 0; i < experimentSubStage; i++)
			{
				desorptionTabs[i]->checkDesorption = true;
			}
			break;
		default:
			break;
		}
	}
}

// Reinitialise the data in all the tabs - not usually needed
void ExperimentPropertySheet::Reinitialisation()
{
	m_general.Reinitialisation();
	m_divers.Reinitialisation();
	if (experimentType == EXPERIMENT_TYPE_CONTINUOUS)
	{
		m_continuousAdsorption.Reinitialisation();
	}
}

// Asks all the tabs to be added
void ExperimentPropertySheet::AddStepTabs() {
	for (size_t i = 0; i < adsorptionTabs.size(); i++)
	{
		AddTab(adsorptionTabs[i].get(), nb_permanent_tabs + i);
	}
	for (size_t i = 0; i < desorptionTabs.size(); i++)
	{
		AddTab(desorptionTabs[i].get(), nb_permanent_tabs + numberOfAdsorptions + i);
	}
}

// Asks all the tabs to be removed
void ExperimentPropertySheet::RemoveStepTabs()
{
	for (size_t i = 0; i < adsorptionTabs.size(); i++)
	{
		RemoveTab(adsorptionTabs[i].get(), nb_permanent_tabs + i);
	}
	for (size_t i = 0; i < desorptionTabs.size(); i++)
	{
		RemoveTab(desorptionTabs[i].get(), nb_permanent_tabs + numberOfAdsorptions + i);
	}
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
	ON_BN_CLICKED(IDC_SAVE_SETT, OnButtonSaveSettings)
	ON_BN_CLICKED(IDC_LOAD_SETT, OnButtonLoadSettings)
	ON_MESSAGE(UWM_PP_ADSORPTION_DELETE, OnButtonRemoveAdsorption)		// Clicking the delete button, inside one of the property sheets
	ON_MESSAGE(UWM_PP_DESORPTION_DELETE, OnButtonRemoveDesorption)		// Clicking the delete button, inside one of the property sheets
END_MESSAGE_MAP()


// ExperimentPropertySheet message handlers

void ExperimentPropertySheet::OnButtonAddAdsorption()
{
	if (experimentType != EXPERIMENT_TYPE_AUTO) {			// Check if the experiment type allows the addition of tabs
		AfxMessageBox(ERROR_NOT_APPLICABLE);
	}
	else {
		if (numberOfAdsorptions > nb_max_tabs) {			// Check if the maximum allowed doses have been reached, yes i know it's bad to hardcode it
			AfxMessageBox(ERROR_TOO_MANY_TABS);
		}
		else {
			// Save the data
			PressButton(PSBTN_APPLYNOW);

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
			availableTabs.insert((availableTabs.begin() + index), false);

			// Insert page
			adsorptionTabs.push_back(std::make_unique<TabDoses>(index - nb_permanent_tabs + 1));

			// Rename all remaining pages
			for (int i = 0; i < numberOfAdsorptions; i++)
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
	if (experimentType != EXPERIMENT_TYPE_AUTO) {
		AfxMessageBox(ERROR_NOT_APPLICABLE);
	}
	else {
		if (numberOfDesorptions > nb_max_tabs) {
			AfxMessageBox(ERROR_TOO_MANY_TABS);
		}
		else {
			// Save the data
			PressButton(PSBTN_APPLYNOW);

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
			availableTabs.insert((availableTabs.begin() + index), false);

			// Insert page
			desorptionTabs.emplace_back(std::make_unique<TabDesorption>(index - numberOfAdsorptions - nb_permanent_tabs + 1));


			// Rename all remaining pages
			for (int i = 0; i < numberOfDesorptions; i++)
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

void ExperimentPropertySheet::OnButtonSaveSettings()
{
	PressButton(PSBTN_APPLYNOW);

	// Create class
	CFileDialog DialogBoxBrowse(TRUE);

	// Display the dalog and check the return code
	if (DialogBoxBrowse.DoModal() == IDOK) {
		CString f = DialogBoxBrowse.GetPathName();
		std::fstream fs;
		fs.open(f, std::fstream::out | std::ofstream::trunc);
		ExperimentSettings tempSettings;
		ReplaceExperimentSettings(tempSettings, true);
		tempSettings.experimentType = experimentType;
		json j = tempSettings;
		fs << j.dump(4);		// Pretty print
		fs.close();
	}
}

void ExperimentPropertySheet::OnButtonLoadSettings()
{
	// Create class
	CFileDialog DialogBoxBrowse(TRUE);

	// Display the dalog and check the return code
	if (DialogBoxBrowse.DoModal() == IDOK) {
	
		CString f = DialogBoxBrowse.GetPathName();
		std::fstream fs;
		std::string filestr;

		fs.open(f, std::fstream::in);

		fs.seekg(0, std::ios::end);
		filestr.reserve(fs.tellg());
		fs.seekg(0, std::ios::beg);
		filestr.assign((std::istreambuf_iterator<char>(fs)), std::istreambuf_iterator<char>());

		fs.close();

		ExperimentSettings tempSettings;

		try
		{
			tempSettings = json::parse(filestr.c_str());
		}
		catch (const std::exception& e)
		{
			AfxMessageBox(TEXT_FOLDERNOTSELECTED, MB_ICONSTOP | MB_TOPMOST);
		}

		if (tempSettings.experimentType == experimentType)
		{
			TabDivers temp;
			AddPage(&temp);
			RemoveTab(&m_general, tab_general);
			RemoveTab(&m_divers, tab_divers);

			switch (experimentType)
			{
			case EXPERIMENT_TYPE_AUTO:
				RemoveStepTabs();
				adsorptionTabs.clear();
				desorptionTabs.clear();
				break;
			case EXPERIMENT_TYPE_CONTINUOUS:
				RemoveTab(&m_continuousAdsorption, tab_adsorption_continue);

			default:
				break;
			}
			Initiate(tempSettings);
			RemovePage(&temp);
			modified = true;
		}
		else
		{
			AfxMessageBox(TEXT_TYPENOTSAME, MB_ICONSTOP | MB_TOPMOST);
		}
	}
}

LRESULT ExperimentPropertySheet::OnButtonRemoveAdsorption(WPARAM, LPARAM lParam)
{
	int pageToRemove = static_cast<int>(lParam);

	// Check to see if the experiment is running and forbid user to delete the current running step
	if (modified && experimentStage == STAGE_AUTO_ADSORPTION && experimentSubStage == (pageToRemove - 1))
	{
		AfxMessageBox(ERROR_CANNOT_DELETE_RUNNING);
	}
	else
	{
		// Save the data
		PressButton(PSBTN_APPLYNOW);

		SetActivePage(0);															// First go to page number 1
		RemovePage(nb_permanent_tabs + pageToRemove - 1);							// Then remove from propertysheet list

																					// Erase bool from available tabs
		auto pos = availableTabs.begin() + (nb_permanent_tabs + pageToRemove - 1);
		availableTabs.erase(pos);

		// Erase the page itself from the list of pages
		auto pos2 = adsorptionTabs.begin() + pageToRemove - 1;
		adsorptionTabs.erase(pos2);

		numberOfAdsorptions--;														// Decrement the number of adsorption 

																					// Rename all remaining pages
		for (int i = 0; i < numberOfAdsorptions; i++)
		{
			adsorptionTabs[i]->Rename(i + 1);
		}

		// Reset the tabs
		RemoveStepTabs();
		AddStepTabs();
	}
	return 0;
}

LRESULT ExperimentPropertySheet::OnButtonRemoveDesorption(WPARAM, LPARAM lParam)
{
	int pageToRemove = static_cast<int>(lParam);

	// Check to see if the experiment is running and forbid user to delete the current running step
	if (modified && experimentStage == STAGE_AUTO_DESORPTION && experimentSubStage == (pageToRemove - 1))
	{
		AfxMessageBox(ERROR_CANNOT_DELETE_RUNNING);
	}
	else
	{
		// Save the data
		PressButton(PSBTN_APPLYNOW);

		SetActivePage(0);																	// First go to page number 1
		RemovePage(nb_permanent_tabs + numberOfAdsorptions + pageToRemove - 1);				// Then remove from propertysheet list

		// Erase bool from available tabs
		std::vector<bool>::iterator pos = availableTabs.begin() + (nb_permanent_tabs + numberOfAdsorptions + pageToRemove - 1);
		availableTabs.erase(pos);

		// Erase the page itself from the list of pages
		auto pos2 = desorptionTabs.begin() + pageToRemove - 1;
		desorptionTabs.erase(pos2);

		numberOfDesorptions--;														// Decrement the number of adsorption 

		// Rename all remaining pages
		for (int i = 0; i < numberOfDesorptions; i++)
		{
			desorptionTabs[i]->Rename(i + 1);
		}

		// Reset the tabs
		RemoveStepTabs();
		AddStepTabs();
	}
	return 0;
}

// Copy all data from a property sheet dialog to the local object
bool ExperimentPropertySheet::GetExperimentData(ExperimentSettings & expS, bool initialRequest) {

	if (initialRequest)
	{
		// Copy data across
		ReplaceExperimentSettings(expS);
	}

	else
	{
		// Must check if everything is the same

		bool modified = false;

		if (adsorptionTabs.size() != expS.dataAdsorption.size()
			|| desorptionTabs.size() != expS.dataDesorption.size())
		{
			modified = true;
		}
		else
		{
			if (m_general.allSettings != expS.dataGeneral)
			{
				modified = true;
			}

			if (m_divers.allSettings != expS.dataDivers)
			{
				modified = true;
			}

			if (m_continuousAdsorption.allSettings != expS.dataContinuous)
			{
				modified = true;
			}

			for (size_t i = 0; i < adsorptionTabs.size(); i++)
			{
				if (adsorptionTabs[i]->allSettings != expS.dataAdsorption[i])
				{
					modified = true;
				}
			}
			for (size_t i = 0; i < desorptionTabs.size(); i++)
			{
				if (desorptionTabs[i]->allSettings != expS.dataDesorption[i])
				{
					modified = true;
				}
			}
		}

		if (modified)
		{
			// Copy data across
			ReplaceExperimentSettings(expS);
		}

		return modified;
	}

	return true;
}

void ExperimentPropertySheet::ReplaceExperimentSettings(ExperimentSettings & expS, bool complete)
{
	expS.dataGeneral = m_general.allSettings;

	if (expS.experimentType != EXPERIMENT_TYPE_MANUAL || complete)
	{
		expS.dataDivers = m_divers.allSettings;
		expS.dataContinuous = m_continuousAdsorption.allSettings;

		expS.dataAdsorption.clear();
		for (size_t i = 0; i < adsorptionTabs.size(); i++)
		{
			expS.dataAdsorption.push_back(std::move(adsorptionTabs[i]->allSettings));
		}
		expS.dataDesorption.clear();
		for (size_t i = 0; i < desorptionTabs.size(); i++)
		{
			expS.dataDesorption.push_back(std::move(desorptionTabs[i]->allSettings));
		}
	}
}
