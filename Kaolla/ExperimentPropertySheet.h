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
#define tab_desorption			5

#define nb_ads_tabs 3
#define nb_tabs 7

class ExperimentPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(ExperimentPropertySheet)

public:
	ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~ExperimentPropertySheet();

protected:
	DECLARE_MESSAGE_MAP()


// variables
public:
	// PropertyPages declared
	TabGeneral m_general;
	TabDivers m_divers;
	TabDoses * p_dose;
	TabDesorption m_desorption;
	TabContinuousAdsorption m_continuousAdsorption;
	vector<TabDoses*> adsorptionTabs;

protected:

	// Pointers for class polymorphism
	CPropertyPage * p_generalPP;
	CPropertyPage * p_diversPP;
	CPropertyPage * p_dosePP;
	CPropertyPage * p_desorptionPP;
	CPropertyPage * p_adsorptioncontinuePP;
	vector<CPropertyPage*> adsorptionTabPointers;

public:
	int experimentType;
	bool availableTabs[nb_tabs];

// custom functions

public:
	void AddAllTabs();		// Asks all the tabs to be added
	void RemoveAllTabs();	// Asks all the tabs to be removed
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

};

#endif // !EXPERIMENTPROPERTYSHEET_H



