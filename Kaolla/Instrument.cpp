#include "StdAfx.h"
#include "Instrument.h"


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

void CInstrument::SetParametresInstrument(int COM,int type)
{
	this->COM = COM;
	this->type = type;
}

bool CInstrument::OuvrirPortInstrument()
{
	if(type == APPAREIL_KEITHLEY)
		if(keithley->OpenCOM(COM))
		{
			keithley->InitKeithley();
			return TRUE;
		}
	if(type == APPAREIL_MENSOR)
		return mensor->OpenCOM(COM);
	//else
	return FALSE;
}

bool CInstrument::FermerPortInstrument()
{
	if(type == APPAREIL_KEITHLEY)
		return keithley->CloseCOM();
	if(type == APPAREIL_MENSOR)
		return mensor->CloseCOM();
	//else
	return FALSE;
}
	
	
double CInstrument::LireMensor()
{
	if(type != APPAREIL_MENSOR)
		return -200;
	return mensor->ReadMensor();
}
double CInstrument::LireKeithley_Voie1()
{
	if(type != APPAREIL_KEITHLEY)
		return -200;
	return keithley->ReadChannel1_double();
}
double CInstrument::LireKeithley_Voie2()
{
	if(type != APPAREIL_KEITHLEY)
		return -200;
	return keithley->ReadChannel2_double();
}