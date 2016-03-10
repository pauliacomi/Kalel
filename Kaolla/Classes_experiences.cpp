#include "StdAfx.h"

#include <string.h>

#include "Classes_experiences.h"

experimentateur experimentateur::operator=(const experimentateur &exp)
{
	nom = exp.nom;
	surnom = exp.surnom;

	return *this;
}

bool experimentateur::operator!=(const experimentateur &exp)
{
	if (nom != exp.nom || surnom != exp.surnom)
		return true;

	return false;
}



gaz gaz::operator=(const gaz &g)
{
	nom = g.nom;
	symbole = g.symbole;
	masse_moleculaire = g.masse_moleculaire;
	temperature_critique = g.temperature_critique;
	pression_critique = g.pression_critique;
	temperature_ebullition = g.temperature_ebullition;
	omega = g.omega;

	return *this;
}

bool gaz::operator!=(const gaz &g)
{
	if (nom != g.nom || symbole != g.symbole 
			|| masse_moleculaire != g.masse_moleculaire
			|| temperature_critique != g.temperature_critique
			|| pression_critique != g.pression_critique
			|| temperature_ebullition != g.temperature_ebullition
			|| omega != g.omega)
		return TRUE;
	return FALSE;
}




cellule cellule::operator=(const cellule &cell)
{
	numero = cell.numero;
	volume_total = cell.volume_total;
	volume_calo = cell.volume_calo;

	return *this;
}

bool cellule::operator!=(const cellule &cell)
{
	if (numero != cell.numero || 
			volume_total != cell.volume_total ||
			volume_calo != cell.volume_calo)
		return TRUE;
	return FALSE;
}



Donnees_General Donnees_General::operator=(const Donnees_General &general)
{
	experimentateur = general.experimentateur;
	gaz = general.gaz;
	nom_echantillon = general.nom_echantillon;
	masse_echantillon = general.masse_echantillon;
	commentaires = general.commentaires;
	chemin = general.chemin;
	fichier = general.fichier;
	temperature_experience = general.temperature_experience;
	date_experience = general.date_experience;

	return *this;
}

bool Donnees_General::operator!=(const Donnees_General &general)
{
	if (experimentateur != general.experimentateur ||
			gaz != general.gaz ||
			nom_echantillon != general.nom_echantillon ||
			masse_echantillon != general.masse_echantillon ||
			commentaires != general.commentaires ||
			chemin != general.chemin ||
			fichier != general.fichier ||
			temperature_experience != general.temperature_experience ||
			date_experience != general.date_experience)
		return TRUE;

	return FALSE;
}




Donnees_Divers Donnees_Divers::operator=(const Donnees_Divers &divers)
{
	cellule = divers.cellule;
	temps_ligne_base = divers.temps_ligne_base;
	mise_sous_vide_fin_experience = divers.mise_sous_vide_fin_experience;
	temps_vide = divers.temps_vide;

	return *this;
}

bool Donnees_Divers::operator!=(const Donnees_Divers &divers)
{
	if (cellule != divers.cellule ||
			temps_ligne_base != divers.temps_ligne_base ||
			mise_sous_vide_fin_experience != divers.mise_sous_vide_fin_experience ||
			temps_vide != divers.temps_vide)
		return TRUE;
	return FALSE;
}




Donnees_Petites_Doses Donnees_Petites_Doses::operator=(const Donnees_Petites_Doses &petites_doses)
{
	a_effectuer = petites_doses.a_effectuer;
	delta_pression = petites_doses.delta_pression;
	pression_finale = petites_doses.pression_finale;
	temps_adsorption = petites_doses.temps_adsorption;
	temps_volume = petites_doses.temps_volume;

	return *this;
}

bool Donnees_Petites_Doses::operator!=(const Donnees_Petites_Doses &petites_doses)
{
	if (a_effectuer != petites_doses.a_effectuer ||
			delta_pression != petites_doses.delta_pression ||
			pression_finale != petites_doses.pression_finale ||
			temps_adsorption != petites_doses.temps_adsorption ||
			temps_volume != petites_doses.temps_volume)
		return TRUE;
	return FALSE;
}


