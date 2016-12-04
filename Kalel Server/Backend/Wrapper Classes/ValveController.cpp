#include "ValveController.h"
#include <Windows.h>

#include "../../MessageHandler.h"
#include "../../../Kalel Shared/Resources/DefineText.h"

#define nb_essais 3
#define temps_ms 100



ValveController::ValveController(MessageHandler & messageHandler) 
	: NI_USB_6008()
	, messageHandler{ messageHandler }
{
	NI_USB_6008::SetDevNI_USB_6008(GetPortVannes());
}

ValveController::~ValveController(void)
{
}



bool ValveController::ValveOpen(int num, bool verbose)
{
	// Open valve
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::OuvrirPort0(num - 1);
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_VALVE_OPENED, num);
	}

	// Return success
	return success;
}

bool ValveController::ValveClose(int num, bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort0(num - 1);
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSED, num);
	}

	// Return success
	return success;
}


// Open and close a valve
bool ValveController::ValveOpenClose(int num, bool verbose)
{
	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_VALVE_OPENCLOSE, num);
	}

	bool ouverture_reussie = ValveOpen(num, false);
	_ASSERT("");
	//WaitSeconds(TIME_WAIT_VALVES);
	bool fermeture_reussie = ValveClose(num, false);
	//WaitSeconds(TIME_WAIT_VALVES);
	if (ouverture_reussie && fermeture_reussie)
		return true;

	// Return success
	return false;
}


bool ValveController::EVActivate(int num, bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::OuvrirPort1(0 + num);
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_EV_ACTIVATED, num);
	}

	// Return success
	return success;
}

bool ValveController::EVDeactivate(int num, bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort1(0 + num);
		if (!success)
		{
			Sleep(temps_ms);
		}
	}
	
	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_EV_DEACTIVATED, num);
	}

	// Return success
	return success;
}

bool ValveController::PumpActivate(bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::OuvrirPort1(2);
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_PUMP_ACTIVATED);
	}

	// Return success
	return success;
}

bool ValveController::PumpDeactivate(bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort1(2);
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_PUMP_DEACTIVATED);
	}

	// Return success
	return success;
}

bool ValveController::CloseAll(bool verbose)
{
	bool success = false;
	if (CloseAllValves(false) && CloseEVsAndPump(false)) {
		success = true;
	}
	
	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_CLOSEEVERYTHING);
	}

	// Return success
	return success;
}

bool ValveController::CloseAllValves(bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort0Tous();
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSEALL);
	}

	// Return success
	return success;
}

bool ValveController::CloseEVsAndPump(bool verbose)
{
	int tentative = 0;
	bool success{ false };
	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort1Tous();
		if (!success)
		{
			Sleep(temps_ms);
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_PUMP_VALVE_CLOSEALL);
	}

	// Return success
	return success;
}


int ValveController::PortUSB()
{
	return NI_USB_6008::GetDevNI_USB_6008();
}


///////////////////////////////////////////////////////
// Functions checking for the state of the equipemt

bool ValveController::VanneEstOuvert(int num)
{return NI_USB_6008::EstOuvertPort0(num-1);}

bool ValveController::VanneEstFerme(int num)
{return NI_USB_6008::EstFermePort0(num-1);}

bool ValveController::EV1EstActive()
{return NI_USB_6008::EstOuvertPort1(0);}

bool ValveController::EV1EstDesactive()
{return NI_USB_6008::EstFermePort1(0);}

bool ValveController::EV2EstActive()
{return NI_USB_6008::EstOuvertPort1(1);}

bool ValveController::EV2EstDesactive()
{return NI_USB_6008::EstFermePort1(1);}

bool ValveController::PompeEstActive()
{return NI_USB_6008::EstOuvertPort1(2);}

bool ValveController::PompeEstDesactive()
{return NI_USB_6008::EstFermePort1(2);}


