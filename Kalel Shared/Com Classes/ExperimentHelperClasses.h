#ifndef CLASSES_EXPERIENCES_H
#define CLASSES_EXPERIENCES_H
#pragma once

#include <string>

class experimentateur
{
	public :
		std::wstring nom;
		std::wstring surnom;

		experimentateur operator=(const experimentateur &exp);
		bool operator!=(const experimentateur &exp);
};

class gaz
{
public :
	std::wstring nom;
	std::wstring symbole;
	double masse_moleculaire;
	double temperature_critique;
	double pression_critique;
	double temperature_ebullition;
	double omega;

	gaz operator=(const gaz &g);
	bool operator!=(const gaz &g);
};

class cellule
{
public :
	std::wstring numero;
	double volume_total;
	double volume_calo;

	cellule operator=(const cellule &cell);
	bool operator!=(const cellule &cell);
};

class Donnees_General
{
public:
	experimentateur experimentateur;
	gaz gaz;
	std::wstring nom_echantillon;
	float masse_echantillon;
	std::wstring commentaires;
	std::wstring chemin;
	std::wstring fichier;
	float temperature_experience;
	std::wstring date_experience;

	Donnees_General operator=(const Donnees_General &general);
	bool operator!=(const Donnees_General &general);
};

class Donnees_Divers
{
public:
	cellule cellule;
	int temps_ligne_base;
	bool mise_sous_vide_fin_experience;
	int temps_vide;

	Donnees_Divers operator=(const Donnees_Divers &divers);
	bool operator!=(const Donnees_Divers &divers);
};

/// General doses class
class Donnees_Doses
{
public:
	float delta_pression;
	float pression_finale;
	int temps_adsorption;
	int temps_volume;

	Donnees_Doses operator=(const Donnees_Doses &petites_doses);
	bool operator!=(const Donnees_Doses &petites_doses);
	bool operator!=(const Donnees_Doses &petites_doses) const;
};

class Donnees_Desorption
{
public:
	float delta_pression;
	float pression_finale;
	int temps_desorption;
	int temps_volume;

	Donnees_Desorption operator=(const Donnees_Desorption &desorption);
	bool operator!=(const Donnees_Desorption &desorption);
	bool operator!=(const Donnees_Desorption &desorption) const;
};

class Donnees_Adsorption_Continue
{
public:
	int temps_etalonnage_debit;
	int temps_etalonnage_volume_inter;
	int temps_equilibre_continue;
	int temps_final_equilibre;
	float pression_finale_adsorption_continue;

	Donnees_Adsorption_Continue operator=(const Donnees_Adsorption_Continue &adsorption);
	bool operator!=(const Donnees_Adsorption_Continue &adsorption);
};

#endif