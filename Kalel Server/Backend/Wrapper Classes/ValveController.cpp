#include "ValveController.h"

#include "../../../Kalel Shared/log.h"										// Logging
#include "../../../Kalel Shared/stringHelpers.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../../Kalel Shared/Resources/DefineText.h"
 

ValveController::ValveController(Instruments & i)
	: instruments {i}
{
}

ValveController::~ValveController(void)
{
}


bool ValveController::ValveOpen(int num, bool verbose)
{
	bool success = instruments.ActuateController(num, true);

	// Log message
	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_VALVE_OPENED, num - CONTROLLER_PUMP);
	}

	return success;
}

bool ValveController::ValveClose(int num, bool verbose)
{
	bool success = instruments.ActuateController(num, false);

	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_VALVE_CLOSED, num - CONTROLLER_PUMP);
	}

	return success;
}

bool ValveController::EVActivate(int num, bool verbose)
{
	bool success = instruments.ActuateController(num, true);

	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_EV_ACTIVATED, num - CONTROLLER_EV);
	}

	return success;
}

bool ValveController::EVDeactivate(int num, bool verbose)
{
	bool success = instruments.ActuateController(num, false);
	
	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_EV_DEACTIVATED, num - CONTROLLER_EV);
	}

	return success;
}

bool ValveController::PumpActivate(bool verbose)
{
	bool success = instruments.ActuateController(ID_PUMP, true);

	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_ACTIVATED;
	}

	return success;
}

bool ValveController::PumpDeactivate(bool verbose)
{
	bool success = instruments.ActuateController(ID_PUMP, false);

	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_DEACTIVATED;
	}

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
	//bool success = instruments.ActuateController(CONTROLLER_EV + num - 1, true);
	bool success = true;

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_VALVE_CLOSEALL;
	}

	// Return success
	return success;
}

bool ValveController::CloseEVsAndPump(bool verbose)
{
	//bool success = instruments.ActuateController(, true);
	bool success = true;

	// Log message
	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_VALVE_CLOSEALL;
	}

	// Return success
	return success;
}

///////////////////////////////////////////////////////
// Functions checking for the state of the equipemt

bool ValveController::ValveIsOpen(int num)
{
	return instruments.MeasureController(num);
}

bool ValveController::EVIsActive(int num)
{
	return instruments.MeasureController(num);
}

bool ValveController::PumpIsActive()
{
	return instruments.MeasureController(ID_PUMP);
}
