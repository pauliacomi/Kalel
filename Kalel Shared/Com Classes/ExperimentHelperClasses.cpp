#include "../Forcelib.h"
#include "ExperimentHelperClasses.h"

user user::operator=(const user &exp)
{
	if (this != &exp)
	{
		nom = exp.nom;
		surnom = exp.surnom;
	}
	return *this;
}

bool user::operator!=(const user &exp)
{
	if (nom != exp.nom || surnom != exp.surnom)
		return true;

	return false;
}



gas gas::operator=(const gas &g)
{
	if (this != &g)
	{
		nom = g.nom;
		symbole = g.symbole;
		masse_moleculaire = g.masse_moleculaire;
		temperature_critique = g.temperature_critique;
		pression_critique = g.pression_critique;
		temperature_ebullition = g.temperature_ebullition;
		omega = g.omega;
	}
	return *this;
}

bool gas::operator!=(const gas &g)
{
	if (nom != g.nom || symbole != g.symbole 
			|| masse_moleculaire != g.masse_moleculaire
			|| temperature_critique != g.temperature_critique
			|| pression_critique != g.pression_critique
			|| temperature_ebullition != g.temperature_ebullition
			|| omega != g.omega)
		return true;
	return false;
}




cell cell::operator=(const cell &cell)
{
	if (this != &cell)
	{
		numero = cell.numero;
		volume_total = cell.volume_total;
		volume_calo = cell.volume_calo;
	}
	return *this;
}

bool cell::operator!=(const cell &cell)
{
	if (numero != cell.numero || 
			volume_total != cell.volume_total ||
			volume_calo != cell.volume_calo)
		return true;
	return false;
}



data_general data_general::operator=(const data_general &general)
{
	if (this != &general)
	{
		user = general.user;
		gas = general.gas;
		nom_echantillon = general.nom_echantillon;
		masse_echantillon = general.masse_echantillon;
		commentaires = general.commentaires;
		chemin = general.chemin;
		fichier = general.fichier;
		temperature_experience = general.temperature_experience;
		temperature_range_initial_check = general.temperature_range_initial_check;
		date_experience = general.date_experience;
	}
	return *this;
}

bool data_general::operator!=(const data_general &general)
{
	if (user != general.user ||
			gas != general.gas ||
			nom_echantillon != general.nom_echantillon ||
			masse_echantillon != general.masse_echantillon ||
			commentaires != general.commentaires ||
			chemin != general.chemin ||
			fichier != general.fichier ||
			temperature_experience != general.temperature_experience ||
			temperature_range_initial_check != general.temperature_range_initial_check ||
			date_experience != general.date_experience)
		return true;

	return false;
}




data_other data_other::operator=(const data_other &divers)
{
	if (this != &divers)
	{
		cell = divers.cell;
		time_baseline = divers.time_baseline;
		mise_sous_vide_fin_experience = divers.mise_sous_vide_fin_experience;
		temps_vide = divers.temps_vide;
	}
	return *this;
}

bool data_other::operator!=(const data_other &divers)
{
	if (cell != divers.cell ||
			time_baseline != divers.time_baseline ||
			mise_sous_vide_fin_experience != divers.mise_sous_vide_fin_experience ||
			temps_vide != divers.temps_vide)
		return true;
	return false;
}


/// General doses class
data_adsorption data_adsorption::operator=(const data_adsorption &STAGE_DOSES)
{
	if (this != &STAGE_DOSES)
	{
		delta_pression = STAGE_DOSES.delta_pression;
		pression_finale = STAGE_DOSES.pression_finale;
		temps_adsorption = STAGE_DOSES.temps_adsorption;
		temps_volume = STAGE_DOSES.temps_volume;
	}
	return *this;
}

bool data_adsorption::operator!=(const data_adsorption &STAGE_DOSES)
{
	if (delta_pression != STAGE_DOSES.delta_pression ||
		pression_finale != STAGE_DOSES.pression_finale ||
		temps_adsorption != STAGE_DOSES.temps_adsorption ||
		temps_volume != STAGE_DOSES.temps_volume)
		return true;
	return false;
}

