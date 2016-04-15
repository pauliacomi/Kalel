
// This file contains Manip functions that are used to interface with the valves object, 
// as well as functions which check whether a valve is opened or not


#include "StdAfx.h"
#include "Automation.h"
#include "Define_Manip_Vannes.h"


// Open a valve
bool Automation::ValveOpen(int num_vanne)
{
	bool ouverture_reussie = g_pVanne->Ouvrir(num_vanne);
	return ouverture_reussie;
}

// Close a valve
bool Automation::ValveClose(int num_vanne)
{
	bool fermeture_reussie = g_pVanne->Fermer(num_vanne);
	return fermeture_reussie;
}

// Open and close a valve
bool Automation::ValveOpenClose(int num_vanne)
{
	bool ouverture_reussie = ValveOpen(num_vanne);
	Sleep(temps_ouverture);
	bool fermeture_reussie = ValveClose(num_vanne);
	Sleep(temps_fermeture);
	if (ouverture_reussie && fermeture_reussie)
		return TRUE;
	return FALSE;
}

// Activate EV number x
bool Automation::EVActivate(int num_ev)
{
	if (!g_pVanne->ActiverEV(num_ev))
		return FALSE;
	return TRUE;
}

// Deactivate EV number x
bool Automation::EVDeactivate(int num_ev)
{
	if (!g_pVanne->DesactiverEV(num_ev))
		return FALSE;
	return TRUE;
}

// Activate pump
bool Automation::ActivatePump()
{
	if (!g_pVanne->ActiverPompe())
		return FALSE;
	return TRUE;
}

// Deactivate pump
bool Automation::DeactivatePump()
{
	if (!g_pVanne->DesactiverPompe())
		return FALSE;
	return TRUE;
}

// Close all valves
bool Automation::ValvesCloseAll()
{
	if (!g_pVanne->FermerToutesLesVannes())
		return FALSE;
	return TRUE;
}

// Close the pump
bool Automation::ValvesAndPumpClose()
{
	if (!g_pVanne->FermerLesValvesEtLaPompe())
		return FALSE;
	return TRUE;
}

// Close everything
bool Automation::ControlMechanismsCloseAll()
{
	if (!ValvesAndPumpClose())
		return FALSE;
	if (!ValvesCloseAll())
		return FALSE;
	return TRUE;
}

//------------ Below are functions to check for functionality being on or off

bool Automation::EstOuvert_Vanne(int num_vanne)
{
	return g_pVanne->VanneEstOuvert(num_vanne);
}

bool Automation::EstFerme_Vanne(int num_vanne)
{
	return g_pVanne->VanneEstFerme(num_vanne);
}

bool Automation::EV1EstActive()
{
	return g_pVanne->EV1EstActive();
}

bool Automation::EV1EstDesactive()
{
	return g_pVanne->EV1EstDesactive();
}

bool Automation::EV2EstActive()
{
	return g_pVanne->EV2EstActive();
}

bool Automation::EV2EstDesactive()
{
	return g_pVanne->EV2EstDesactive();
}

bool Automation::PompeEstActive()
{
	return g_pVanne->PompeEstActive();
}

bool Automation::PompeEstDesactive()
{
	return g_pVanne->PompeEstDesactive();
}