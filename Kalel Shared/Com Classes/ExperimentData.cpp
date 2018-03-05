#include "../Forcelib.h"
#include "ExperimentData.h"

ExperimentData::ExperimentData()
{
}

ExperimentData::~ExperimentData()
{
}

ExperimentData::ExperimentData(const ExperimentData & p)
{
	///*******************
	///		Recorded Data
	///*******************

	resultCalorimeter				= p.GetresultCalorimeter();

	pressureHigh					= p.GetpressureHigh();
	pressureLow						= p.GetpressureLow();

	temperatureCalo					= p.GettemperatureCalo();
	temperatureCage					= p.GettemperatureCage();
	temperatureRoom					= p.GettemperatureRoom();
}


double ExperimentData::GetresultCalorimeter()							const { return resultCalorimeter					; }
double ExperimentData::GetpressureHigh()								const { return pressureHigh							; }
double ExperimentData::GetpressureLow()									const { return pressureLow							; }
double ExperimentData::GettemperatureCalo()								const { return temperatureCalo						; }
double ExperimentData::GettemperatureCage()								const { return temperatureCage						; }
double ExperimentData::GettemperatureRoom()								const { return temperatureRoom						; }


void ExperimentData::SetresultCalorimeter(double a)							{ resultCalorimeter				= a ; }
void ExperimentData::SetpressureHigh(double a)								{ pressureHigh					= a ; }
void ExperimentData::SetpressureLow(double a)								{ pressureLow					= a ; }
void ExperimentData::SettemperatureCalo(double a)							{ temperatureCalo				= a ; }
void ExperimentData::SettemperatureCage(double a)							{ temperatureCage				= a ; }
void ExperimentData::SettemperatureRoom(double a)							{ temperatureRoom				= a ; }