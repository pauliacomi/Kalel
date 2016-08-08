#include "StdAfx.h"
#include "Manip_AutoGaz.h"

using namespace std;



CManip_AutoGaz::CManip_AutoGaz(void) : CManip_Auto()
{
}

CManip_AutoGaz::~CManip_AutoGaz(void)
{
}

void CManip_AutoGaz::LancementExperience(LPVOID pParam)
{
	if(true)
	{

		switch(TypeExperience)	//from dialog which used to be here
		{
			case EXPERIMENT_TYPE_MANUAL:
				SetManipType(EXPERIMENT_TYPE_MANUAL);
				break;
			case EXPERIMENT_TYPE_AUTO :
				SetManipType(EXPERIMENT_TYPE_AUTO);
				break;
			default :
				ASSERT(0); // Should never reach this
				break;
		}
			
		if(m_proprietes_experience->DoModal() == IDOK)
		{
			Execution(pParam);
		}
	}

	messageHandler.CancelExperiment();

}


void CManip_AutoGaz::ArretExperience()
{
	if(TypeExperience == EXPERIMENT_TYPE_MANUAL)
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
