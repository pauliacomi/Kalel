#include "stdafx.h"
#include "Calorimeter.h"
#include "Parametres.h"
#include "DefineInstruments.h"


CCalorimeter::CCalorimeter() : Instruments()
{
	calorimeterDevice = NULL;

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
				// No instrument found here
				chanToRead != INSTRUMENT_INEXIST;
				break;
			}
		}
		
		if (GetTypeInstrument(i) == INSTRUMENT_NONE)
		{
			chanToRead = INSTRUMENT_INEXIST;
			i++;
		}
	}

	// Create the device and open the ports
	if (chanToRead != INSTRUMENT_INEXIST)
	{
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
}


CCalorimeter::~CCalorimeter()
{
	if(calorimeterDevice)
		delete calorimeterDevice;
}

bool CCalorimeter::Read(double * calorimeter)
{
	bool success = false;

	if (chanToRead == INSTRUMENT_INEXIST) {
		*calorimeter = -1;
		return true;
	}

	if (chanToRead == INSTRUMENT_KEYTHLEY_V1) {
		success = calorimeterDevice->ReadChannel1(calorimeter);
		*calorimeter = converter.ConversionCalo(*calorimeter);
		return success;
	}

	if (chanToRead == INSTRUMENT_KEYTHLEY_V2) {
		success = calorimeterDevice->ReadChannel2(calorimeter);
		*calorimeter = converter.ConversionCalo(*calorimeter);
		return success;
	}

	return success;
}

void CCalorimeter::GetError(std::string * err) 
{
	calorimeterDevice->GetError(err);
	return;
}