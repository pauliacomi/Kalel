#include "SerialInstruments.h"

#include "../../MessageHandler.h"

#include "../../../Kalel Shared/Resources/DefineInstruments.h"
#include "../../../Kalel Shared/Resources/DefineText.h"

#include "../../../Kalel Shared/Com Classes/MachineSettings.h"

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
	instruments = m.instruments;

	for (int i = 0; i < instruments.size(); i++)
	{
		switch (instruments[i].instrument)
		{
		case INSTRUMENT_KEITHLEY:
			try	{
				keithleys.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {

			}
			break;

		case INSTRUMENT_MENSOR:
			try {
				mensors.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {

			}
			break;

		case INSTRUMENT_NI_USB_6008:
			try {
				NI_USB_6008s.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {

			}
			break;
		
		case INSTRUMENT_NI_USB_9211A:
			try {
				NI_USB_9211As.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {
				std::string errorInit;
				messageHandler.DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
			}
			break;

		case INSTRUMENT_NONE:
		case INSTRUMENT_UNDEF:
			break;
		default:
			// Should never be reached
			throw;
			break;
		}
	}
}


SerialInstruments::~SerialInstruments()
{
}



//
// initiation functions
//

void SerialInstruments::Reset(MachineSettings & m)
{
	for (int i = 0; i < instruments.size(); i++)
	{
		switch (instruments[i].instrument)
		{
		case INSTRUMENT_KEITHLEY:
			if (instruments[i].function = )
			{

			}
			try {
				keithleys.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {

			}
			break;

		case INSTRUMENT_MENSOR:
			try {
				mensors.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {

			}
			break;

		case INSTRUMENT_NI_USB_6008:
			try {
				NI_USB_6008s.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {

			}
			break;

		case INSTRUMENT_NI_USB_9211A:
			try {
				NI_USB_9211As.emplace_back(instruments[i].port);
			}
			catch (const std::exception&) {
				messageHandler.DisplayMessageBox(MESSAGE_INSTRUMENT_INIT_FAIL, MB_ICONERROR | MB_OK, false, errorInit);
			}
			break;

		case INSTRUMENT_NONE:
		case INSTRUMENT_UNDEF:
			break;
		default:
			// Should never be reached
			throw;
			break;
		}
	}
}

std::function<void(double* p)> SerialInstruments::Link(Reader & r)
{
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
