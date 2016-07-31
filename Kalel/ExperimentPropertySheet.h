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

// Experiment settings class
#include "ExperimentSettings.h"


// Other includes
#include <vector>		// Using a vector to keep the collection of tabs in


/// Defines
#define tab_general				0
#define tab_divers				1
#define tab_adsorption_continue	6

#define nb_permanent_tabs		2
#define nb_max_tabs				5

class ExperimentPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(ExperimentPropertySheet)

public:
	ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~ExperimentPropertySheet();

	void Initiate(ExperimentSettings * experimentSettings);
	BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()


// variables
public:
	// PropertyPages declared
	TabGeneral m_general;
	TabDivers m_divers;
	vector<TabDoses*> adsorptionTabs;
	vector<TabDesorption*> desorptionTabs;
	TabContinuousAdsorption m_continuousAdsorption;

protected:
	CMFCButton m_addAdsorption;
	CMFCButton m_addDesorption;

	// Number of tabs
	int numberOfAdsorptions;
	int numberOfDesorptions;

	// Availability
	bool modified;
	int experimentStage;
	int experimentSubStage;

public:
	int experimentType;
	vector<bool> availableTabs;   /// it is here to prevent a tab being added twice, might be useleess but need to reprogram stuff

// custom functions

public:
	void AddStepTabs();		// Asks all the tabs to be added
	void RemoveStepTabs();	// Asks all the tabs to be removed
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
	void SetProprietiesModif(int stage, int substage);

	void Reinitialisation(bool automatic);		// Reinitialise the data in all the tabs

};

#endif // !EXPERIMENTPROPERTYSHEET_H



