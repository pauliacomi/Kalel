#include "stdafx.h"
#include "SerialInstruments.h"
#include "Parametres.h"
#include "DefineInstruments.h"


SerialInstruments::SerialInstruments()
	:keithley(NULL)
	,mens_HighRange(NULL)
	,mens_LowRange(NULL)
	,keithleyInitiated(false)
{
	// This loop goes through all the instruments in the Parameters file
	// Depending on the type of instruments it will output parameters for
	//
	// * Calorimeter Apparatus
	// * Low Range Pressure Apparatus
	// * High Range Pressure Apparatus
	//
	// The parameters for each are 
	//
	// * Type:			if using a keithley or a mensor
	// * COM port		the COM port that the instrument is connected to
	// * Channel:		the channel that has to be read (only makes sense for a keithley)
	//

	calorimeter = INSTRUMENT_INEXIST;
	pressureLowRange = INSTRUMENT_INEXIST;
	pressureHighRange = INSTRUMENT_INEXIST;

	for (int i = 1; i <= GetNumberInstruments(); i++)
	{
		switch (GetTypeInstrument(i))
		{
		case INSTRUMENT_KEITHLEY:

			switch (GetFonctionInstrument(i))
			{
			case CALO_V1_BP_V2_KEITHLEY:

				calorimeter = INSTRUMENT_KEITHLEY;
				calorimeterChannel = INSTRUMENT_KEYTHLEY_V1;
				calorimeterCOM = GetCOMInstrument(i);

				pressureLowRange = INSTRUMENT_KEITHLEY;
				pressureLowRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureLowRangeCOM = GetCOMInstrument(i);

				break;

			case CALO_V1_HP_V2_KEITHLEY:

				calorimeter = INSTRUMENT_KEITHLEY;
				calorimeterChannel = INSTRUMENT_KEYTHLEY_V1;
				calorimeterCOM = GetCOMInstrument(i);

				pressureHighRange = INSTRUMENT_KEITHLEY;
				pressureHighRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureHighRangeCOM = GetCOMInstrument(i);

				break;

			case CALO_V1_KEITHLEY:

				calorimeter = INSTRUMENT_KEITHLEY;
				calorimeterChannel = INSTRUMENT_KEYTHLEY_V1;
				calorimeterCOM = GetCOMInstrument(i);

				break;

			case INSTRUMENT_KEYTHLEY_LP_V2:

				pressureLowRange = INSTRUMENT_KEITHLEY;
				pressureLowRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureLowRangeCOM = GetCOMInstrument(i);

				break;

			case INSTRUMENT_KEYTHLEY_HP_V2:

				pressureHighRange = INSTRUMENT_KEITHLEY;
				pressureHighRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureHighRangeCOM = GetCOMInstrument(i);

				break;

			default:
				ASSERT(0); // Should never be reached
				break;
			}
			break;


		case INSTRUMENT_MENSOR:
			if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_LP)
			{
				pressureLowRange = INSTRUMENT_MENSOR;
				pressureLowRangeChannel = MENSOR_VOIE;
				pressureLowRangeCOM = GetCOMInstrument(i);
			}
			else if (GetFonctionInstrument(i) == INSTRUMENT_MENSOR_HP)
			{
				pressureHighRange = INSTRUMENT_MENSOR;
				pressureHighRangeChannel = MENSOR_VOIE;
				pressureHighRangeCOM = GetCOMInstrument(i);
			}

			break;
		case INSTRUMENT_NONE:
		case INSTRUMENT_UNDEF:
		case INSTRUMENT_INEXIST:
			// If there no instrument set, don't do anything
			break;
		default:
			// Should never be reached
			ASSERT(0);
			break;
		}
	}

	// Now the init function can be called
}


SerialInstruments::~SerialInstruments()
{
	if (keithley != NULL)
		delete keithley;
	if (mens_LowRange != NULL)
		delete mens_LowRange;
	if (mens_HighRange != NULL)
		delete mens_HighRange;
}



//
// initiation functions
//

bool SerialInstruments::Init()
{
	// Now that we have all the data we can call the initiator functions
	if (!InitiateCalorimeter());
		//return false;
	if (!InitiatePressureLowRange());
		//return false;
	if (!InitiatePressureHighRange());
		//return false;
	
	return true;
}

bool SerialInstruments::InitiateCalorimeter()
{
	if (calorimeter != INSTRUMENT_INEXIST)
	{
		if (calorimeter == INSTRUMENT_KEITHLEY && keithleyInitiated == false)
		{
			keithleyInitiated = true;

			keithley = new Keithley();

			if (keithley->OpenCOM(calorimeterCOM))
			{
				keithley->InitKeithley();
				return true;
			}
			else
				return false;
		}
	}
	return true;
}


