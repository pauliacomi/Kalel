#pragma once
#ifndef EXPERIMENTPROPERTYSHEET_H
#define EXPERIMENTPROPERTYSHEET_H

// ExperimentPropertySheet

/// Property pages include
#include "TabGeneral.h"
#include "TabDivers.h"
#include "TabDoses.h"


/// Defines
#define tab_general				0
#define tab_divers				1
#define tab_adsorption_continue	2
#define tab_petites_doses		3
#define tab_moyennes_doses		4
#define tab_grandes_doses		5
#define tab_desorption			6

#define nb_tabs 7

class ExperimentPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(ExperimentPropertySheet)

public:
	ExperimentPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	ExperimentPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~ExperimentPropertySheet();

	void SetProprietiesManual(void);
	void SetProprietiesAuto(void);
	void SetProprietiesModif(int etape_en_cours);

	void ReinitialisationAuto();
	void ReinitialisationManual();

protected:
	DECLARE_MESSAGE_MAP()


// variables
protected:
	// PropertyPages declared
	TabGeneral m_general;
	TabDivers m_divers;
	TabDoses m_smalldoses;
	TabDoses m_mediumdoses;
	TabDoses m_bigdoses;
	// desorption
	// continued adsorption

	CPropertyPage * p_general;
	CPropertyPage * p_divers;
	CPropertyPage * p_smalldoses;
	CPropertyPage * p_mediumdoses;
	CPropertyPage * p_bigdoses;
	CPropertyPage * p_desorption;
	CPropertyPage * p_adsorptioncontinue;

public:
	int experimentType;
	bool availableTabs[nb_tabs];

// custom functions

public:
	void AddAllTabs();
	void RemoveAllTabs();
	void AddTab(CPropertyPage * tab, int checkTab);
	void RemoveTab(CPropertyPage * tab, int checkTab);

};

#endif // !EXPERIMENTPROPERTYSHEET_H



