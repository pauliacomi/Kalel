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
	for (size_t i = 0; i < 4; i++)
	{
		CloseHandle(h_MeasurementThread[i]);
		h_MeasurementThread[i] = NULL;
	}
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
	double temp = ReadMeasurementFromDevice(AppareilBP);
	temp = fluxConverter.ConversionCalo(temp);

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
	//g_pTemperature->Temperature(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);

	srand(time(NULL)); // temp

	// Write it in the shared object
	EnterCriticalSection(&criticalSection); 
	experimentLocalData.temperatureCalo = 29 + (rand() % 2);
	experimentLocalData.temperatureCage = 29 + (rand() % 2);;
	experimentLocalData.temperatureRoom = 29 + (rand() % 2);;

	//experimentLocalData.temperatureCalo = (float)dTemperatureCalo;
	//experimentLocalData.temperatureCage = (float)dTemperatureCage;
	//experimentLocalData.temperatureRoom = (float)dTemperaturePiece;
	LeaveCriticalSection(&criticalSection);

}



///------------------- Other functions

double Automation::ReadMeasurementFromDevice(ConnectionMesure Appareil)
{
	srand(time(NULL)); // temp
	double tsp;
	if (Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V1) {
		tsp = 1 + ((double)(rand() % 100) / 500);
		return tsp;
		//return instrument[Appareil.index]->LireKeithley_Voie1();
	}
		
	if (Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V2) {
		tsp = 0.1 + ((double)(rand() % 100) / 500);
		return tsp;
		//return instrument[Appareil.index]->LireKeithley_Voie2();
	}

	if (Appareil.voie_mesure == MENSOR_VOIE) {
		tsp = 0.1 + ((double)(rand() % 100) / 500);
		return tsp;
		//return instrument[Appareil.index]->LireMensor();
	}
	return ERROR_MESURE;
}
