#include "ValveController.h"

#include "../../../Kalel Shared/log.h"										// Logging
#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../../Kalel Shared/Resources/DefineText.h"

#define nb_essais 3
#define temps_ms 10


ValveController::ValveController(int port) 
	: NI_USB_6008(port)
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
		success = NI_USB_6008::SetSubchannel(0, num -1, true);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_VALVE_OPENED << num;
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
		success = NI_USB_6008::SetSubchannel(0, num -1, false);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_VALVE_CLOSED << num;
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
		success = NI_USB_6008::SetSubchannel(1, num, true);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_EV_ACTIVATED << num;
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
		success = NI_USB_6008::SetSubchannel(1, num, false);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}
	
	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_EV_DEACTIVATED << num;
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
		success = NI_USB_6008::SetSubchannel(1, 2, true);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_ACTIVATED;
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
		success = NI_USB_6008::SetSubchannel(1, 2, false);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_DEACTIVATED;
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
		LOG(logINFO) << MESSAGE_CLOSEEVERYTHING;
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
		success = NI_USB_6008::SetChannelAll(0, false);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_VALVE_CLOSEALL;
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
		success = NI_USB_6008::SetChannelAll(1, false);
		if (!success)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(temps_ms));
		}
	}

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_VALVE_CLOSEALL;
	}

	// Return success
	return success;
}


int ValveController::GetReadPort()
{	return NI_USB_6008::GetComPort();	}

void ValveController::SetReadPort(int port)
{
	// Lock for the remainder of function
	std::lock_guard<std::mutex> lk(ctrlmutex);
	return NI_USB_6008::SetComPort(port);	
}


///////////////////////////////////////////////////////
// Functions checking for the state of the equipemt

bool ValveController::ValveIsOpen(int num)
{return NI_USB_6008::IsOpenSubchannel(0, num-1);}

bool ValveController::EVIsActive(int num)
{return NI_USB_6008::IsOpenSubchannel(1, num-1);}

bool ValveController::PumpIsActive()
{return NI_USB_6008::IsOpenSubchannel(1, 2);}
