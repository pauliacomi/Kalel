#include "StdAfx.h"
#include "Manip.h"
#include "Define_Manip_Vannes.h"

using namespace std;


bool CManip::OuvrirEtFermer_Vanne(int num_vanne)
{
	bool ouverture_reussie = Ouverture_Vanne(num_vanne);
	Sleep(temps_ouverture);
	bool fermeture_reussie = Fermeture_Vanne(num_vanne);
	Sleep(temps_fermeture);
	if(ouverture_reussie && fermeture_reussie)
		return TRUE;
	return FALSE;
}

bool CManip::Ouverture_Vanne(int num_vanne)
{
	bool ouverture_reussie = g_pVanne->Ouvrir(num_vanne);
	if(ouverture_reussie)
		SignalerOuvertureVanne(num_vanne);
	return ouverture_reussie;
}

bool CManip::Fermeture_Vanne(int num_vanne)
{
	bool fermeture_reussie = g_pVanne->Fermer(num_vanne);
	if(fermeture_reussie)
		SignalerFermetureVanne(num_vanne);
	return fermeture_reussie;
}

bool CManip::FermetureDeToutesLesVannes()
{
	if(!g_pVanne->FermerToutesLesVannes())
		return FALSE;
	SignalerFermetureDeToutesLesVannes();
	return TRUE;

}

bool CManip::ActiverEV1()
{
	if(!g_pVanne->ActiverEV1())
		return FALSE;
	SignalerActivationEV1();
	return TRUE;
}

bool CManip::DesactiverEV1()
{
	if(!g_pVanne->DesactiverEV1())
		return FALSE;
	SignalerDesactivationEV1();
	return TRUE;
}

bool CManip::ActiverEV2()
{
	if(!g_pVanne->ActiverEV2())
		return FALSE;
	SignalerActivationEV2();
	return TRUE;
}

bool CManip::DesactiverEV2()
{
	if(!g_pVanne->DesactiverEV2())
		return FALSE;
	SignalerDesactivationEV2();
	return TRUE;
}

bool CManip::ActiverPompe()
{
	if(!g_pVanne->ActiverPompe())
		return FALSE;
	SignalerActivationPompe();
	return TRUE;
}

bool CManip::DesactiverPompe()
{
	if(!g_pVanne->DesactiverPompe())
		return FALSE;
	SignalerDesactivationPompe();
	return TRUE;
}

bool CManip::FermerLesValvesEtLaPompe()
{
	if(!g_pVanne->FermerLesValvesEtLaPompe())
		return FALSE;
	SignalerDesactivationValvesEtPompe();
	return TRUE;
}

bool CManip::ToutFermer()
{
	if(!FermetureDeToutesLesVannes())
		return FALSE;
	if(!FermerLesValvesEtLaPompe())
		return FALSE;
	return TRUE;

}


bool CManip::EstOuvert_Vanne(int num_vanne)
{
	//return m_KaollaView->vanne.EstOuvert(num_vanne);
	return g_pVanne->VanneEstOuvert(num_vanne);
}

bool CManip::EstFerme_Vanne(int num_vanne)
{
	//return m_KaollaView->vanne.EstFerme(num_vanne);
	return g_pVanne->VanneEstFerme(num_vanne);
}

bool CManip::EV1EstActive()
{
	return g_pVanne->EV1EstActive();
}

bool CManip::EV1EstDesactive()
{
	return g_pVanne->EV1EstDesactive();
}

bool CManip::EV2EstActive()
{
	return g_pVanne->EV2EstActive();
}

bool CManip::EV2EstDesactive()
{
	return g_pVanne->EV2EstDesactive();
}

bool CManip::PompeEstActive()
{
	return g_pVanne->PompeEstActive();
}

bool CManip::PompeEstDesactive()
{
	return g_pVanne->PompeEstDesactive();
}

void CManip::SignalerOuvertureVanne(int num_vanne)
{
	m_KaollaView->MarquerTemoin(num_vanne, _T("Ouverte"));
}

void CManip::SignalerFermetureVanne(int num_vanne)
{
	m_KaollaView->MarquerTemoin(num_vanne, _T("Fermée"));
}

void CManip::SignalerFermetureDeToutesLesVannes()
{
	m_KaollaView->MarquerTousLesTemoinsFermes();
}


void CManip::SignalerActivationEV1()
{
	m_KaollaView->MarquerEV1(_T("Activée"));
}
void CManip::SignalerDesactivationEV1()
{
	m_KaollaView->MarquerEV1(_T("Désactivée"));
}
void CManip::SignalerActivationEV2()
{
	m_KaollaView->MarquerEV2(_T("Activée"));
}
void CManip::SignalerDesactivationEV2()
{
	m_KaollaView->MarquerEV2(_T("Désactivée"));
}
void CManip::SignalerActivationPompe()
{
	m_KaollaView->MarquerPompe(_T("Activée"));
}
void CManip::SignalerDesactivationPompe()
{
	m_KaollaView->MarquerPompe(_T("Désactivée"));
}

void CManip::SignalerDesactivationValvesEtPompe()
{
	m_KaollaView->MarquerValvesEtPompeDesactivees();
}
