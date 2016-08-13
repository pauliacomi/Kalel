#include "StdAfx.h"
#include "Classes_experiences.h"

#include <string.h>


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


/// General doses class
Donnees_Doses Donnees_Doses::operator=(const Donnees_Doses &STAGE_DOSES)
{
	delta_pression = STAGE_DOSES.delta_pression;
	pression_finale = STAGE_DOSES.pression_finale;
	temps_adsorption = STAGE_DOSES.temps_adsorption;
	temps_volume = STAGE_DOSES.temps_volume;

	return *this;
}

bool Donnees_Doses::operator!=(const Donnees_Doses &STAGE_DOSES)
{
	if (delta_pression != STAGE_DOSES.delta_pression ||
		pression_finale != STAGE_DOSES.pression_finale ||
		temps_adsorption != STAGE_DOSES.temps_adsorption ||
		temps_volume != STAGE_DOSES.temps_volume)
		return TRUE;
	return FALSE;
}



Donnees_Desorption Donnees_Desorption::operator=(const Donnees_Desorption &STAGE_DESORPTION)
{
	delta_pression = STAGE_DESORPTION.delta_pression;
	pression_finale = STAGE_DESORPTION.pression_finale;
	temps_desorption = STAGE_DESORPTION.temps_desorption;
	temps_volume = STAGE_DESORPTION.temps_volume;

	return *this;
}

bool Donnees_Desorption::operator!=(const Donnees_Desorption &STAGE_DESORPTION)
{
	if(delta_pression != STAGE_DESORPTION.delta_pression ||
			pression_finale != STAGE_DESORPTION.pression_finale ||
			temps_desorption != STAGE_DESORPTION.temps_desorption ||
			temps_volume != STAGE_DESORPTION.temps_volume)
		return TRUE;
	return FALSE;
}




Donnees_Adsorption_Continue Donnees_Adsorption_Continue::operator=(const Donnees_Adsorption_Continue &adsorption)
{
	temps_etalonnage_debit = adsorption.temps_etalonnage_debit;
	temps_etalonnage_volume_inter = adsorption.temps_etalonnage_volume_inter;
	temps_equilibre_continue = adsorption.temps_equilibre_continue;
	temps_final_equilibre = adsorption.temps_final_equilibre;
	pression_finale_adsorption_continue = adsorption.pression_finale_adsorption_continue;

	return *this;
}

bool Donnees_Adsorption_Continue::operator!=(const Donnees_Adsorption_Continue &adsorption)
{
	if(temps_etalonnage_debit != adsorption.temps_etalonnage_debit ||
			temps_etalonnage_volume_inter != adsorption.temps_etalonnage_volume_inter ||
			temps_equilibre_continue != adsorption.temps_equilibre_continue ||
			temps_final_equilibre != adsorption.temps_final_equilibre ||
			pression_finale_adsorption_continue != adsorption.pression_finale_adsorption_continue)
		return TRUE;
	return FALSE;
}