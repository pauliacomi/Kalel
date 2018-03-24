#pragma once

#include "../Resources/DefineInstruments.h"			// Definitions for instruments

#include <map>

struct ControlInstrumentStateData
{
	int instrumentID;
	bool instrumentState;
};

typedef std::map<unsigned int, bool> ControlInstrumentState;
