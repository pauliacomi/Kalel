#include "StdAfx.h"
#include "Manip_Auto.h"



using namespace std;

bool CManip_Auto::FermetureDeToutesLesVannes()
{
	if(!CManip::FermetureDeToutesLesVannes())
		return FALSE;
	AffichageMessages(_T("Fermeture de toutes les vannes\r\n"));
	return TRUE;
}

bool CManip_Auto::Ouverture_Vanne(int num_vanne)
{
	bool ouverture_reussie = CManip::Ouverture_Vanne(num_vanne);
	if(!ouverture_reussie)
	{
		// problème
		return FALSE;
	}

	CString message;
	message.Format(_T("Vanne %d ouverte\r\n"),num_vanne);
	AffichageMessages(message);
	return TRUE;
}

bool CManip_Auto::Fermeture_Vanne(int num_vanne)
{
	bool fermeture_reussie = CManip::Fermeture_Vanne(num_vanne);
	if(!fermeture_reussie)
	{
		// problème
		return FALSE;
	}
	CString message;
	message.Format(_T("Vanne %d fermée\r\n"),num_vanne);
	AffichageMessages(message);
	return TRUE;
}

bool CManip_Auto::OuvrirEtFermer_Vanne(int num_vanne)
{
	bool ouvfer_reussie = CManip::OuvrirEtFermer_Vanne(num_vanne);
	if(!ouvfer_reussie)
	{
		// problème
		return FALSE;
	}
	CString message;
	message.Format(_T("Ouverture et fermeture de la Vanne %d\r\n"),num_vanne);
	AffichageMessages(message);
	return TRUE;
}

bool CManip_Auto::ActiverEV1()
{
	bool activation_reussie = CManip::ActiverEV1();
	if(!activation_reussie)
	{
		// probleme
		return FALSE;
	}
	AffichageMessages(_T("Activation de la valve EV1\r\n"));
	return TRUE;
}

bool CManip_Auto::DesactiverEV1()
{
	bool desactivation_reussie = CManip::DesactiverEV1();
	if(!desactivation_reussie)
	{
		// probleme
		return FALSE;
	}
	AffichageMessages(_T("Désactivation de la valve EV1\r\n"));
	return TRUE;
}

bool CManip_Auto::ActiverEV2()
{
	bool activation_reussie = CManip::ActiverEV2();
	if(!activation_reussie)
	{
		// probleme
		return FALSE;
	}
	AffichageMessages(_T("Activation de la valve EV2\r\n"));
	return TRUE;
}

bool CManip_Auto::DesactiverEV2()
{
	bool desactivation_reussie = CManip::DesactiverEV2();
	if(!desactivation_reussie)
	{
		// probleme
		return FALSE;
	}
	AffichageMessages(_T("Désactivation de la valve EV2\r\n"));
	return TRUE;
}

bool CManip_Auto::ActiverPompe()
{
	bool activation_reussie = CManip::ActiverPompe();
	if(!activation_reussie)
	{
		// probleme
		return FALSE;
	}
	AffichageMessages(_T("Activation de la valve Pompe\r\n"));
	return TRUE;
}

bool CManip_Auto::DesactiverPompe()
{
	bool desactivation_reussie = CManip::DesactiverPompe();
	if(!desactivation_reussie)
	{
		// probleme
		return FALSE;
	}
	AffichageMessages(_T("Désactivation de la valve Pompe\r\n"));
	return TRUE;
}

bool CManip_Auto::MettreEnRouteLaPompe()
{
	if(!ActiverEV1())
		return FALSE;
	if(!ActiverEV2())
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
	if(!DesactiverEV1())
		return FALSE;
	if(!DesactiverEV2())
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

