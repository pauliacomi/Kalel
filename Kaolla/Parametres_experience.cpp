// Parametres_experience.cpp : fichier d'implémentation
//

#include "stdafx.h"
#include "Kaolla.h"
#include "Parametres_experience.h"

//#include "ParserXML.h"

#include "SpinBtnCtrl.h"

#include <sstream>

#include "time.h"

using namespace std;



/////////////////////////////////////////////////////////////////////////////////
//
// CProprietes_experience

IMPLEMENT_DYNAMIC(CProprietes_experience, CPropertySheet)

CProprietes_experience::CProprietes_experience(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{

}

CProprietes_experience::CProprietes_experience(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	for(int i=0;i<nb_onglet;i++)
		OngletDispo[i]=FALSE;

	
	//AddPage(&m_general); 
	RajouterGeneral();

	//DemandeExperience = MANUEL;
	//TypeExperience = MANUEL;
	TypeFenetre = MANUEL;
	SetTitle("Paramètres de l'expérience manuelle");
}

CProprietes_experience::~CProprietes_experience()
{
}


BEGIN_MESSAGE_MAP(CProprietes_experience, CPropertySheet)
END_MESSAGE_MAP()


// Gestionnaires de messages de CProprietes_experience


void CProprietes_experience::SetProprietesManuelle(void)
{

	//if(TypeExperience != MANUEL)
	if(TypeFenetre != MANUEL)
	{
		//TypeExperience = MANUEL;
		TypeFenetre = MANUEL;
		SetTitle("Paramètres de l'expérience manuelle");

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
	//if(TypeExperience != AUTO)
	if(TypeFenetre != AUTO)
	{
		//TypeExperience = AUTO;
		TypeFenetre = AUTO;
		SetTitle("Paramètres de l'expérience automatique");	

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
	TypeFenetre = MODIF;

	SetTitle("Modifications en cours d'expérience");

	EnleverTousLesOnglets();

	switch(etape_en_cours)
	{
		case TEMPERATURES:
		case LIGNE_EQUILIBRE :
		case ADSORPTION_CONTINUE :
			RajouterPetitesDoses();
			RajouterMoyennesDoses();
			RajouterGrandesDoses();
			RajouterDesorption();
			break;
		case PETITES_DOSES :
			RajouterPetitesDoses();
			RajouterMoyennesDoses();
			RajouterGrandesDoses();
			RajouterDesorption();
			m_petitesdoses.GriserCheck_PetitesDoses();
			break;
		case MOYENNES_DOSES : 
			RajouterMoyennesDoses();
			RajouterGrandesDoses();
			RajouterDesorption();
			m_moyennesdoses.GriserCheck_MoyennesDoses();
			break;
		case GRANDES_DOSES :
			RajouterGrandesDoses();
			RajouterDesorption();
			m_grandesdoses.GriserCheck_GrandesDoses();
			break;
		case DESORPTION :
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



/*--*/


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