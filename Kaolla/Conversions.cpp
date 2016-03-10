#include "StdAfx.h"
#include "Conversions.h"
#include "Define.h"


double ConversionCalo(double flux)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / GetSensibiliteCalo());
}

double ConversionBP(double flux)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / GetSensibiliteCapteurBassePression());
}

double ConversionHP(double flux)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / GetSensibiliteCapteurHautePression());
}
