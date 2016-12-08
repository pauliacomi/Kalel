#pragma once

#include "../Resources/DefineInstruments.h"			// Definitions for instruments

#include <vector>

#define no_valves	8
#define no_EVs		2
#define no_pumps	1

class ControlInstrumentState {

public:
	ControlInstrumentState();
	~ControlInstrumentState();

	std::vector<bool> valves;
	std::vector<bool> EVs;
	std::vector<bool> pumps;
};


ControlInstrumentState::ControlInstrumentState() {
	for (size_t i = 0; i < no_valves; i++)
	{
		valves.push_back(false);
	}
	for (size_t i = 0; i < no_EVs; i++)
	{
		EVs.push_back(false);
	}
	for (size_t i = 0; i < no_pumps; i++)
	{
		pumps.push_back(false);
	}
}

ControlInstrumentState::~ControlInstrumentState()
{
}