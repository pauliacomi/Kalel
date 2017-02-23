#pragma once

/**********************************************************************************************************************************
*
// Security measures and checks
*
***********************************************************************************************************************************/
class MessageHandler;
class ValveController;
class ExperimentData;

class Security {

public:
	Security(bool activated, ValveController & valveControl, MessageHandler & messageHandler);
	~Security();

	void SecurityHighPressure(int experimentType, float maxPlow, float maxPhigh, const ExperimentData &expData);
	void SecurityTemperatures(int experimentType, float maxPlow, float maxPhigh, const ExperimentData &expData);

private:
	ValveController & valveController;
	MessageHandler & messageHandler;

protected:

	bool securityActivated;

	// Bool flags to keep track of security
	bool security_PressureHigh_flag = false;
	bool security_TemperatureHigh_flag = false;
	bool security_TemperatureLow_flag = false;


	void SecurityHighPressureManual(float maxPlow, float maxPhigh, const ExperimentData &expData);
	void SecurityHighPressureAuto(float maxPlow, float maxPhigh, const ExperimentData &expData);

	void SecurityTemperaturesManual(double maximumT, double minimumT, const ExperimentData &expDat);
	void SecuriteTemperaturesAuto(double maximumT, double minimumT, const ExperimentData &expDat);
};
