#pragma once

#include <atomic>
#include "../classHelpers.h"


struct ExperimentAtomicData
{
	atomic_time_point tp;

	///*******************
	///		Recorded Data
	///*******************

	// Calorimeter
	std::atomic<double> resultCalorimeter = 0.f;						// Calorimeter result

	// Pressure
	std::atomic<double> pressureHigh = 0.f;								// Pressure recorded from the high PT
	std::atomic<double> pressureLow = 0.f;								// Pressure recorded from the low PT
	
	// Temperature
	std::atomic<double> temperatureCalo = 0.f;							// Temperature recorded by the Calorimeter
	std::atomic<double> temperatureCage = 0.f;							// Temperature recorded in the enclosure
	std::atomic<double> temperatureRoom = 0.f;							// Temperature recorded in the room
};

struct ExperimentData 
{
	///*******************
	///		Recorded Data - but not atomic
	///*******************
	inline ExperimentData() {};

	inline ExperimentData(const ExperimentAtomicData & atomicData) {
		resultCalorimeter	= atomicData.resultCalorimeter;
		pressureHigh		= atomicData.pressureHigh;
		pressureLow			= atomicData.pressureLow;
		temperatureCalo		= atomicData.temperatureCalo;
		temperatureRoom		= atomicData.temperatureRoom;
		temperatureCage		= atomicData.temperatureCage;
	}

	double resultCalorimeter = 0.f;							// Calorimeter result
	double pressureHigh = 0.f;								// Pressure recorded from the high PT
	double pressureLow = 0.f;								// Pressure recorded from the low PT
	double temperatureCalo = 0.f;							// Temperature recorded by the Calorimeter
	double temperatureCage = 0.f;							// Temperature recorded in the enclosure
	double temperatureRoom = 0.f;							// Temperature recorded in the room
};