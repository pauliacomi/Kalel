////////////////////////////////////////////////
// Modifié le 7 septembre 2009
////////////////////////////////////////////////

#ifndef PARSERXML_H
#define PARSERXML_H

#include "tinystr.h"
#include "tinyxml.h"

#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <vector>

#include <iostream>
#include <list>

#include "Classes_experiences.h"

using namespace std;

/*
class experimentateur
{
	public :
		std::string nom;
		std::string surnom;
};

class gaz
{
public :
	std::string nom;
	std::string symbole;
	double masse_moleculaire;
	double temperature_critique;
	double pression_critique;
	double temperature_ebullition;
	double omega;
};

class cellule
{
public :
	std::string numero;
	double volume_total;
	double volume_calo;
};
*/

// XML
void Initialisation_XML();
bool ConfigsExists();
TiXmlElement * NewElement(std::string nom_element, std::string text_element);
TiXmlElement * NewElement(std::string nom_element, int int_element);
TiXmlElement * NewElement(std::string nom_element, float float_element);
TiXmlElement * NewElement(std::string nom_element, double double_element);

// Experimentateur
vector<experimentateur> GetExperimentateurs();
bool Rajout_Experimentateur(std::string nom,std::string surnom,int index=-1);
bool Rajout_Experimentateur(experimentateur new_exp,int index=-1);
bool Suppression_Experimentateur(int index);
bool Modif_Experimentateur(std::string nom, std::string surnom,int index);
bool Modif_Experimentateur(experimentateur new_exp,int index);

// Gaz
vector<gaz> GetGazs();
bool Rajout_Gaz(std::string nom,std::string symbole,double masse,double temp_critique, 
				double pres_critique, double temp_ebullition, int index=-1);
bool Rajout_Gaz(gaz new_gaz,int index=-1);
bool Suppression_Gaz(int index);
bool Modif_Gaz(std::string nom,std::string symbole,double masse,double temp_critique, 
			   double pres_critique, double temp_ebullition, int index);
bool Modif_Gaz(gaz new_gaz,int index);

// Cellule
vector<cellule> GetCellules();
bool Rajout_Cellule(std::string num, double total, double calo, int index=-1);
bool Rajout_Cellule(cellule new_cellule,int index=-1);
bool Suppression_Cellule(int index);
bool Modif_Cellule(std::string num, double total, double calo, int index);
bool Modif_Cellule(cellule new_cellule,int index);

// Doublon
bool Doublon(TiXmlHandle handle_root, std::string valeur, std::string type);
bool DoublonNomExperimentateur(std::string valeur);
bool DoublonSurnomExperimentateur(std::string valeur);
bool DoublonExperimentateur(std::string valeur, std::string type);

bool DoublonNomGaz(std::string valeur);
bool DoublonSymboleGaz(std::string valeur);
bool DoublonGaz(std::string valeur, std::string type);

bool DoublonNumeroCellule(std::string num);

#endif