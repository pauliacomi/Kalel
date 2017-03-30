#ifndef MACHINESETTINGS_H
#define MACHINESETTINGS_H
#pragma once

#include <string>
#include <vector>

struct Instrument
{
	int type;
	int instrument;
	int function;
	int port;
	int channel;
};

struct Reader
{
	int identifier;
	long double sensitivity;
	std::vector<Instrument*> instruments;
};

class MachineSettings
{
public:
	MachineSettings();
	~MachineSettings();

	long double SensitivityCalo;					// Used for the calorimetry measurements
	float SensitivityLowPRange;						// Used for the pressure measurements
	float SensitivityHighPRange;					// Used for the pressure measurements

	bool ActivationSecurite;						// Used for activating the automatic security
	float PressionSecuriteBassePression;			// Considered the maximum pressure for the low range pressure transmitter
	float PressionSecuriteHautePression;			// Considered the maximum pressure for the high range pressure transmitter

	float PressionLimiteVide;						// Used for determining the pressure considered "good vacuum"
	
	bool PresenceTuyereSonique;						// Used for user information (no calculations)
	float VolumeRef;								// Used for user information (no calculations)
	float VolumeP6;									// Used for user information (no calculations)

	std::vector<Instrument> instruments;			// The instruments in the machine

	std::vector<Reader> calorimeter;
	std::vector<Reader> pressure;
	std::vector<Reader> temperatures;
	std::vector<Reader> valves;
	std::vector<Reader> pumps;

	std::wstring CaloName;
	std::wstring CaloEntete;

	std::wstring CheminFichierGeneral;

	bool CaloToMeasure;
	bool LowPressureToMeasure;
	bool HighPressureToMeasure;

	int TypeInstrument(int num);
	int COMInstrument(int num);
	int FonctionInstrument(int num);
	void SetInstrument(int num, int TypeInstrument, int COMInstrument, int FonctionInstrument);
	void SetTypeInstrument(int num, int TypeInstrument);
	void SetCOMInstrument(int num, int COMInstrument);
	void SetFunctionInstrument(int num, int FonctionInstrument);
};

#endif