Donnees_Moyennes_Doses Donnees_Moyennes_Doses::operator=(const Donnees_Moyennes_Doses &moyennes_doses)
{
	a_effectuer = moyennes_doses.a_effectuer;
	delta_pression = moyennes_doses.delta_pression;
	pression_finale = moyennes_doses.pression_finale;
	temps_adsorption = moyennes_doses.temps_adsorption;
	temps_volume = moyennes_doses.temps_volume;

	return *this;
}

bool Donnees_Moyennes_Doses::operator!=(const Donnees_Moyennes_Doses &moyennes_doses)
{
	if (a_effectuer != moyennes_doses.a_effectuer ||
			delta_pression != moyennes_doses.delta_pression ||
			pression_finale != moyennes_doses.pression_finale ||
			temps_adsorption != moyennes_doses.temps_adsorption ||
			temps_volume != moyennes_doses.temps_volume)
		return TRUE;
	return FALSE;
}




Donnees_Grandes_Doses Donnees_Grandes_Doses::operator=(const Donnees_Grandes_Doses &grandes_doses)
{
	a_effectuer = grandes_doses.a_effectuer;
	delta_pression = grandes_doses.delta_pression;
	pression_finale = grandes_doses.pression_finale;
	temps_adsorption = grandes_doses.temps_adsorption;
	temps_volume = grandes_doses.temps_volume;

	return *this;
}

bool Donnees_Grandes_Doses::operator!=(const Donnees_Grandes_Doses &grandes_doses)
{
	if (a_effectuer != grandes_doses.a_effectuer ||
			delta_pression != grandes_doses.delta_pression ||
			pression_finale != grandes_doses.pression_finale ||
			temps_adsorption != grandes_doses.temps_adsorption ||
			temps_volume != grandes_doses.temps_volume)
		return TRUE;
	return FALSE;
}




Donnees_Desorption Donnees_Desorption::operator=(const Donnees_Desorption &desorption)
{
	a_effectuer = desorption.a_effectuer;
	delta_pression = desorption.delta_pression;
	pression_finale = desorption.pression_finale;
	temps_desorption = desorption.temps_desorption;
	temps_volume = desorption.temps_volume;
	derniere_etape = desorption.derniere_etape;

	return *this;
}

bool Donnees_Desorption::operator!=(const Donnees_Desorption &desorption)
{
	if(a_effectuer != desorption.a_effectuer ||
			delta_pression != desorption.delta_pression ||
			pression_finale != desorption.pression_finale ||
			temps_desorption != desorption.temps_desorption ||
			temps_volume != desorption.temps_volume ||
			derniere_etape != desorption.derniere_etape)
		return TRUE;
	return FALSE;
}




Donnees_Adsorption_Continue Donnees_Adsorption_Continue::operator=(const Donnees_Adsorption_Continue &adsorption)
{
	a_effectuer = adsorption.a_effectuer;
	temps_etalonnage_debit = adsorption.temps_etalonnage_debit;
	temps_etalonnage_volume_inter = adsorption.temps_etalonnage_volume_inter;
	temps_equilibre_continue = adsorption.temps_equilibre_continue;
	temps_final_equilibre = adsorption.temps_final_equilibre;
	pression_finale_adsorption_continue = adsorption.pression_finale_adsorption_continue;

	return *this;
}

bool Donnees_Adsorption_Continue::operator!=(const Donnees_Adsorption_Continue &adsorption)
{
	if(a_effectuer != adsorption.a_effectuer ||
			temps_etalonnage_debit != adsorption.temps_etalonnage_debit ||
			temps_etalonnage_volume_inter != adsorption.temps_etalonnage_volume_inter ||
			temps_equilibre_continue != adsorption.temps_equilibre_continue ||
			temps_final_equilibre != adsorption.temps_final_equilibre ||
			pression_finale_adsorption_continue != adsorption.pression_finale_adsorption_continue)
		return TRUE;
	return FALSE;
}