bool SerialInstruments::InitiatePressureLowRange()
{
	if (pressureLowRange != INSTRUMENT_INEXIST)
	{
		if (pressureLowRange == INSTRUMENT_KEITHLEY && keithleyInitiated == false)
		{
			keithleyInitiated = true;

			keithley = new Keithley();

			if (keithley->OpenCOM(pressureLowRangeCOM))
			{
				keithley->InitKeithley();
				return true;
			}
			else
				return false;
		}

		if (pressureLowRange == INSTRUMENT_MENSOR)
		{
			mens_LowRange = new Mensor();

			if (mens_LowRange->OpenCOM(pressureLowRangeCOM))
				return true;
			else
				return false;
		}
	}
	return true;
}

bool SerialInstruments::InitiatePressureHighRange()
{
	if (pressureHighRange != INSTRUMENT_INEXIST)
	{
		if (pressureHighRange == INSTRUMENT_KEITHLEY && keithleyInitiated == false)
		{
			keithleyInitiated = true;

			keithley = new Keithley();

			if (keithley->OpenCOM(pressureHighRangeCOM))
			{
				keithley->InitKeithley();
				return true;
			}
			else
				return false;
		}

		if (pressureHighRange == INSTRUMENT_MENSOR)
		{
			mens_HighRange = new Mensor();

			if (mens_HighRange->OpenCOM(pressureHighRangeCOM))
				return true;
			else
				return false;
		}
	}
	return true;
}




//
// read functions
//

bool SerialInstruments::ReadCalorimeter(double * result)
{
	bool success = false;

	if (calorimeter == INSTRUMENT_INEXIST) {
		*result = -1;
		return true;
	}

	if (calorimeter == INSTRUMENT_KEITHLEY) {
		if (calorimeterChannel == INSTRUMENT_KEYTHLEY_V1) {
			success = keithley->ReadChannel1(result);
			*result = converter.ConversionCalo(*result);
			return success;
		}
		if (calorimeterChannel == INSTRUMENT_KEYTHLEY_V2) {
			success = keithley->ReadChannel2(result);
			*result = converter.ConversionCalo(*result);
			return success;
		}
	}	

	return success;
}


bool SerialInstruments::ReadPressureLowRange(double * result)
{
	bool success = false;

	if (pressureLowRange == INSTRUMENT_INEXIST) {
		*result = -1;
		return true;
	}

	if (pressureLowRange == INSTRUMENT_KEITHLEY) {

		if (pressureLowRangeChannel == INSTRUMENT_KEYTHLEY_V1) {
			success = keithley->ReadChannel1(result);
			*result = converter.ConversionBP(*result);
			return success;
		}

		if (pressureLowRangeChannel == INSTRUMENT_KEYTHLEY_V2) {
			success = keithley->ReadChannel2(result);
			*result = converter.ConversionBP(*result);
			return success;
		}

	}
	
	if (pressureLowRange == INSTRUMENT_MENSOR) {
		success = mens_LowRange->ReadMensor(result);
		*result = converter.ConversionBP(*result);
		return success;
	}

	return success;
}

bool SerialInstruments::ReadPressureHighRange(double * result)
{
	bool success = false;

	if (pressureHighRange == INSTRUMENT_INEXIST) {
		*result = -1;
		return true;
	}

	if (pressureHighRange == INSTRUMENT_KEITHLEY) {

		if (pressureHighRangeChannel == INSTRUMENT_KEYTHLEY_V1) {
			success = keithley->ReadChannel1(result);
			*result = converter.ConversionHP(*result);
			return success;
		}

		if (pressureHighRangeChannel == INSTRUMENT_KEYTHLEY_V2) {
			success = keithley->ReadChannel2(result);
			*result = converter.ConversionHP(*result);
			return success;
		}

	}

	if (pressureHighRange == INSTRUMENT_MENSOR) {
		success = mens_HighRange->ReadMensor(result);
		*result = converter.ConversionHP(*result);
		return success;
	}

	return success;
}

//
// error functions
//

void SerialInstruments::GetErrorCalrimeter(std::string * err)
{
	keithley->GetError(err);
	return;
}

void SerialInstruments::GetErrorLowRange(std::string * err)
{
	if (pressureLowRange != INSTRUMENT_INEXIST) {

		if (pressureLowRange == INSTRUMENT_KEITHLEY) {
			keithley->GetError(err);
			return;
		}

		if (pressureLowRange == INSTRUMENT_MENSOR) {
			mens_LowRange->GetError(err);
			return;
		}
	}
}

void SerialInstruments::GetErrorHighRange(std::string * err)
{
	if (pressureHighRange != INSTRUMENT_INEXIST) {

		if (pressureHighRange == INSTRUMENT_KEITHLEY) {
			keithley->GetError(err);
			return;
		}

		if (pressureHighRange == INSTRUMENT_MENSOR) {
			mens_HighRange->GetError(err);
			return;
		}
	}
}
