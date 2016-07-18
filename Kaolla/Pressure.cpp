#include "stdafx.h"
#include "Pressure.h"
#include "Parametres.h"
#include "DefineInstruments.h"


CPressure::CPressure()
{
	keith_LowRange = NULL;
	keith_HighRange = NULL;
	mens_LowRange = NULL;
	mens_HighRange = NULL;

	// Get device settings
	for (size_t i = 0; i < GetNumberInstruments(); i++)
	{
		if (GetTypeInstrument(i) == INSTRUMENT_KEITHLEY)
		{
			switch (GetFonctionInstrument(i))
			{
			case CALO_V1_BP_V2_KEITHLEY:
				chanToRead_LowRange = INSTRUMENT_KEYTHLEY_V2;	// Store the way to access the device
				index_LowRange = i;								// Store the index in the settings file
				COM_LowRange = GetCOMInstrument(i);				// Get the COM port from the settings file
				break;

			case CALO_V1_HP_V2_KEITHLEY:
				chanToRead_HighRange = INSTRUMENT_KEYTHLEY_V2;	// Store the way to access the device
				index_HighRange = i;							// Store the index in the settings file
				COM_HighRange = GetCOMInstrument(i);			// Get the COM port from the settings file
				break;

			case INSTRUMENT_KEYTHLEY_LP_V2:
				chanToRead_LowRange = INSTRUMENT_KEYTHLEY_V2;	// Store the way to access the device
				index_LowRange = i;								// Store the index in the settings file
				COM_LowRange = GetCOMInstrument(i);				// Get the COM port from the settings file
				break;

			case INSTRUMENT_KEYTHLEY_HP_V2:
				chanToRead_HighRange = INSTRUMENT_KEYTHLEY_V2;	// Store the way to access the device
				index_HighRange = i;							// Store the index in the settings file
				COM_HighRange = GetCOMInstrument(i);			// Get the COM port from the settings file
				break;

			default:
				ASSERT(0);										// Should never be reached
				break;
			}
		}

		if (GetTypeInstrument(i) == INSTRUMENT_MENSOR)
		{
			if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_LP)
			{
				chanToRead_LowRange = MENSOR_VOIE;				// Store the way to access the device
				index_LowRange = i;								// Store the index in the settings file
				COM_LowRange = GetCOMInstrument(i);				// Get the COM port from the settings file			
			}
			else
			{
				chanToRead_HighRange = MENSOR_VOIE;				// Store the way to access the device
				index_HighRange = i;							// Store the index in the settings file
				COM_HighRange = GetCOMInstrument(i);			// Get the COM port from the settings file			
			}
		}
	}

	// Create the devices and open the ports
	if (chanToRead_LowRange = INSTRUMENT_KEYTHLEY_V2) {
		keith_LowRange = new Keithley();
		if (keith_LowRange->OpenCOM(COM_LowRange))
		{
			keith_LowRange->InitKeithley();
		}
		else
		{
			// error
		}
	}
	else if (chanToRead_LowRange == MENSOR_VOIE)
	{
		mens_LowRange = new Mensor();
		if (mens_LowRange->OpenCOM(COM_LowRange)) {
			// success
		}
	}

	if (chanToRead_HighRange = INSTRUMENT_KEYTHLEY_V2) {
		keith_HighRange = new Keithley();
		if (keith_HighRange->OpenCOM(COM_LowRange))
		{
			keith_HighRange->InitKeithley();
		}
		else
		{
			// error
		}
	}
	else if (chanToRead_HighRange == MENSOR_VOIE)
	{
		mens_HighRange = new Mensor();
		if (mens_HighRange->OpenCOM(COM_LowRange)) {
			// success
		}
	}
	
}


CPressure::~CPressure()
{
	if (keith_LowRange != NULL)
		delete keith_LowRange;
	if(keith_HighRange != NULL)
		delete keith_HighRange;
	if(mens_LowRange != NULL)
		delete mens_LowRange;
	if(mens_HighRange != NULL)
		delete mens_HighRange;
}

bool CPressure::ReadLowRange(double * pressureLowrange)
{
	if (chanToRead_LowRange == INSTRUMENT_KEYTHLEY_V2) {
		*pressureLowrange = converter.ConversionBP(keith_LowRange->ReadChannel2_double());
		return true;
	}

	if (chanToRead_LowRange == MENSOR_VOIE) {
		*pressureLowrange = converter.ConversionBP(mens_LowRange->ReadMensor());
		return true;
	}

	return false;
}

bool CPressure::ReadHighRange(double * pressureHighRange)
{
	if (chanToRead_HighRange == INSTRUMENT_KEYTHLEY_V2) {
		*pressureHighRange = converter.ConversionHP(keith_HighRange->ReadChannel2_double());
		return true;
	}

	if (chanToRead_HighRange == MENSOR_VOIE) {
		*pressureHighRange = converter.ConversionHP(mens_HighRange->ReadMensor());
		return true;
	}

	return false;
}
