#ifndef EXPERIMENTPROPERTYSHEET_H
#define EXPERIMENTPROPERTYSHEET_H
#pragma once

// ExperimentPropertySheet

// Forward declarations
#include "TabGeneral.h"
#include "TabDivers.h"
#include "TabDoses.h"
#include "TabDesorption.h"
#include "TabContinuousAdsorption.h"

// Forward declaration
class ExperimentSettings;

// Other includes
#include <vector>		// Using a vector to keep the collection of tabs in


/// Defines
#define tab_general				0
#define tab_divers				1
#define tab_adsorption_continue	2

#define nb_permanent_tabs		2
#define nb_max_tabs				8

class ExperimentPropertySheet : public CMFCPropertySheet
{
	DECLARE_DYNAMIC(ExperimentPropertySheet)

public:
	ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ExperimentPropertySheet(LPCTSTR pszCaption, MachineSettings * machineSettings, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~ExperimentPropertySheet();

	void Initiate(const ExperimentSettings & experimentSettings);
	BOOL OnInitDialog();

protected:
	DECLARE_MESSAGE_MAP()

// variables
public:
	// PropertyPages declared
	TabGeneral m_general;
	TabDivers m_divers;
	std::vector<std::unique_ptr<TabDoses>> adsorptionTabs;
	std::vector<std::unique_ptr<TabDesorption>> desorptionTabs;
	TabContinuousAdsorption m_continuousAdsorption;

protected:
	CMFCButton m_addAdsorption;
	CMFCButton m_addDesorption;
	CMFCButton m_saveSettings;
	CMFCButton m_loadSettings;

	// Number of tabs
	int numberOfAdsorptions;
	int numberOfDesorptions;

	// Availability
	bool modified;
	int experimentStage;
	int experimentSubStage;

public:
	int experimentType;
	std::vector<bool> availableTabs;   /// it is here to prevent a tab being added twice, might be useleess but need to reprogram stuff

// custom functions

public:
	void AddStepTabs();		// Asks all the tabs to be added
	void RemoveStepTabs();	// Asks all the tabs to be removed
	void OnButtonAddAdsorption();
	void OnButtonAddDesorption();
	void OnButtonSaveSettings();
	void OnButtonLoadSettings();
	LRESULT OnButtonRemoveAdsorption(WPARAM wParam, LPARAM lParam);
	LRESULT OnButtonRemoveDesorption(WPARAM wParam, LPARAM lParam);
	bool GetExperimentData(ExperimentSettings & expS, bool initialRequest);
	void ReplaceExperimentSettings(ExperimentSettings & expS, bool complete = false);
	void AddTab(CPropertyPage * tab, int checkTab);			// Adds a tab, checking if it is available first
	void RemoveTab(CPropertyPage * tab, int checkTab);		// Removes a tab, checking if it is available first

	// Sets the experiment type as manual
	// It allows only the general tab to be viewed
	void SetProprietiesManual(void);

	// Sets the experiment type as automatic
	// It allows all the tabs to be viewed
	void SetProprietiesAuto(void);

	void SetProprietiesAutoCont(void);

	// Sets the experiment type as modified
	// It allows only the tabs which have parameters that can be mofified to be showed
	void SetProprietiesModif(int stage, int substage);

	void Reinitialisation();		// Reinitialise the data in all the tabs

};

#endif // !EXPERIMENTPROPERTYSHEET_H



