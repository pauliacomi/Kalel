#include "StdAfx.h"
#include "Instrument.h"

#include "DefineInstruments.h"


CInstrument::CInstrument(void)
{
	keithley = new Keithley();
	mensor = new Mensor();
	COM = -1;
	type = -1;
}

CInstrument::CInstrument(int COM,int type)
{
	keithley = new Keithley();
	mensor = new Mensor();
	this->COM = COM;
	this->type = type;
}

CInstrument::~CInstrument(void)
{
	delete keithley;
	delete mensor;
}

void CInstrument::SetParametresInstrument(int pCOM,int pType)
{
	this->COM = pCOM;
	this->type = pType;
}

bool CInstrument::OuvrirPortInstrument()
{
	if(type == INSTRUMENT_KEITHLEY)
		if(keithley->OpenCOM(COM))
		{
			keithley->InitKeithley();
			return TRUE;
		}
	if(type == INSTRUMENT_MENSOR)
		return mensor->OpenCOM(COM);
	//else
	return FALSE;
}

bool CInstrument::FermerPortInstrument()
{
	if(type == INSTRUMENT_KEITHLEY)
		return keithley->CloseCOM();
	if(type == INSTRUMENT_MENSOR)
		return mensor->CloseCOM();
	//else
	return FALSE;
}
	
	
double CInstrument::LireMensor()
{
	if(type != INSTRUMENT_MENSOR)
		return -200;
	return mensor->ReadMensor();
}
double CInstrument::LireKeithley_Voie1()
{
	if(type != INSTRUMENT_KEITHLEY)
		return -200;
	return keithley->ReadChannel1_double();
}
double CInstrument::LireKeithley_Voie2()
{
	if(type != INSTRUMENT_KEITHLEY)
		return -200;
	return keithley->ReadChannel2_double();
}