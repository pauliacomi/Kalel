#include "NI_USB_9211A.h"

#include "../../../Kalel Shared/log.h"

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { goto Error; } }


NI_USB_9211A::NI_USB_9211A(int dev)
{
	portUSB = dev;
}

NI_USB_9211A::~NI_USB_9211A(void)
{
}

int NI_USB_9211A::GetComPort()
{
	return portUSB;
}

void NI_USB_9211A::SetComPort(int dev)
{
	portUSB = dev;
}


bool NI_USB_9211A::ReadAll(float64 data[bufferSize])
{	
	// Task variables
    TaskHandle  taskHandle		= nullptr;
    int32       error			= 0;
	char        errBuff[2048]	= { '\0' };				// C string for error
	int32       pointsRead		= 0;

	// Write channel string
	sprintf_s(chan, "Dev%d/ai%d:%d", portUSB, analog_start, analog_end);

	// Create thrermocouple Task and Channel
	DAQmxErrChk (DAQmxCreateTask ("ReadThermocouple", &taskHandle));
	DAQmxErrChk (DAQmxCreateAIThrmcplChan(taskHandle, chan, "", min, max, DAQmx_Val_DegC, DAQmx_Val_K_Type_TC, DAQmx_Val_BuiltIn, 0, ""));
	
	// Start Task (configure port)
	DAQmxErrChk (DAQmxStartTask (taskHandle));
	// Run task for single reading of value
	DAQmxErrChk (DAQmxReadAnalogF64 (taskHandle, pointsToRead, timeout, 0, data, bufferSize, &pointsRead, NULL));

	// In case of error
Error:
    if (DAQmxFailed (error))
	{
		DAQmxGetExtendedErrorInfo (errBuff, 2048);
		LOG(logDEBUG) << errBuff;
		for (size_t i = 0; i < bufferSize; i++)
		{
			data[i] = 0;
		}
	}

	// Clear task to free memory
	if (taskHandle != 0)
	{
		DAQmxStopTask(taskHandle);
		DAQmxClearTask(taskHandle);
	}

	// Return success
	if (error == 0)
		return true;
	return false;
}

//////////////////////////////////////////
//
// Individual functions to read data

bool NI_USB_9211A::ReadAllChannels(double* Valeur0,double* Valeur1,double* Valeur2,double* Valeur3)
{
	float64     data[bufferSize];
	bool result = ReadAll(data);

	*Valeur0 = data[0];
	*Valeur1 = data[1];
	*Valeur2 = data[2];
	*Valeur3 = data[3];

	if (result == true)
	{
		return true;
	}
	return false;
}


double NI_USB_9211A::Read(unsigned int channel)
{
	float64     data[bufferSize];
	bool result = ReadAll(data);

	return data[channel];
}