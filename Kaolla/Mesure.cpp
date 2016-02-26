#include "StdAfx.h"
#include "Mesure.h"

CMesure::CMesure(void)
: numero(0)
, temps(0)
, calo(0)
, basse_pression(0)
, haute_pression(0)
, temperature_calo(0)
, temperature_cage(0)
, temperature_piece(0)
{
}

CMesure::CMesure(int num, double tps, double calorimetre, double bpression, double hpression,
		double temp_calo, double temp_cage, double temp_piece)
{
	numero=num;
	temps=tps;
	calo=calorimetre;
	basse_pression=bpression;
	haute_pression=hpression;
	temperature_calo=temp_calo;
	temperature_cage=temp_cage;
	temperature_piece=temp_piece;
}

CMesure::~CMesure(void)
{
}


