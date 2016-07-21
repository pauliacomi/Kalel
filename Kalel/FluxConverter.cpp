#include "StdAfx.h"
#include "FluxConverter.h"
#include "DefineAutomationSettings.h"
#include "Parametres.h"

FluxConverter::FluxConverter()
{
}

FluxConverter::~FluxConverter()
{
}


double FluxConverter::ConversionCalo(double flux)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / GetSensibiliteCalo());
}

double FluxConverter::ConversionBP(double flux)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / GetSensibiliteCapteurBassePression());
}

double FluxConverter::ConversionHP(double flux)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / GetSensibiliteCapteurHautePression());
}
