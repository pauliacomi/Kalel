#include "StdAfx.h"
#include "Manip_AutoGaz.h"


using namespace std;


bool CManip_AutoGaz::Ouverture_Vanne(int num_vanne)
{
	if(num_vanne == 6 && !GetMesureBassePression())
		return TRUE;

	return CManip_Auto::Ouverture_Vanne(num_vanne);
}

bool CManip_AutoGaz::Fermeture_Vanne(int num_vanne)
{
	if(num_vanne == 6 && !GetMesureBassePression())
		return TRUE;

	return CManip_Auto::Fermeture_Vanne(num_vanne);
}

bool CManip_AutoGaz::OuvrirEtFermer_Vanne(int num_vanne)
{
	if(num_vanne == 6 && !GetMesureBassePression())
		return TRUE;

	return CManip_Auto::OuvrirEtFermer_Vanne(num_vanne);
}