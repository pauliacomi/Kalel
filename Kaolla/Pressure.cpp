#include "stdafx.h"
#include "Pressure.h"
#include "Parametres.h"
#include "DefineInstruments.h"


CPressure::CPressure()
	: Instruments()
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
				// No instrument found here
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
			if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_HP)
			{
				chanToRead_HighRange = MENSOR_VOIE;				// Store the way to access the device
				index_HighRange = i;							// Store the index in the settings file
				COM_HighRange = GetCOMInstrument(i);			// Get the COM port from the settings file			
			}
		}

		if (GetTypeInstrument(i) == INSTRUMENT_NONE)
		{
			chanToRead_LowRange = INSTRUMENT_INEXIST;
			i++;
		}

	}


	// Create the devices and open the ports
	if (chanToRead_LowRange != INSTRUMENT_INEXIST)
	{
		if (chanToRead_LowRange = INSTRUMENT_KEYTHLEY_V2) {
			keith_LowRange = new Keithley();
			
			// no initialisation, because it will be already done in the calorimeter class 
		}
		else if (chanToRead_LowRange == MENSOR_VOIE)
		{
			mens_LowRange = new Mensor();
			if (mens_LowRange->OpenCOM(COM_LowRange)) {
				// success
			}
		}
	}
	
	if (chanToRead_HighRange != INSTRUMENT_INEXIST)
	{
		if (chanToRead_HighRange = INSTRUMENT_KEYTHLEY_V2) {
			keith_HighRange = new Keithley();

			// no initialisation, because it will be already done in the calorimeter class 
		}
		else if (chanToRead_HighRange == MENSOR_VOIE)
		{
			mens_HighRange = new Mensor();
			if (mens_HighRange->OpenCOM(COM_LowRange)) {
				// success
			}
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
	bool success = false;

	if (chanToRead_LowRange == INSTRUMENT_INEXIST) {
		*pressureLowrange = -1;
		return true;
	}

	if (chanToRead_LowRange == INSTRUMENT_KEYTHLEY_V2) {
		success = keith_LowRange->ReadChannel2(pressureLowrange);
		*pressureLowrange = converter.ConversionBP(*pressureLowrange);
		return success;
	}

	if (chanToRead_LowRange == MENSOR_VOIE) {
		success = mens_LowRange->ReadMensor(pressureLowrange);
		*pressureLowrange = converter.ConversionBP(*pressureLowrange);
		return success;
	}

	return success;
}

bool CPressure::ReadHighRange(double * pressureHighRange)
{
	bool success = false;

	if (chanToRead_HighRange == INSTRUMENT_INEXIST) {
		*pressureHighRange = -1;
		return true;
	}

	if (chanToRead_HighRange == INSTRUMENT_KEYTHLEY_V2) {
		success = keith_HighRange->ReadChannel2(pressureHighRange);
		*pressureHighRange = converter.ConversionHP(*pressureHighRange);
		return success;
	}

	if (chanToRead_HighRange == MENSOR_VOIE) {
		success = mens_HighRange->ReadMensor(pressureHighRange);
		*pressureHighRange = converter.ConversionHP(*pressureHighRange);
		return success;
	}

	return success;
}

void CPressure::GetErrorLowRange(std::string * err) {

	if (chanToRead_LowRange == INSTRUMENT_KEYTHLEY_V2) {
		keith_LowRange->GetError(err);
		return;
	}

	if (chanToRead_LowRange == MENSOR_VOIE) {
		mens_LowRange->GetError(err);
		return;
	}
}

void CPressure::GetErrorHighRange(std::string * err) {

	if (chanToRead_HighRange == INSTRUMENT_KEYTHLEY_V2) {
		keith_HighRange->GetError(err);
		return;
	}

	if (chanToRead_HighRange == MENSOR_VOIE) {
		mens_HighRange->GetError(err);
		return;
	}
}

//
//void CPressure::GetSerialPorts()
//{
//	int chanToRead_Calorimeter;		// Store the way to access the device
//	int chanToRead_LowRange;		// Store the way to access the device
//	int chanToRead_HighRange;		// Store the way to access the device
//	int COM_Calorimeter;
//	int COM_LowRange;
//	int COM_HighRange;
//
//	for (int i = 1; i <= GetNumberInstruments(); i++)
//	{
//		switch (GetTypeInstrument(i))
//		{
//		case INSTRUMENT_KEITHLEY:
//
//			switch (GetFonctionInstrument(i))
//			{
//			case CALO_V1_BP_V2_KEITHLEY:
//
//				chanToRead_Calorimeter = INSTRUMENT_KEYTHLEY_V1;
//				COM_Calorimeter = GetCOMInstrument(i);
//
//				chanToRead_LowRange = INSTRUMENT_KEYTHLEY_V2;
//				COM_LowRange = GetCOMInstrument(i);
//
//				break;
//
//			case CALO_V1_HP_V2_KEITHLEY:
//
//				chanToRead_Calorimeter = INSTRUMENT_KEYTHLEY_V1;
//				COM_Calorimeter = GetCOMInstrument(i);
//
//				chanToRead_HighRange = INSTRUMENT_KEYTHLEY_V2;
//				COM_HighRange = GetCOMInstrument(i);
//
//				break;
//
//			case CALO_V1_KEITHLEY:
//
//				chanToRead_Calorimeter = INSTRUMENT_KEYTHLEY_V1;
//				COM_Calorimeter = GetCOMInstrument(i);
//
//				break;
//
//			case INSTRUMENT_KEYTHLEY_LP_V2:
//
//				chanToRead_LowRange = INSTRUMENT_KEYTHLEY_V2;
//				COM_LowRange = GetCOMInstrument(i);
//
//				break;
//
//			case INSTRUMENT_KEYTHLEY_HP_V2:
//
//				chanToRead_HighRange = INSTRUMENT_KEYTHLEY_V2;
//				COM_HighRange = GetCOMInstrument(i);
//
//				break;
//
//			default:
//				ASSERT(0); // Should never be reached
//				break;
//			}
//			break;
//
//
//		case INSTRUMENT_MENSOR:
//			if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_LP)
//			{
//				chanToRead_LowRange = MENSOR_VOIE;
//				COM_LowRange = GetCOMInstrument(i);
//			}
//			else if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_HP)
//			{
//				chanToRead_HighRange = MENSOR_VOIE;
//				COM_HighRange = GetCOMInstrument(i);
//			}
//
//			break;
//		case INSTRUMENT_NONE:
//		case INSTRUMENT_UNDEF:
//		case INSTRUMENT_INEXIST:
//		default:
//			ASSERT(0); // Should never be reached
//			break;
//		}
//	}
//}