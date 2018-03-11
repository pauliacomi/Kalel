#pragma once

#include <string>
#include <functional>	// for function
#include <memory>		// for pointers


class user
{
public :
	std::wstring nom;
	std::wstring surnom;

	user operator=(const user &u);
	bool operator!=(const user &u);
};

class gas
{
public :
	std::wstring nom;
	std::wstring symbole;
	double masse_moleculaire;
	double temperature_critique;
	double pression_critique;
	double temperature_ebullition;
	double omega;

	gas operator=(const gas &g);
	bool operator!=(const gas &g);
};

class cell
{
public :
	std::wstring numero;
	double volume_total;
	double volume_calo;

	cell operator=(const cell &c);
	bool operator!=(const cell &c);
};

class data_general
{
public:
	user user;
	gas gas;
	std::wstring nom_echantillon;
	float masse_echantillon;
	std::wstring commentaires;
	std::wstring chemin;
	std::wstring fichier;
	float temperature_experience;
	std::wstring date_experience;

	data_general operator=(const data_general &general);
	bool operator!=(const data_general &general);
};

class data_other
{
public:
	cell cell;
	int temps_ligne_base;
	bool mise_sous_vide_fin_experience;
	int temps_vide;

	data_other operator=(const data_other &divers);
	bool operator!=(const data_other &divers);
};

/// General doses class
class data_adsorption
{
public:
	float delta_pression;
	float pression_finale;
	int temps_adsorption;
	int temps_volume;

	data_adsorption operator=(const data_adsorption &petites_doses);
	bool operator!=(const data_adsorption &petites_doses);
	bool operator!=(const data_adsorption &petites_doses) const;
};

class data_desorption
{
public:
	float delta_pression;
	float pression_finale;
	int temps_desorption;
	int temps_volume;

	data_desorption operator=(const data_desorption &desorption);
	bool operator!=(const data_desorption &desorption);
	bool operator!=(const data_desorption &desorption) const;
};

class data_continuous
{
public:
	int temps_etalonnage_debit;
	int temps_etalonnage_volume_inter;
	int temps_equilibre_continue;
	int temps_final_equilibre;
	float pression_finale_adsorption_continue;

	data_continuous operator=(const data_continuous &adsorption);
	bool operator!=(const data_continuous &adsorption);
};


class Instrument
{
public:
	unsigned int type = 0;				// keithley/mensor/national instruments
	std::wstring port_type;				// port type
	unsigned int port = 0;				// port being used (USB1, COM1, DEV3)
	bool initialised = false;			// whether the instrument is intialised, internal
	unsigned int iNumber = 0;

	Instrument operator=(const Instrument &i);
	bool operator==(const Instrument &i);
	bool operator==(const Instrument &i) const;
};

class Reader
{
public:
	unsigned int type = 0;				// Pressure / Temperature etc
	unsigned int identifier = 0;		// Number low pressure, calo temperature
	unsigned int channel = 0;			// channel 1/2 of the instrument
	unsigned int instrument = 0;		// key of the instrument used
	long double sensitivity = 1;		// sensitivity of measurement
	std::function<double(void)>			readerfunction = []() {return 0; };

	Reader operator=(const Reader &r);
	bool operator==(const Reader &r);
};

class Controller
{
public:
	unsigned int type = 0;				// Valve, Pump, etc
	unsigned int identifier = 0;		// Number 1/2
	unsigned int channel = 0;			// channel 1/2 of the instrument
	unsigned int instrument = 0;		// Key of the associated instrument
	std::function<bool(void)>			readerfunction = []() {return false; };
	std::function<bool(bool)>			controllerfunction = [](bool) {return false; };

	Controller operator=(const Controller &c);
	bool operator==(const Controller &c);
};