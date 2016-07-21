#pragma once

#ifndef FLUXCONVERTER_H
#define FLUXCONVERTER_H

class FluxConverter {

public:
	FluxConverter();
	~FluxConverter();

public:
	double ConversionCalo(double flux);
	double ConversionBP(double flux);
	double ConversionHP(double flux);
};

#endif