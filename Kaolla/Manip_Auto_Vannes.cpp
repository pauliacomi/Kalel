#include "StdAfx.h"
#include "Manip_Auto.h"



using namespace std;

bool CManip_Auto::FermetureDeToutesLesVannes()
{
	if(!CManip::FermetureDeToutesLesVannes())
		return FALSE;
	return TRUE;
}

bool CManip_Auto::Ouverture_Vanne(int num_vanne)
{
	if(!CManip::Ouverture_Vanne(num_vanne))
	{
		// problem
		return FALSE;
	}

	CString message;
	message.Format(_T("Vanne %d ouverte\r\n"),num_vanne);
	AffichageMessages(message);
	return TRUE;
}

bool CManip_Auto::Fermeture_Vanne(int num_vanne)
{
	if(!CManip::Fermeture_Vanne(num_vanne))
	{
		// problem
		return FALSE;
	}
	CString message;
	message.Format(_T("Vanne %d fermée\r\n"),num_vanne);
	AffichageMessages(message);
	return TRUE;
}

bool CManip_Auto::OuvrirEtFermer_Vanne(int num_vanne)
{
	if(!CManip::OuvrirEtFermer_Vanne(num_vanne))
	{
		// problem
		return FALSE;
	}
	CString message;
	message.Format(_T("Ouverture et fermeture de la Vanne %d\r\n"),num_vanne);
	AffichageMessages(message);
	return TRUE;
}

bool CManip_Auto::ActiverEV(int num_ev)
{
	if(!CManip::ActiverEV(num_ev))
	{
		// problem
		return FALSE;
	}
	AffichageMessages(_T("Activation de la valve EVx\r\n")); 
	return TRUE;
}

bool CManip_Auto::DesactiverEV(int num_ev)
{
	if(!CManip::DesactiverEV(num_ev))
	{
		// problem
		return FALSE;
	}
	AffichageMessages(_T("Désactivation de la valve EVx\r\n"));
	return TRUE;
}

bool CManip_Auto::ActiverPompe()
{
	if(!CManip::ActiverPompe())
	{
		// problem
		return FALSE;
	}
	AffichageMessages(_T("Activation de la valve Pompe\r\n"));
	return TRUE;
}

bool CManip_Auto::DesactiverPompe()
{
	if(!CManip::DesactiverPompe())
	{
		// problem
		return FALSE;
	}
	AffichageMessages(_T("Désactivation de la valve Pompe\r\n"));
	return TRUE;
}

bool CManip_Auto::MettreEnRouteLaPompe()
{
	if(!ActiverEV(1))
		return FALSE;
	if(!ActiverEV(2))
		return FALSE;
	if(!ActiverPompe())
		return FALSE;
	//Sleep(15000);
	AttenteSecondes(15);
	return TRUE;

}

bool CManip_Auto::ArreterLaPompe()
{
	if(!DesactiverPompe())
		return FALSE;
	Sleep(2000);
	if(!DesactiverEV(1))
		return FALSE;
	if(!DesactiverEV(2))
		return FALSE;
	return TRUE;

}

bool CManip_Auto::PompeEnRoute()
{
	if(EV1EstActive() && EV2EstActive() && PompeEstActive())
		return true;
	return false;
}

bool CManip_Auto::PompeEnArret()
{
	if(EV1EstDesactive() && EV2EstDesactive() && PompeEstDesactive())
		return true;
	return false;
}


bool CManip_Auto::ToutFermer()
{
	if(!FermetureDeToutesLesVannes())
		return FALSE;
	if(EV1EstActive() || EV2EstActive() || PompeEstActive())
	{
		if(!ArreterLaPompe())
			return FALSE;
	}
	return TRUE;
}

