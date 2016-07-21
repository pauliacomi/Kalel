
// This file contains Manip functions that are used to interface with the valves object, 
// as well as functions which check whether a valve is opened or not


#include "StdAfx.h"
#include "Manip.h"
#include "Define_Manip_Vannes.h"

// Open a valve
bool CManip::Ouverture_Vanne(int num_vanne)
{
	bool ouverture_reussie = g_pVanne->Ouvrir(num_vanne);
	return ouverture_reussie;
}

// Close a valve
bool CManip::Fermeture_Vanne(int num_vanne)
{
	bool fermeture_reussie = g_pVanne->Fermer(num_vanne);
	return fermeture_reussie;
}

// Open and close a valve
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

// Activate EV number x
bool CManip::ActiverEV(int num_ev)
{
	if(!g_pVanne->ActiverEV(num_ev))
		return FALSE;
	return TRUE;
}

// Deactivate EV number x
bool CManip::DesactiverEV(int num_ev)
{
	if(!g_pVanne->DesactiverEV(num_ev))
		return FALSE;
	return TRUE;
}

// Activate pump
bool CManip::ActiverPompe()
{
	if(!g_pVanne->ActiverPompe())
		return FALSE;
	return TRUE;
}

// Deactivate pump
bool CManip::DesactiverPompe()
{
	if(!g_pVanne->DesactiverPompe())
		return FALSE;
	return TRUE;
}

// Close all valves
bool CManip::FermetureDeToutesLesVannes()
{
	if (!g_pVanne->FermerToutesLesVannes())
		return FALSE;
	return TRUE;
}

// Close the pump
bool CManip::FermerLesValvesEtLaPompe()
{
	if(!g_pVanne->FermerLesValvesEtLaPompe())
		return FALSE;
	return TRUE;
}

// Close everything
bool CManip::ToutFermer()
{
	if(!FermetureDeToutesLesVannes())
		return FALSE;
	if(!FermerLesValvesEtLaPompe())
		return FALSE;
	return TRUE;
}

//------------ Below are functions to check for functionality being on or off

bool CManip::EstOuvert_Vanne(int num_vanne)
{
	return g_pVanne->VanneEstOuvert(num_vanne);
}

bool CManip::EstFerme_Vanne(int num_vanne)
{
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