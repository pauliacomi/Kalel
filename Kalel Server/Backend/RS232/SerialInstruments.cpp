#include "SerialInstruments.h"

#include "../../MessageHandler.h"
#include "../RS232/Keithley.h"
#include "../RS232/Mensor.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"
#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../../Kalel Shared/Resources/DefineText.h"


SerialInstruments::SerialInstruments(MessageHandler & messageHandler, MachineSettings & m)
	: messageHandler{ messageHandler }
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

	calorimeter			= INSTRUMENT_INEXIST;
	pressureLowRange	= INSTRUMENT_INEXIST;
	pressureHighRange	= INSTRUMENT_INEXIST;

	this->sensitivity_calo = m.SensibiliteCalo;
	this->sensitivity_lp = m.SensibiliteCapteurBassePression;
	this->sensitivity_hp = m.SensibiliteCapteurHautePression;

	for (int i = 0; i < m.NumberInstruments; i++)
	{
		switch (m.typeInstruments[i])
		{
		case INSTRUMENT_KEITHLEY:

			switch (m.FunctionInstruments[i])
			{
			case CALO_V1_BP_V2_KEITHLEY:

				calorimeter = INSTRUMENT_KEITHLEY;
				calorimeterChannel = INSTRUMENT_KEYTHLEY_V1;
				calorimeterCOM = m.COMInstruments[i];

				pressureLowRange = INSTRUMENT_KEITHLEY;
				pressureLowRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureLowRangeCOM = m.COMInstruments[i];

				break;

			case CALO_V1_HP_V2_KEITHLEY:

				calorimeter = INSTRUMENT_KEITHLEY;
				calorimeterChannel = INSTRUMENT_KEYTHLEY_V1;
				calorimeterCOM = m.COMInstruments[i];

				pressureHighRange = INSTRUMENT_KEITHLEY;
				pressureHighRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureHighRangeCOM = m.COMInstruments[i];

				break;

			case CALO_V1_KEITHLEY:

				calorimeter = INSTRUMENT_KEITHLEY;
				calorimeterChannel = INSTRUMENT_KEYTHLEY_V1;
				calorimeterCOM = m.COMInstruments[i];

				break;

			case INSTRUMENT_KEYTHLEY_LP_V2:

				pressureLowRange = INSTRUMENT_KEITHLEY;
				pressureLowRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureLowRangeCOM = m.COMInstruments[i];

				break;

			case INSTRUMENT_KEYTHLEY_HP_V2:

				pressureHighRange = INSTRUMENT_KEITHLEY;
				pressureHighRangeChannel = INSTRUMENT_KEYTHLEY_V2;
				pressureHighRangeCOM = m.COMInstruments[i];

				break;

			default:
				throw; // Should never be reached
				break;
			}
			break;


		case INSTRUMENT_MENSOR:
			if (m.FunctionInstruments[i] == INSTRUMENT_MENSOR_LP)
			{
				pressureLowRange = INSTRUMENT_MENSOR;
				pressureLowRangeChannel = MENSOR_VOIE;
				pressureLowRangeCOM = m.COMInstruments[i];
			}
			else if (m.FunctionInstruments[i] == INSTRUMENT_MENSOR_HP)
			{
				pressureHighRange = INSTRUMENT_MENSOR;
				pressureHighRangeChannel = MENSOR_VOIE;
				pressureHighRangeCOM = m.COMInstruments[i];
			}

			break;
		case INSTRUMENT_NONE:
		case INSTRUMENT_UNDEF:
		case INSTRUMENT_INEXIST:
			// If there no instrument set, don't do anything
			break;
		default:
			// Should never be reached
			throw;
			break;
		}
	}

	// Now the init functions can be called
	std::string errorInit;
	std::string tempErr;
	bool successs = true;

	// Now that we have all the data we can call the initiator functions
	if (!InitiateCalorimeter()) {
		GetErrorCalrimeter(&tempErr);
		errorInit += tempErr + "\n";
		successs = false;
	}
	if (!InitiatePressureLowRange()) {
		GetErrorLowRange(&tempErr);
		errorInit += tempErr + "\n";;
		successs = false;
	}
	if (!InitiatePressureHighRange()) {
		GetErrorHighRange(&tempErr);
		errorInit += tempErr + "\n";;
		successs = false;
	}
	if (!successs)
	{
		messageHandler.DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
	}
}


SerialInstruments::~SerialInstruments()
{
	if (keithley != nullptr)
		delete keithley;
	if (mens_LowRange != nullptr)
		delete mens_LowRange;
	if (mens_HighRange != nullptr)
		delete mens_HighRange;
}



//
// initiation functions
//

bool SerialInstruments::SetSensitivity(double sensitivity_calo, double sensitivity_lp, double sensitivity_hp)
{
	this->sensitivity_calo = sensitivity_calo;
	this->sensitivity_calo = sensitivity_calo;
	this->sensitivity_calo = sensitivity_calo;
	return false;
}

bool SerialInstruments::InitiateCalorimeter()
{
	if (calorimeter != INSTRUMENT_INEXIST)
	{
		if (calorimeter == INSTRUMENT_KEITHLEY && keithleyInitiated == false)
		{
			keithleyInitiated = true;
			keithley = new Keithley(calorimeterCOM);
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
			keithley = new Keithley(pressureLowRangeCOM);
		}

		if (pressureLowRange == INSTRUMENT_MENSOR)
		{
			mens_LowRange = new Mensor(pressureLowRangeCOM);
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
			keithley = new Keithley(pressureHighRangeCOM);
		}

		if (pressureHighRange == INSTRUMENT_MENSOR)
		{
			mens_HighRange = new Mensor(pressureHighRangeCOM);
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
			*result = converter.ConversionCalo(*result, sensitivity_calo);
			return success;
		}
		if (calorimeterChannel == INSTRUMENT_KEYTHLEY_V2) {
			success = keithley->ReadChannel2(result);
			*result = converter.ConversionCalo(*result, sensitivity_calo);
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
			*result = converter.ConversionBP(*result, sensitivity_lp);
			return success;
		}

		if (pressureLowRangeChannel == INSTRUMENT_KEYTHLEY_V2) {
			success = keithley->ReadChannel2(result);
			*result = converter.ConversionBP(*result, sensitivity_lp);
			return success;
		}

	}
	
	if (pressureLowRange == INSTRUMENT_MENSOR) {
		success = mens_LowRange->ReadMensor(result);
		*result = converter.ConversionBP(*result, sensitivity_lp);
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
			*result = converter.ConversionHP(*result, sensitivity_hp);
			return success;
		}

		if (pressureHighRangeChannel == INSTRUMENT_KEYTHLEY_V2) {
			success = keithley->ReadChannel2(result);
			*result = converter.ConversionHP(*result, sensitivity_hp);
			return success;
		}

	}

	if (pressureHighRange == INSTRUMENT_MENSOR) {
		success = mens_HighRange->ReadMensor(result);
		*result = converter.ConversionHP(*result, sensitivity_hp);
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
