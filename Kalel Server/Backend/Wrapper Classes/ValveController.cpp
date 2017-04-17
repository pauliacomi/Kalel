#include "ValveController.h"

#include "../../MessageHandler.h"
#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../../Kalel Shared/Resources/DefineText.h"

#define nb_essais 3
#define temps_ms 10


ValveController::ValveController(MessageHandler & messageHandler, int port) 
	: NI_USB_6008(port)
	, messageHandler{ messageHandler }
{
}

ValveController::~ValveController(void)
{
}

void ValveController::Reset(MachineSettings & m)
{
	for (auto i = m.instruments.begin(); i != m.instruments.end(); ++i) {
		if (i->second.type == INSTRUMENT_NI_USB_6008) {
			if (i->second.port != GetReadPort())
			{
				SetReadPort(i->second.port);
			}
		}
	}
}



bool ValveController::ValveOpen(int num, bool verbose)
{
	// Open valve
	int tentative = 0;
	bool success{ false };
	
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::OuvrirPort0(num - 1);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
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

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort0(num - 1);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_VALVE_CLOSED, num);
	}

	// Return success
	return success;
}

bool ValveController::EVActivate(int num, bool verbose)
{
	int tentative = 0;
	bool success{ false };

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::OuvrirPort1(0 + num);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
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

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort1(0 + num);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
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

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::OuvrirPort1(2);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
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

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort1(2);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
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

	// No need to lock
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

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort0Tous();
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
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

	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);

	while (!success && tentative <= nb_essais)
	{
		tentative++;
		success = NI_USB_6008::FermerPort1Tous();
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		messageHandler.DisplayMessage(MESSAGE_PUMP_VALVE_CLOSEALL);
	}

	// Return success
	return success;
}


int ValveController::GetReadPort()
{	return NI_USB_6008::GetDevNI_USB_6008();	}

void ValveController::SetReadPort(int port)
{
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);
	return NI_USB_6008::SetDevNI_USB_6008(port);	
}


///////////////////////////////////////////////////////
// Functions checking for the state of the equipemt

bool ValveController::ValveIsOpen(int num)
{return NI_USB_6008::EstOuvertPort0(num-1);}

bool ValveController::EV1IsActive()
{return NI_USB_6008::EstOuvertPort1(0);}

bool ValveController::EV2IsActive()
{return NI_USB_6008::EstOuvertPort1(1);}

bool ValveController::PumpIsActive()
{return NI_USB_6008::EstOuvertPort1(2);}
