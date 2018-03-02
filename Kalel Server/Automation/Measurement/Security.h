#pragma once

/**********************************************************************************************************************************
*
// Security measures and checks
*
***********************************************************************************************************************************/
class ValveController;
class ExperimentData;

class Security {

public:
	Security(bool activated, ValveController & valveControl);
	~Security();

	void SecurityOverPressure(int experimentType, double maxPlow, double maxPhigh, const ExperimentData &expData);
	void SecurityTemperatures(int experimentType, double maxPlow, double maxPhigh, const ExperimentData &expData);

private:
	ValveController & valveController;

protected:

	bool securityActivated;

	// Bool flags to keep track of security
	bool security_PressureHigh_flag = false;
	bool security_TemperatureHigh_flag = false;
	bool security_TemperatureLow_flag = false;


	void SecurityHighPressureManual(double maxPlow, double maxPhigh, const ExperimentData &expData);
	void SecurityHighPressureAuto(double maxPlow, double maxPhigh, const ExperimentData &expData);

	void SecurityTemperaturesManual(double maximumT, double minimumT, const ExperimentData &expDat);
	void SecuriteTemperaturesAuto(double maximumT, double minimumT, const ExperimentData &expDat);
};
