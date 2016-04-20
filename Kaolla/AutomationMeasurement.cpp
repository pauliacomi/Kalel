#include "StdAfx.h"
#include "Automation.h"


void Automation::ThreadMeasurement()
{
	// Start the 4 threads and get their handles
	// The functions wait for the hEvent
	h_MeasurementThread[0] = CreateThread(NULL, NULL, ThreadProc_ReadCalorimeter, this, NULL, NULL);
	h_MeasurementThread[1] = CreateThread(NULL, NULL, ThreadProc_ReadLowPressure, this, NULL, NULL);
	h_MeasurementThread[2] = CreateThread(NULL, NULL, ThreadProc_ReadHighPressure, this, NULL, NULL);
	h_MeasurementThread[3] = CreateThread(NULL, NULL, ThreadProc_ReadTemperature, this, NULL, NULL);

	// Set the event to start the threads
	SetEvent(h_MeasurementThreadStartEvent);

	// Wait for the threads to finish
	WaitForMultipleObjects(4, h_MeasurementThread, TRUE, INFINITE);

	// Clean-up
	h_MeasurementThread[0] = NULL;
	h_MeasurementThread[1] = NULL;
	h_MeasurementThread[2] = NULL;
	h_MeasurementThread[3] = NULL;
	ResetEvent(h_MeasurementThreadStartEvent);
}


///------------------- Threads

DWORD WINAPI Automation::ThreadProc_ReadCalorimeter(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadCalorimeter();

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadHighPressure(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadHighPressure();

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadLowPressure(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadLowPressure();

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadTemperature(LPVOID lpParam)
{
	Automation *manipulation = static_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Execute the function
	manipulation->ReadTemperatures();

	return 0;
}


///------------------- Thread corresponding functions

void Automation::ReadCalorimeter()
{
	// Read the value from the calorimeter
	double temp = fluxConverter.ConversionCalo(ReadMeasurementFromDevice(AppareilCalo));

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData.resultCalorimeter = temp;
	LeaveCriticalSection(&criticalSection);
}


void Automation::ReadLowPressure()
{
	// Read the value from the calorimeter
	double temp = fluxConverter.ConversionCalo(ReadMeasurementFromDevice(AppareilBP));

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData.pressureLow = temp;
	LeaveCriticalSection(&criticalSection);
}


void Automation::ReadHighPressure()
{
	// Read the value from the calorimeter
	double temp = fluxConverter.ConversionCalo(ReadMeasurementFromDevice(AppareilHP));

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentLocalData.pressureHigh = temp;
	LeaveCriticalSection(&criticalSection);
}

void Automation::ReadTemperatures()
{
	// Read the value from the calorimeter
	double dTemperatureCalo, dTemperatureCage, dTemperaturePiece;
	g_pTemperature->Temperature(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);

	// Write it in the shared object
	EnterCriticalSection(&criticalSection);
	experimentData->temperatureCalo = (float)dTemperatureCalo;
	experimentData->temperatureCage = (float)dTemperatureCage;
	experimentData->temperatureRoom = (float)dTemperaturePiece;
	LeaveCriticalSection(&criticalSection);

}



///------------------- Other functions

double Automation::ReadMeasurementFromDevice(ConnectionMesure Appareil)
{
	if (Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V1)
		return instrument[Appareil.index]->LireKeithley_Voie1();
	if (Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V2)
		return instrument[Appareil.index]->LireKeithley_Voie2();
	if (Appareil.voie_mesure == MENSOR_VOIE)
		return instrument[Appareil.index]->LireMensor();
	return ERROR_MESURE;
}
