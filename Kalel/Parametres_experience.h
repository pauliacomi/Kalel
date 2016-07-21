#pragma once
#ifndef PARAMETRES_EXPERIENCE_H
#define PARAMETRES_EXPERIENCE_H

/// Property pages include
#include "CGeneral.h"
#include "CDivers.h"
#include "CPetitesDoses.h"
#include "CMoyennesDoses.h"
#include "CGrandesDoses.h"
#include "CDesorption.h"
#include "CAdsorptionContinue.h"

#define onglet_general				0
#define onglet_divers				1
#define onglet_adsorption_continue	2
#define onglet_petites_doses		3
#define onglet_moyennes_doses		4
#define onglet_grandes_doses		5
#define onglet_desorption			6

#define nb_onglet 7

/////////////////////////////////////////////////////////////////////////
//
// CProprietes_experience

class CProprietes_experience : public CPropertySheet
{
	DECLARE_DYNAMIC(CProprietes_experience)
public:
	// PropertyPages declared 
	CGeneral m_general;
	CDivers m_divers;
	CPetitesDoses m_petitesdoses;
	CMoyennesDoses m_moyennesdoses;
	CGrandesDoses m_grandesdoses;
	CDesorption m_desorption;
	CAdsorptionContinue m_adsorptioncontinue;

	int TypeFenetre;
	bool OngletDispo[nb_onglet];

public:
	CProprietes_experience(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CProprietes_experience(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CProprietes_experience();

protected:
	DECLARE_MESSAGE_MAP()
public:
	//virtual BOOL OnInitDialog();

	void SetProprietesManuelle(void);
	void SetProprietesAuto(void);
	void SetProprietesModif(int etape_en_cours);

	void ReinitialisationAuto();
	void ReinitialisationManuelle();

	void RajouterGeneral();
	void EnleverGeneral();
	void RajouterDivers();
	void EnleverDivers();
	void RajouterAdsorptionContinue();
	void EnleverAdsorptionContinue();
	void RajouterPetitesDoses();
	void EnleverPetitesDoses();
	void RajouterGrandesDoses();
	void EnleverGrandesDoses();
	void RajouterMoyennesDoses();
	void EnleverMoyennesDoses();
	void RajouterDesorption();
	void EnleverDesorption();

	void EnleverTousLesOnglets();
};


#endif // !PARAMETRES_EXPERIENCE_H