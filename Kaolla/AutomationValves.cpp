
// This file contains Manip functions that are used to interface with the valves object, 
// as well as functions which check whether a valve is opened or not


#include "StdAfx.h"
#include "Automation.h"


// Open a valve
bool Automation::ValveOpen(int num_vanne)
{
	// Open valve
	if(!g_pVanne->Ouvrir(num_vanne))
		return false;

	// Tell GUI valve is opened
	messageHandler.DisplayMessage(MESSAGE_VALVE_OPENED, num_vanne);

	// Return success
	return true;
}

// Close a valve
bool Automation::ValveClose(int num_vanne)
{
	// Close valve
	if(!g_pVanne->Fermer(num_vanne))
		return false;

	// Tell GUI valve is closed
	messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSED, num_vanne);
	
	// Return success
	return true;
}

// Open and close a valve
bool Automation::ValveOpenClose(int num_vanne)
{
	// Tell GUI what is going on
	messageHandler.DisplayMessage(MESSAGE_VALVE_OPENCLOSE, num_vanne);

	bool ouverture_reussie = ValveOpen(num_vanne);
	Sleep(temps_ouverture);
	bool fermeture_reussie = ValveClose(num_vanne);
	Sleep(temps_fermeture);
	if (ouverture_reussie && fermeture_reussie)
		return true;

	// Return success
	return false;
}

// Activate EV number x
bool Automation::EVActivate(int num_ev)
{
	if (!g_pVanne->ActiverEV(num_ev))
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_EV_ACTIVATED, num_ev);

	// Return success
	return true;
}

// Deactivate EV number x
bool Automation::EVDeactivate(int num_ev)
{
	if (!g_pVanne->DesactiverEV(num_ev))
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_EV_DEACTIVATED, num_ev);

	// Return success
	return true;
}

// Activate pump
bool Automation::ActivatePump()
{
	if (!g_pVanne->ActiverPompe())
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_PUMP_ACTIVATED);

	// Return success
	return true;
}

// Deactivate pump
bool Automation::DeactivatePump()
{
	if (!g_pVanne->DesactiverPompe())
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_PUMP_ACTIVATED);

	// Return success
	return true;
}

// Close all valves
bool Automation::ValvesCloseAll()
{
	if (!g_pVanne->FermerToutesLesVannes())
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSEALL);

	// Return success
	return true;
}

// Close the pump
bool Automation::ValvesAndPumpClose()
{
	if (!g_pVanne->FermerLesValvesEtLaPompe())
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_PUMP_VALVE_CLOSEALL);

	// Return success
	return true;
}

// Close everything
bool Automation::ControlMechanismsCloseAll()
{
	if (!ValvesAndPumpClose())
		return false;
	if (!ValvesCloseAll())
		return false;

	// Tell GUI
	messageHandler.DisplayMessage(MESSAGE_CLOSEEVERYTHING);

	// Return success
	return true;
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