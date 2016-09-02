#ifndef PARSERXML_H
#define PARSERXML_H
#pragma once

#include "Com Classes/ExperimentHelperClasses.h"

#include <string>
#include <vector>

// Forward declarations
class TiXmlElement;
class TiXmlHandle;

// XML
void Initialisation_XML();
bool ConfigsExists();
TiXmlElement * NewElement(std::wstring nom_element, std::wstring text_element);
TiXmlElement * NewElement(std::wstring nom_element, int int_element);
TiXmlElement * NewElement(std::wstring nom_element, float float_element);
TiXmlElement * NewElement(std::wstring nom_element, double double_element);

// Experimentateur
std::vector<experimentateur> GetExperimentateurs();
bool Rajout_Experimentateur(std::wstring nom,std::wstring surnom,int index=-1);
bool Rajout_Experimentateur(experimentateur new_exp,int index=-1);
bool Suppression_Experimentateur(int index);
bool Modif_Experimentateur(std::wstring nom, std::wstring surnom,int index);
bool Modif_Experimentateur(experimentateur new_exp,int index);

// Gaz
std::vector<gaz> GetGazs();
bool Rajout_Gaz(std::wstring nom,std::wstring symbole,double masse,double temp_critique, 
				double pres_critique, double temp_ebullition, int index=-1);
bool Rajout_Gaz(gaz new_gaz,int index=-1);
bool Suppression_Gaz(int index);
bool Modif_Gaz(std::wstring nom,std::wstring symbole,double masse,double temp_critique, 
			   double pres_critique, double temp_ebullition, int index);
bool Modif_Gaz(gaz new_gaz,int index);

// Cellule
std::vector<cellule> GetCellules();
bool Rajout_Cellule(std::wstring num, double total, double calo, int index=-1);
bool Rajout_Cellule(cellule new_cellule,int index=-1);
bool Suppression_Cellule(int index);
bool Modif_Cellule(std::wstring num, double total, double calo, int index);
bool Modif_Cellule(cellule new_cellule,int index);

// Doublon
bool Doublon(TiXmlHandle& handle_root, std::wstring valeur, std::wstring type);
bool DoublonNomExperimentateur(std::wstring valeur);
bool DoublonSurnomExperimentateur(std::wstring valeur);
bool DoublonExperimentateur(std::wstring valeur, std::wstring type);

bool DoublonNomGaz(std::wstring valeur);
bool DoublonSymboleGaz(std::wstring valeur);
bool DoublonGaz(std::wstring valeur, std::wstring type);

bool DoublonNumeroCellule(std::wstring num);

#endif