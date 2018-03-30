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


double NI_USB_9211A::Read(unsigned int channel)
{
	char chanBuf[30] = { 0 };

	// Write channel string
	sprintf_s(chanBuf, "Dev%d/ai%d", portUSB, channel);

	float64     data[1] = { 0 };
	if(ReadAnalog(chanBuf, data))
		return data[0];
	return 0;
}

bool NI_USB_9211A::ReadAnalog(char * chan, float64 * data)
{	
	// Task variables
    TaskHandle  taskHandle		= nullptr;				// NiDAQ task
    int32       error			= 0;					// Error number
	
	// Write variables
	int32       pointsRead		= 0;					// Output points read

	// Create thrermocouple Task and Channel
	DAQmxErrChk (DAQmxCreateTask ("ReadThermocouple", &taskHandle));
	DAQmxErrChk (DAQmxCreateAIThrmcplChan(taskHandle, chan, "", t_min, t_max, DAQmx_Val_DegC, DAQmx_Val_K_Type_TC, DAQmx_Val_BuiltIn, 0, ""));
	
	// Start Task (configure port)
	DAQmxErrChk (DAQmxStartTask (taskHandle));
	// Run task for single reading of value
	DAQmxErrChk (DAQmxReadAnalogF64 (taskHandle, samplesPerChan, timeout, 0, data, maxBufSize, &pointsRead, NULL));

	// In case of error
Error:
    if (DAQmxFailed (error))	// Extended error string
	{
		unsigned int errorLength = DAQmxGetExtendedErrorInfo(0, 0);
		char * errBuff = new char[errorLength];							
		DAQmxGetExtendedErrorInfo(errBuff, errorLength);
		LOG(logDEBUG) << errBuff;
		delete errBuff;
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

bool NI_USB_9211A::ReadAllChannels(double * readArray)
{
	char chanBuf[30] = { 0 };

	// Write channel string
	sprintf_s(chanBuf, "Dev%d/ai%d:%d", portUSB, chan_min, chan_max);

	float64     data[maxBufSize];
	bool result = ReadAnalog(chanBuf, data);

	for (size_t i = 0; i < sizeof(readArray) / sizeof(readArray[0]); i++)
	{
		if (i > maxBufSize) { break; }
		readArray[i] = data[i];
	}

	if (result)
	{
		return true;
	}
	return false;
}
