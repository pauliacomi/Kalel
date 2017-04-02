#ifndef MACHINESETTINGS_H
#define MACHINESETTINGS_H
#pragma once

#include <string>
#include <vector>
#include <map>

struct Instrument
{
	int name;					// keithley/mensor/national instruments
	int port;					// port being used (USB1, COM1, DEV3)
	bool initialised = false;	//
};

struct Reader
{
	int type;						// Pressure / Temperature etc
	int identifier;					// Number low pressure, calo temperature
	long double sensitivity;		// sensitivity of measurement
	int channel;					// channel 1/2 of the instrument
	int instrument;					// key of the instrument used
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

	std::wstring CaloName = std::to_wstring(1);
	std::wstring CaloEntete = std::to_wstring(1);
	std::wstring CheminFichierGeneral = std::to_wstring(1);

	bool ActivationSecurite = true;							// Used for activating the automatic security
	float PressionSecuriteBassePression = 1;				// Considered the maximum pressure for the low range pressure transmitter
	float PressionSecuriteHautePression = 5;				// Considered the maximum pressure for the high range pressure transmitter

	float PressionLimiteVide = 0.1;							// Used for determining the pressure considered "good vacuum"
	
	bool PresenceTuyereSonique = false;						// Used for user information (no calculations)
	float VolumeRef = 1;									// Used for user information (no calculations)
	float VolumeP6 = 1;										// Used for user information (no calculations)

	std::map<unsigned int, Instrument> instruments;			// The instruments in the machine
	std::map<unsigned int, Reader>	readers;				// The readers which are available
	std::map<unsigned int, Controller>	controllers;		// The controllers which are available

};

#endif