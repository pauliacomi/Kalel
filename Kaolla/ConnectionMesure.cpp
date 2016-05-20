#include "StdAfx.h"
#include "ConnectionMesure.h"


ConnectionMesure::ConnectionMesure()
{
	voie_mesure = -1;
	index = -1;
}

ConnectionMesure::~ConnectionMesure()
{
}

void ConnectionMesure::SetConnectionMesure(int pvoieMesure,int pindex)
{
	this->voie_mesure = pvoieMesure;
	this->index = pindex;
}