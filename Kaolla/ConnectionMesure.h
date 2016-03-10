#pragma once

#ifndef CONNECTIONMESURE_H
#define CONNECTIONMESURE_H

#include "Define.h"

class ConnectionMesure
{
public:
	int voie_mesure;
	int index;

	ConnectionMesure(void);
	~ConnectionMesure(void);
	void SetConnectionMesure(int voie_mesure,int index);
};

#endif