bool data_adsorption::operator!=(const data_adsorption & STAGE_DOSES) const
{
	if (delta_pression != STAGE_DOSES.delta_pression ||
		pression_finale != STAGE_DOSES.pression_finale ||
		temps_adsorption != STAGE_DOSES.temps_adsorption ||
		temps_volume != STAGE_DOSES.temps_volume)
		return true;
	return false;
}



data_desorption data_desorption::operator=(const data_desorption &STAGE_DESORPTION)
{
	if (this != &STAGE_DESORPTION)
	{
		delta_pression = STAGE_DESORPTION.delta_pression;
		pression_finale = STAGE_DESORPTION.pression_finale;
		temps_desorption = STAGE_DESORPTION.temps_desorption;
		temps_volume = STAGE_DESORPTION.temps_volume;
	}
	return *this;
}

bool data_desorption::operator!=(const data_desorption &STAGE_DESORPTION)
{
	if(delta_pression != STAGE_DESORPTION.delta_pression ||
			pression_finale != STAGE_DESORPTION.pression_finale ||
			temps_desorption != STAGE_DESORPTION.temps_desorption ||
			temps_volume != STAGE_DESORPTION.temps_volume)
		return true;
	return false;
}

bool data_desorption::operator!=(const data_desorption & STAGE_DESORPTION) const
{
	if (delta_pression != STAGE_DESORPTION.delta_pression ||
		pression_finale != STAGE_DESORPTION.pression_finale ||
		temps_desorption != STAGE_DESORPTION.temps_desorption ||
		temps_volume != STAGE_DESORPTION.temps_volume)
		return true;
	return false;
}


data_continuous data_continuous::operator=(const data_continuous &data)
{
	if (this != &data)
	{
		temps_etalonnage_debit = data.temps_etalonnage_debit;
		temps_etalonnage_volume_inter = data.temps_etalonnage_volume_inter;
		temps_equilibre_continue = data.temps_equilibre_continue;
		temps_final_equilibre = data.temps_final_equilibre;
		pression_finale_adsorption_continue = data.pression_finale_adsorption_continue;
	}

	return *this;
}

bool data_continuous::operator!=(const data_continuous &adsorption)
{
	if(temps_etalonnage_debit != adsorption.temps_etalonnage_debit ||
			temps_etalonnage_volume_inter != adsorption.temps_etalonnage_volume_inter ||
			temps_equilibre_continue != adsorption.temps_equilibre_continue ||
			temps_final_equilibre != adsorption.temps_final_equilibre ||
			pression_finale_adsorption_continue != adsorption.pression_finale_adsorption_continue)
		return true;
	return false;
}

Instrument Instrument::operator=(const Instrument & i)
{
	if (this != &i)
	{
		type = i.type;
		port = i.port;
	}

	return *this;    // Return ref for multiple assignment
}

bool Instrument::operator==(const Instrument & i)
{
	if (type == i.type && port == i.port)
		return true;
	return false;
}

bool Instrument::operator==(const Instrument & i) const
{
	if (type == i.type && port == i.port)
		return true;
	return false;
}

bool Reader::operator==(const Reader & r)
{
	if (type == r.type && identifier == r.identifier && sensitivity == r.sensitivity 
		&& channel == r.channel && instrument == r.instrument)
		return true;
	return false;
}

Controller Controller::operator=(const Controller & c)
{
	if (this != &c)
	{
		type = c.type;								// Valve, Pump, etc
		identifier = c.identifier;					// Number 1/2
		instrument = c.instrument;					// Key of the associated instrument
		readerfunction = c.readerfunction;
		controllerfunction = c.controllerfunction;
	}

	return *this;    // Return ref for multiple assignment
}

bool Controller::operator==(const Controller & c)
{
	if (type == c.type && identifier == c.identifier && instrument == c.instrument)
		return true;
	return false;
}
