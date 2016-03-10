#pragma once

#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "Define.h"

#include "Keithley.h"
#include "Mensor.h"

class CInstrument
{
public:
	Keithley* keithley;
	Mensor* mensor;
	int COM;
	int type;
	
	CInstrument(void);
	CInstrument(int COM,int type);
	~CInstrument(void);
	void SetParametresInstrument(int COM,int type);
	bool OuvrirPortInstrument();
	bool FermerPortInstrument();
	
	double LireMensor();
	double LireKeithley_Voie1();
	double LireKeithley_Voie2();	
	
};

#endif