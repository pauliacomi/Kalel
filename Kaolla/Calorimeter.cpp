#include "stdafx.h"
#include "Calorimeter.h"
#include "Parametres.h"
#include "DefineInstruments.h"


CCalorimeter::CCalorimeter()
{
	// Get device settings
	for (size_t i = 0; i < GetNumberInstruments(); i++)
	{
		if (GetTypeInstrument(i) == INSTRUMENT_KEITHLEY)
		{
			switch (GetFonctionInstrument(i))
			{
			case CALO_V1_BP_V2_KEITHLEY:
				chanToRead = INSTRUMENT_KEYTHLEY_V1;	// Store the way to access the device
				index = i;								// Store the index in the settings file
				COM = GetCOMInstrument(i);				// Get the COM port from the settings file
				break;

			case CALO_V1_HP_V2_KEITHLEY:
				chanToRead = INSTRUMENT_KEYTHLEY_V1;	// Store the way to access the device
				index = i;								// Store the index in the settings file
				COM = GetCOMInstrument(i);				// Get the COM port from the settings file
				break;

			case CALO_V1_KEITHLEY:
				chanToRead = INSTRUMENT_KEYTHLEY_V1;	// Store the way to access the device
				index = i;								// Store the index in the settings file
				COM = GetCOMInstrument(i);				// Get the COM port from the settings file
				break;

			default:
				ASSERT(0);								// Should never be reached
				break;
			}
		}
	}

	// Create the device and open the ports
	calorimeterDevice = new Keithley();
	if (calorimeterDevice->OpenCOM(COM))
	{
		calorimeterDevice->InitKeithley();
	}
	else
	{
		// error
	}
}


CCalorimeter::~CCalorimeter()
{
	delete calorimeterDevice;
}

bool CCalorimeter::Read(double * calorimeter)
{
	if (chanToRead == INSTRUMENT_KEYTHLEY_V1) {
		* calorimeter = converter.ConversionCalo(calorimeterDevice->ReadChannel1_double());
		return true;
	}

	if (chanToRead == INSTRUMENT_KEYTHLEY_V2) {
		* calorimeter = converter.ConversionCalo(calorimeterDevice->ReadChannel2_double());
		return true;
	}

	return false;
}


/////------------------- Other functions
//
//double Automation::ReadMeasurementFromDevice(ConnectionMesure Appareil)
//{
//	srand(time(NULL)); // temp
//	double tsp;
//	if (Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V1) {
//		tsp = 1 + ((double)(rand() % 100) / 500);
//		return tsp;
//		return instrument[Appareil.index]->LireKeithley_Voie1();
//	}
//
//	if (Appareil.voie_mesure == INSTRUMENT_KEYTHLEY_V2) {
//		tsp = 0.1 + ((double)(rand() % 100) / 500);
//		return tsp;
//		return instrument[Appareil.index]->LireKeithley_Voie2();
//	}
//
//	if (Appareil.voie_mesure == MENSOR_VOIE) {
//		tsp = 0.1 + ((double)(rand() % 100) / 500);
//		return tsp;
//		//return instrument[Appareil.index]->LireMensor();
//	}
//	return ERROR_MESURE;
//}