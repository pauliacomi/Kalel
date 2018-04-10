#pragma once

/**********************************************************************************************************************************
*
// Security measures and checks
*
***********************************************************************************************************************************/
class ValveController;
class Storage;

class Security {

public:
	Security(ValveController & valveControl);
	~Security();

	void SecurityOverPressure(const Storage & storage);
	void SecurityTemperatures(const Storage & storage);

private:
	ValveController & valveController;

protected:

	// Bool flags to keep track of security
	bool security_PressureHigh_flag = false;
	bool security_PressureLow_flag = false;
	bool security_TemperatureHigh_flag = false;
	bool security_TemperatureLow_flag = false;


	void SecurityHighPressureManual(const Storage & storage);
	void SecurityHighPressureAuto(const Storage & storage);

	void SecurityTemperaturesManual(const Storage & storage);
	void SecuriteTemperaturesAuto(const Storage & storage);
};
