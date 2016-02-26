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

void ConnectionMesure::SetConnectionMesure(int voie_mesure,int index)
{
	this->voie_mesure = voie_mesure;
	this->index = index;
}