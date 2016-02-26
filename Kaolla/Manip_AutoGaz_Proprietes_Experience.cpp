#include "StdAfx.h"
#include "Manip_AutoGaz.h"


using namespace std;


void CManip_AutoGaz::ModifierParametresExperience()
{
	m_proprietes_experience->SetProprietesModif(etape_en_cours);
	m_proprietes_experience->DoModal();
}