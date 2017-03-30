#include "FluxConverter.h"
#include "../../Kalel Shared/Resources/DefineAutomationSettings.h"

FluxConverter::FluxConverter()
{
}

FluxConverter::~FluxConverter()
{
}


double FluxConverter::ConversionCalo(double flux, double sensitivity_calo)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / sensitivity_calo);
}

double FluxConverter::ConversionBP(double flux, double sensitivity_lp)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / sensitivity_lp);
}

double FluxConverter::ConversionHP(double flux, double sensitivity_hp)
{
	if(flux == ERROR_MESURE)
		return flux;
	return (flux / sensitivity_hp);
}
