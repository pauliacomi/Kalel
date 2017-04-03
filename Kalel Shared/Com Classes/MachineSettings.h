#ifndef MACHINESETTINGS_H
#define MACHINESETTINGS_H
#pragma once

#include <string>
#include <vector>
#include <map>

struct Instrument
{
	int name = 0;						// keithley/mensor/national instruments
	int port = 0;						// port being used (USB1, COM1, DEV3)
	bool initialised = false;			//
};

struct Reader
{
	int type = -1;						// Pressure / Temperature etc
	int identifier = -1;				// Number low pressure, calo temperature
	long double sensitivity = 0;		// sensitivity of measurement
	int channel = -1;					// channel 1/2 of the instrument
	int instrument = -1;				// key of the instrument used

};

struct Controller
{
	int type = -1;						// Pressure / Temperature etc
	int identifier = -1;				// Number 1/ 2 low pressure etc
	long double sensitivity = 0;		// sensitivity of it
	int channel = -1;					// channel 1/2 of the instrument
	int instrument = -1;
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


	// Functions to easily create instruments or readers
	void AddInstrument(Instrument i, unsigned int position = 0);
	void AddReader(Reader r, unsigned int position = 0);
	void AddController(Controller r, unsigned int position = 0);
};

#endif