#ifndef CONNECTIONMESURE_H
#define CONNECTIONMESURE_H
#pragma once


class ConnectionMesure
{
public:
	int voie_mesure;
	int index;

public:
	ConnectionMesure();
	~ConnectionMesure();
	void SetConnectionMesure(int voie_mesure,int index);
};

#endif