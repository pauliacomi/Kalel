#pragma once

/**********************************************************************************************************************************
*
// Security measures and checks
*
***********************************************************************************************************************************/

class Security {

public:
	Security(bool activated, float PressureHigh_HighRange, float PressureHigh_LowRange);
	~Security();

protected:

	bool securityActivated;

	// Keep limits in memory
	float security_PressureHigh_HighRange;
	float security_PressureHigh_LowRange;

	// Bool flags to keep track of security
	bool security_PressureHigh_flag;
	bool security_TemperatureHigh_flag;
	bool security_TemperatureLow_flag;


	void SecuriteHautePression();
	void SecuriteHautePressionManuelle();
	void SecuriteHautePressionAuto();

	void SecuriteTemperatures();
	void SecuriteTemperaturesManuelle();
	void SecuriteTemperaturesAuto();
};
