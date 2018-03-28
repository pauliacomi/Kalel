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
	bool success = instruments.ActuateController(CONTROLLER_VALVE + num - 1, true);

	// Log message
	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_VALVE_OPENED, num);
	}

	return success;
}

bool ValveController::ValveClose(int num, bool verbose)
{
	bool success = instruments.ActuateController(CONTROLLER_VALVE + num - 1, false);

	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_VALVE_CLOSED, num);
	}

	return success;
}

bool ValveController::EVActivate(int num, bool verbose)
{
	bool success = instruments.ActuateController(CONTROLLER_EV + num - 1, true);

	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_EV_ACTIVATED, num);
	}

	return success;
}

bool ValveController::EVDeactivate(int num, bool verbose)
{
	bool success = instruments.ActuateController(CONTROLLER_EV + num - 1, false);
	
	if (verbose) {
		LOG(logINFO) << stringh::string_format(MESSAGE_EV_DEACTIVATED, num);
	}

	return success;
}

bool ValveController::PumpActivate(bool verbose)
{
	bool success = instruments.ActuateController(CONTROLLER_PUMP, true);

	if (verbose) {
		LOG(logINFO) << MESSAGE_PUMP_ACTIVATED;
	}

	return success;
}

bool ValveController::PumpDeactivate(bool verbose)
{
	bool success = instruments.ActuateController(CONTROLLER_PUMP, false);

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
	return instruments.MeasureController(CONTROLLER_VALVE + num -1);
}

bool ValveController::EVIsActive(int num)
{
	return instruments.MeasureController(CONTROLLER_EV + num - 1);
}

bool ValveController::PumpIsActive()
{
	return instruments.MeasureController(CONTROLLER_PUMP);
}
