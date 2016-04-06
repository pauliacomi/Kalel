#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;



CManip_AutoGaz::CManip_AutoGaz(void) : CManip_Auto()
{
	InitialisationDialogueTypeExperience();
}

CManip_AutoGaz::~CManip_AutoGaz(void)
{
	//delete DialogueTypeExperience;
}

void CManip_AutoGaz::LancementExperience(LPVOID pParam)
{
	if(DialogueTypeExperience->DoModal() == IDOK)
	{

		switch(DialogueTypeExperience->TypeExperience)
		{
			case EXPERIMENT_TYPE_MANUAL:
				SetManipType(EXPERIMENT_TYPE_MANUAL);
				m_proprietes_experience->SetProprietesManuelle();
				break;
			case EXPERIMENT_TYPE_AUTO :
				SetManipType(EXPERIMENT_TYPE_AUTO);
				m_proprietes_experience->SetProprietesAuto();
				break;
			default :
				// Problème
				break;
		}
			
		if(m_proprietes_experience->DoModal() == IDOK)
		{
			Execution(pParam);
		}
	}

	m_KaollaView->Annuler();

}


void CManip_AutoGaz::ArretExperience()
{
	if(DialogueTypeExperience->TypeExperience == EXPERIMENT_TYPE_MANUAL)
	{
		messageHandler.DisplayMessage(_T("Demande d'arrêt\r\n"));
		manip_en_cours = FALSE;
	}
	else
		ArretImmediat();
}


void CManip_AutoGaz::ReinitialisationAuto()
{
	m_proprietes_experience->ReinitialisationAuto();
}
