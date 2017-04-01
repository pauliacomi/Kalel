#ifndef MACHINESETTINGS_H
#define MACHINESETTINGS_H
#pragma once

#include <string>
#include <vector>
#include <map>

struct Instrument
{
	int type;			// serial/usb/etc
	int name;			// keithley/mensor/national instruments
	int function;		// ?
	int port;			// port being used (USB1, COM1, DEV3)
	bool initialised = false;	//
};

struct Reader
{
	int type;						// Pressure / Temperature etc
	int identifier;					// Number 1/ 2 low pressure etc
	long double sensitivity;		// sensitivity of it
	int channel;					// channel 1/2 of the instrument
	int instrument;
};

struct Controller
{
	int type;						// Pressure / Temperature etc
	int identifier;					// Number 1/ 2 low pressure etc
	long double sensitivity;		// sensitivity of it
	int channel;					// channel 1/2 of the instrument
	int instrument;
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

	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available

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