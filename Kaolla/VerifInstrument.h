#pragma once

#ifndef VERIFINSTRUMENT_H
#define VERIFINSTRUMENT_H

#include "Define.h"

#include "Keithley.h"
#include "Mensor.h"

class VerifInstrument
{
public:
	int COM;
	int type;
	int numero;
};

#endif