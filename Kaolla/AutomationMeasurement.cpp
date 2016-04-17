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

	// Reset event
	ResetEvent(h_MeasurementThreadStartEvent);
}


void Automation::InstrumentsOpen()
{
	for (int i = 0; i<NB_OF_INSTRUMENTS; i++)
		instrument[i]->OuvrirPortInstrument();
}

void Automation::InstrumentsClose()
{
	for (int i = 0; i<NB_OF_INSTRUMENTS; i++)
		instrument[i]->FermerPortInstrument();
}


DWORD WINAPI Automation::ThreadProc_ReadCalorimeter(LPVOID lpParam)
{
	Automation *manipulation = reinterpret_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Block the corresponding variable
	EnterCriticalSection(&manipulation->criticalSection);
	manipulation->ReadCalorimeter();
	LeaveCriticalSection(&manipulation->criticalSection);

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadHighPressure(LPVOID lpParam)
{
	Automation *manipulation = reinterpret_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Block the corresponding variable
	EnterCriticalSection(&manipulation->criticalSection);
	manipulation->ReadHighPressure();
	LeaveCriticalSection(&manipulation->criticalSection);

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadLowPressure(LPVOID lpParam)
{
	Automation *manipulation = reinterpret_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Read the intstrument


	// Block the corresponding variable
	EnterCriticalSection(&manipulation->criticalSection);
	manipulation->ReadLowPressure();
	LeaveCriticalSection(&manipulation->criticalSection);

	return 0;
}

DWORD WINAPI Automation::ThreadProc_ReadTemperature(LPVOID lpParam)
{
	Automation *manipulation = reinterpret_cast<Automation *>(lpParam);

	// Wait for the activation of the hEvent
	WaitForSingleObject(manipulation->h_MeasurementThreadStartEvent, INFINITE);

	// Block the corresponding variable
	EnterCriticalSection(&manipulation->criticalSection);
	manipulation->ReadTemperatures();
	LeaveCriticalSection(&manipulation->criticalSection);

	return 0;
}



void Automation::ReadCalorimeter()
{
	experimentLocalData.resultCalorimeter = fluxConverter.ConversionCalo(ReadMeasurementFromDevice(AppareilCalo));
}


void Automation::ReadLowPressure()
{
	experimentLocalData.pressureLow = fluxConverter.ConversionBP(ReadMeasurementFromDevice(AppareilBP));
}


void Automation::ReadHighPressure()
{
	experimentLocalData.pressureHigh = fluxConverter.ConversionHP(ReadMeasurementFromDevice(AppareilHP));
	SecuriteHautePression();
}

void Automation::ReadTemperatures()
{
	double dTemperatureCalo, dTemperatureCage, dTemperaturePiece;
	g_pTemperature->Temperature(&dTemperatureCalo, &dTemperatureCage, &dTemperaturePiece);
	experimentData->temperatureCalo = (float)dTemperatureCalo;
	experimentData->temperatureCage = (float)dTemperatureCage;
	experimentData->temperatureRoom = (float)dTemperaturePiece;

	SecuriteTemperatures();
}

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