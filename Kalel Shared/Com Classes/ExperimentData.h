#pragma once

#include <atomic>
#include <memory>		// for the typedef
#include <chrono>		// for the typedef
#include <map>			// for the typedef


class ExperimentData
{
public:
	ExperimentData();
	ExperimentData(const ExperimentData & p);
	~ExperimentData();

	///*******************
	///		Recorded Data
	///*******************

	// Calorimeter
	std::atomic<double> resultCalorimeter = 0.f;

	// Pressure
	std::atomic<double> pressureHigh = 0.f;								// Pressure recorded from the high PT
	std::atomic<double> pressureLow = 0.f;								// Pressure recorded from the low PT
	
	// Temperature
	std::atomic<double> temperatureCalo = 0.f;							// Temperature recorded by the Calorimeter
	std::atomic<double> temperatureCage = 0.f;							// Temperature recorded in the enclosure
	std::atomic<double> temperatureRoom = 0.f;							// Temperature recorded in the room
	


	///******************************************************************************************************************
	///
	///
	///
	///		Functions
	///
	///
	///******************************************************************************************************************


	double GetresultCalorimeter()			const;					// Signal recorded from the calorimeter
	double GetpressureHigh()				const;					// Pressure recorded from the high PT
	double GetpressureLow()					const;					// Pressure recorded from the low PT
	double GettemperatureCalo()				const;					// Temperature recorded by the Calorimeter
	double GettemperatureCage()				const;					// Temperature recorded in the enclosure
	double GettemperatureRoom()				const;					// Temperature recorded in the room
	
	void SetresultCalorimeter(double a)							;
	void SetpressureHigh(double a)								;
	void SetpressureLow(double a)								;
	void SettemperatureCalo(double a)							;
	void SettemperatureCage(double a)							;
	void SettemperatureRoom(double a)							;

};