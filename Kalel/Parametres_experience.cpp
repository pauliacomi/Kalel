// Parametres_experience.cpp
//

#include "stdafx.h"
#include "Kalel.h"
#include "Parametres_experience.h"
#include "MessageBoxTexts.h"			// For the required strings

/////////////////////////////////////////////////////////////////////////////////
//
// CProprietes_experience

IMPLEMENT_DYNAMIC(CProprietes_experience, CPropertySheet)

// Constructors and destrutor

CProprietes_experience::CProprietes_experience(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CProprietes_experience::CProprietes_experience(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	for(int i=0;i<nb_onglet;i++)
		OngletDispo[i]=FALSE;

	RajouterGeneral();

	TypeFenetre = EXPERIMENT_TYPE_MANUAL;

	// Set the name of the title
	CString title;
	title.Format(TITLE_MANUALPARAM);
	SetTitle(title);
}

CProprietes_experience::~CProprietes_experience()
{
}


BEGIN_MESSAGE_MAP(CProprietes_experience, CPropertySheet)
END_MESSAGE_MAP()


// Gestionnaires de messages de CProprietes_experience


void CProprietes_experience::SetProprietesManuelle(void)
{
	if(TypeFenetre != EXPERIMENT_TYPE_MANUAL)
	{
		TypeFenetre = EXPERIMENT_TYPE_MANUAL;
		
		// Set the name of the title
		CString title;
		title.Format(TITLE_MANUALPARAM);
		SetTitle(title);

		ReinitialisationManuelle();

		RajouterGeneral();
		EnleverDivers();
		EnleverAdsorptionContinue();
		EnleverPetitesDoses();
		EnleverGrandesDoses();
		EnleverDesorption();
	}
	
}

void CProprietes_experience::SetProprietesAuto(void)
{
	if(TypeFenetre != EXPERIMENT_TYPE_AUTO)
	{
		// Set the name of the title
		CString title;
		title.Format(TITLE_AUTOPARAM);
		SetTitle(title);

		TypeFenetre = EXPERIMENT_TYPE_AUTO;

		EnleverTousLesOnglets();

		ReinitialisationAuto();

		RajouterGeneral();
		RajouterDivers();
		RajouterAdsorptionContinue();
		RajouterPetitesDoses();
		RajouterMoyennesDoses();
		RajouterGrandesDoses();
		RajouterDesorption();
	}
}



void CProprietes_experience::SetProprietesModif(int etape_en_cours)
{
	TypeFenetre = EXPERIMENT_TYPE_MODIFY;

	SetTitle("Modifications en cours d'expérience");

	EnleverTousLesOnglets();

	switch(etape_en_cours)
	{
		case STAGE_TEMP:
		case STAGE_EQ_LINE :
		case STAGE_CONT_ADSORPTON :
			RajouterPetitesDoses();
			RajouterMoyennesDoses();
			RajouterGrandesDoses();
			RajouterDesorption();
			break;
		case STAGE_SMALL_DOSES :
			RajouterPetitesDoses();
			RajouterMoyennesDoses();
			RajouterGrandesDoses();
			RajouterDesorption();
			m_petitesdoses.GriserCheck_PetitesDoses();
			break;
		case STAGE_MEDIUM_DOSES : 
			RajouterMoyennesDoses();
			RajouterGrandesDoses();
			RajouterDesorption();
			m_moyennesdoses.GriserCheck_MoyennesDoses();
			break;
		case STAGE_BIG_DOSES :
			RajouterGrandesDoses();
			RajouterDesorption();
			m_grandesdoses.GriserCheck_GrandesDoses();
			break;
		case STAGE_DESORPTION :
			RajouterDesorption();
			m_desorption.GriserCheck_Desorption();
			break;
		default:
			break;
	}
}


void CProprietes_experience::ReinitialisationAuto()
{
	m_general.Reinitialisation();
	m_divers.Reinitialisation();
	m_petitesdoses.Reinitialisation();
	m_moyennesdoses.Reinitialisation();
	m_grandesdoses.Reinitialisation();
	m_desorption.Reinitialisation();
	m_adsorptioncontinue.Reinitialisation();
}

void CProprietes_experience::ReinitialisationManuelle()
{
	m_general.Reinitialisation();
}



/*- Functions to add or to remove a particular tab depending on whether it is avaiable or not. Very ugly.-*/


void CProprietes_experience::RajouterGeneral()
{
	if(OngletDispo[onglet_general]==FALSE)
	{
		AddPage(&m_general); 
		OngletDispo[onglet_general]=TRUE;
	}
}

void CProprietes_experience::EnleverGeneral()
{
	if(OngletDispo[onglet_general]==TRUE)
	{
		RemovePage(&m_general); 
		OngletDispo[onglet_general]=FALSE;
	}
}

/*--*/

void CProprietes_experience::RajouterDivers()
{
	if(OngletDispo[onglet_divers]==FALSE)
	{
		AddPage(&m_divers); 
		OngletDispo[onglet_divers]=TRUE;
	}
}

void CProprietes_experience::EnleverDivers()
{
	if(OngletDispo[onglet_divers]==TRUE)
	{
		RemovePage(&m_divers); 
		OngletDispo[onglet_divers]=FALSE;
	}
}

/*--*/


void CProprietes_experience::RajouterAdsorptionContinue()
{
	if(GetPresenceTuyereSonique())
	{
		if(OngletDispo[onglet_adsorption_continue]==FALSE)
		{
			AddPage(&m_adsorptioncontinue); 
			OngletDispo[onglet_adsorption_continue]=TRUE;
		}
	}
	else
	{
		EnleverAdsorptionContinue();
	}
}

void CProprietes_experience::EnleverAdsorptionContinue()
{
	if(OngletDispo[onglet_adsorption_continue]==TRUE)
	{
		RemovePage(&m_adsorptioncontinue); 
		OngletDispo[onglet_adsorption_continue]=FALSE;
	}
}


/*--*/


void CProprietes_experience::RajouterPetitesDoses()
{
	if(OngletDispo[onglet_petites_doses]==FALSE)
	{
		AddPage(&m_petitesdoses); 
		OngletDispo[onglet_petites_doses]=TRUE;
	}
}

void CProprietes_experience::EnleverPetitesDoses()
{
	if(OngletDispo[onglet_petites_doses]==TRUE)
	{
		RemovePage(&m_petitesdoses); 
		OngletDispo[onglet_petites_doses]=FALSE;
	}
}

/*--*/

void CProprietes_experience::RajouterMoyennesDoses()
{
	if(OngletDispo[onglet_moyennes_doses]==FALSE)
	{
		AddPage(&m_moyennesdoses); 
		OngletDispo[onglet_moyennes_doses]=TRUE;
	}
}

void CProprietes_experience::EnleverMoyennesDoses()
{
	if(OngletDispo[onglet_moyennes_doses]==TRUE)
	{
		RemovePage(&m_moyennesdoses); 
		OngletDispo[onglet_moyennes_doses]=FALSE;
	}
}


/*--*/

void CProprietes_experience::RajouterGrandesDoses()
{
	if(OngletDispo[onglet_grandes_doses]==FALSE)
	{
		AddPage(&m_grandesdoses); 
		OngletDispo[onglet_grandes_doses]=TRUE;
	}
}

void CProprietes_experience::EnleverGrandesDoses()
{
	if(OngletDispo[onglet_grandes_doses]==TRUE)
	{
		RemovePage(&m_grandesdoses); 
		OngletDispo[onglet_grandes_doses]=FALSE;
	}
}


/*--*/


void CProprietes_experience::RajouterDesorption()
{
	if(OngletDispo[onglet_desorption]==FALSE)
	{
		AddPage(&m_desorption); 
		OngletDispo[onglet_desorption]=TRUE;
	}
}

void CProprietes_experience::EnleverDesorption()
{
	if(OngletDispo[onglet_desorption]==TRUE)
	{
		RemovePage(&m_desorption); 
		OngletDispo[onglet_desorption]=FALSE;
	}
}

/*--*/

// Add all the tabs to the Property Sheet
void CProprietes_experience::EnleverTousLesOnglets()
{
		EnleverGeneral();
		EnleverDivers();
		EnleverAdsorptionContinue();
		EnleverPetitesDoses();
		EnleverMoyennesDoses();
		EnleverGrandesDoses();
		EnleverDesorption();
}