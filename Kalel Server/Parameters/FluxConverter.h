#ifndef FLUXCONVERTER_H
#define FLUXCONVERTER_H
#pragma once


class FluxConverter {

public:
	FluxConverter();
	~FluxConverter();

public:
	double ConversionCalo(double flux, double sensitivity_calo);
	double ConversionBP(double flux, double sensitivity_lp);
	double ConversionHP(double flux, double sensitivity_hp);
};

#endif