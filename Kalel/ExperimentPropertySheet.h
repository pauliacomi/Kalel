#pragma once
#ifndef EXPERIMENTPROPERTYSHEET_H
#define EXPERIMENTPROPERTYSHEET_H

// ExperimentPropertySheet

// Property pages include
#include "TabGeneral.h"
#include "TabDivers.h"
#include "TabDoses.h"
#include "TabDesorption.h"
#include "TabContinuousAdsorption.h"

// Other includes
#include <vector>		// Using a vector to keep the collection of tabs in


/// Defines
#define tab_general				0
#define tab_divers				1
#define tab_adsorption_continue	6

#define nb_permanent_tabs		2

class ExperimentPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(ExperimentPropertySheet)

public:
	ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~ExperimentPropertySheet();

	BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()


// variables
public:
	// PropertyPages declared
	TabGeneral m_general;
	TabDivers m_divers;
	TabDoses * m_dose;
	TabDesorption * m_desorption;
	TabContinuousAdsorption m_continuousAdsorption;

	vector<TabDoses*> adsorptionTabs;
	vector<TabDesorption*> desorptionTabs;

protected:
	CMFCButton m_addAdsorption;
	CMFCButton m_addDesorption;

	// Pointers for class polymorphism
	CPropertyPage * p_generalPP;
	CPropertyPage * p_diversPP;
	CPropertyPage * p_adsorptioncontinuePP;
	vector<CPropertyPage*> adsorptionTabPointers;
	vector<CPropertyPage*> desorptionTabPointers;

	// Number of tabs
	int numberOfAdsorptions;
	int numberOfDesorptions;

public:
	int experimentType;
	vector<bool> availableTabs;

// custom functions

public:
	void AddAllTabs();		// Asks all the tabs to be added
	void RemoveAllTabs();	// Asks all the tabs to be removed
	void AddAdsorption(int i);	// Adds one new adsorption experiment tab
	void AddDesorption(int i);	// Adds one new adsorption experiment tab
	void OnButtonAddAdsorption();
	void OnButtonAddDesorption();
	LRESULT OnButtonRemoveAdsorption(WPARAM wParam, LPARAM lParam);
	LRESULT OnButtonRemoveDesorption(WPARAM wParam, LPARAM lParam);
	void AddTab(CPropertyPage * tab, int checkTab);			// Adds a tab, checking if it is available first
	void RemoveTab(CPropertyPage * tab, int checkTab);		// Removes a tab, checking if it is available first

	// Sets the experiment type as manual
	// It allows only the general tab to be viewed
	void SetProprietiesManual(void);

	// Sets the experiment type as automatic
	// It allows all the tabs to be viewed
	void SetProprietiesAuto(void);

	// Sets the experiment type as modified
	// It allows only the tabs which have parameters that can be mofified to be showed
	void SetProprietiesModif(int etape_en_cours);

	void ReinitialisationAuto();		// Reinitialise the data in all the tabs
	void ReinitialisationManual();		// Reinitialise the data only in the general tab

	//Inserts the page at a given Index
	void InsertPageAt(CPropertyPage* pPageToInsert, int nPos);
};

#endif // !EXPERIMENTPROPERTYSHEET_H



