#pragma once

#include <string>

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