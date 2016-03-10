#pragma once

#ifndef MESURE_H
#define MESURE_H

class CMesure
{
public:
	CMesure(void);
	CMesure(int num, double tps, double calorimetre, double bpression, double hpression,
		double temp_calo, double temp_cage, double temp_piece);
	~CMesure(void);

public:
	int numero;
	double temps;
	double calo;
	double basse_pression;
	double haute_pression;
	double temperature_calo;
	double temperature_cage;
	double temperature_piece;
};


typedef CArray<CMesure, CMesure*> CArrayMesure;

#endif